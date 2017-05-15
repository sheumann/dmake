/* RCS      -- $Header: /u2/dvadura/src/generic/dmake/src/RCS/infer.c,v 1.1 1992/01/24 03:27:21 dvadura Exp $
-- SYNOPSIS -- infer how to make a target.
-- 
-- DESCRIPTION
--	This file contains the code to infer a recipe, and possibly some new
--	prerequisites for a target which dmake does not know how to make, or
--	has no explicit recipe.
--
--	The inference fails if no path through the inference graph can be
--	found by which we can make the target.
-- 
-- AUTHOR
--      Dennis Vadura, dvadura@watdragon.uwaterloo.ca
--      CS DEPT, University of Waterloo, Waterloo, Ont., Canada
--
-- COPYRIGHT
--      Copyright (c) 1990 by Dennis Vadura.  All rights reserved.
-- 
--      This program is free software; you can redistribute it and/or
--      modify it under the terms of the GNU General Public License
--      (version 1), as published by the Free Software Foundation, and
--      found in the file 'LICENSE' included with this distribution.
-- 
--      This program is distributed in the hope that it will be useful,
--      but WITHOUT ANY WARRANTY; without even the implied warrant of
--      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--      GNU General Public License for more details.
-- 
--      You should have received a copy of the GNU General Public License
--      along with this program;  if not, write to the Free Software
--      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
--
-- LOG
--     $Log: infer.c,v $
 * Revision 1.1  1992/01/24  03:27:21  dvadura
 * dmake Version 3.8, Initial revision
 *
*/

#include "extern.h"

/* attributes that get transfered from the % start cell to the inferred
 * cells. */

#define A_TRANSFER  (A_EPILOG | A_PRECIOUS | A_SILENT | A_SHELL | A_SETDIR |\
		     A_SEQ | A_LIBRARY | A_IGNORE | A_PROLOG | A_SWAP |\
		     A_NOSTATE )


/* Define local static functions */
static DFALINKPTR _dfa_subset  ANSI((DFALINKPTR, DFASETPTR));
static void 	  _free_dfas   ANSI((DFALINKPTR));
static int	  _count_dots  ANSI((char *));
static char *	  _build_name  ANSI((char *, char *, char *));
static void	  _free_icells ANSI(());
static ICELLPTR   _union_iset  ANSI((ICELLPTR, ICELLPTR));
static ICELLPTR   _add_iset    ANSI((ICELLPTR,ICELLPTR,CELLPTR,DFALINKPTR,
				     CELLPTR,int,int,char *,char *, int));
static ICELLPTR   _derive_prerequisites ANSI((ICELLPTR, ICELLPTR *));
static char *	  _dump_inf_chain ANSI((ICELLPTR, int, int));

static int _prep = -1;	/* Integer value of Prep variable */


PUBLIC void
Infer_recipe( cp, setdirroot )/*
================================
   Perform a breadth-first search of the inference graph and return if
   possible an inferred set of prerequisites for making the current target.
*/
CELLPTR cp;
CELLPTR setdirroot;
{
   ICELLPTR nomatch, match;

   DB_ENTER("Infer_recipe");

   if( cp->ce_attr & A_NOINFER ) {DB_VOID_RETURN;}
   if( _prep == -1 ) _prep = atoi(Prep);  /* _dfa_subset needs _prep */

   match = NIL(ICELL);
   nomatch = _add_iset( NIL(ICELL), NIL(ICELL), NIL(CELL), NIL(DFALINK),
			setdirroot, _prep+_count_dots(cp->CE_NAME), 0,
			_strdup(cp->CE_NAME), NIL(char),
			cp->ce_time != (time_t)0L);

   /* Make sure we try whole heartedly to infer at least one suffix */
   if( nomatch->ic_dmax == 0 ) ++nomatch->ic_dmax;

   DB_EXECUTE( "inf", _dump_iset("nomatch",nomatch); );

   while( nomatch != NIL(ICELL) ) {
      ICELLPTR new_nomatch = NIL(ICELL);
      ICELLPTR ic, pmatch, mmatch;
      CELLPTR  prereq;
      int      first;

      for( ic=nomatch; ic != NIL(ICELL); ic=ic->ic_next ) {
	 int ipush = FALSE;

	 if( ic->ic_dir ) ipush = Push_dir(ic->ic_dir, ic->ic_name, FALSE);
	 match = _union_iset(match, _derive_prerequisites(ic, &new_nomatch));
	 if( ipush ) Pop_dir(FALSE);
      }

      DB_EXECUTE( "inf", _dump_iset("match",match); );
      DB_EXECUTE( "inf", _dump_iset("nomatch",new_nomatch); );

      /* We have now deduced the two sets MATCH and NOMATCH.  MATCH holds the
       * set of edges that we encountered that matched.  If this set is empty
       * then we can apply transitive closure (if enabled) to the elements of
       * NOMATCH to see if we can find some other method to make the target.
       *
       * If MATCH is non-empty, we have found a method for making the target.
       * It is the shortest method for doing so (ie. uses fewest number of
       * steps).  If MATCH contains more than one element then we have a
       * possible ambiguity.
       */
      if( match == NIL(ICELL) ) {
	 nomatch = new_nomatch;
	 if( Transitive ) continue;
	 goto all_done;
      }

      /* Ok, we have a set of possible matches in MATCH, we should check the
       * set for ambiguity.  If more than one inference path exists of the
       * same depth, then we may issue an ambigous inference error message.
       *
       * The message is suppressed if MATCH contains two elements and one of
       * them is the empty-prerequisite-rule.  In this case we ignore the
       * ambiguity and take the rule that infers the prerequisite.
       *
       * Also if there are any chains that rely on a non-existant prerequisite
       * that may get made because it has a recipe then we prefer any that
       * rely on existing final prerequisites over those that we have to make.
       *
       * NOTE:  May turn this around at some point.
       */

      /* Split out those that have to be made from those that end in
       * prerequisites that already exist. */
      pmatch = mmatch = NIL(ICELL);
      for(; match; match = ic ) {
	 ic = match->ic_next;
	 match->ic_next = NIL(ICELL);

	 if( match->ic_exists )
	    pmatch = _union_iset(pmatch, match);
	 else
	    mmatch = _union_iset(mmatch, match);
      }

      if( pmatch )
	 match = pmatch;
      else
	 match = mmatch;

      /* Make sure it is unique */
      if( match->ic_next != NIL(ICELL) ) {
	 int dump = (match->ic_next->ic_next != NIL(ICELL));

	 /* Check for definite ambiguity */
	 if( !dump )
	    if( (match->ic_meta->ce_prq && match->ic_next->ic_meta->ce_prq) ||
	        (!match->ic_meta->ce_prq && !match->ic_next->ic_meta->ce_prq)  )
	       dump = TRUE;
	    else if(!match->ic_meta->ce_prq && match->ic_next->ic_meta->ce_prq )
	       match = match->ic_next;

	 if( dump ) {
	    int count = 1;

	    Continue = TRUE;
	    Error( "Ambiguous inference chains for target '%s'", cp->CE_NAME );
	    for( ic=match; ic; ic=ic->ic_next )
	       (void) _dump_inf_chain(ic, TRUE, count++);
	    Fatal( "resolve ambiguity before proceeding.");
	    /*NOTREACHED*/
	 }
      }

      /* MATCH now points at the derived recipe.  We must now take cp, and
       * construct the correct graph so that the make may proceed. */

      if( Verbose & V_INFER ) {
	 char *tmp = _dump_inf_chain(match, TRUE, FALSE);
	 printf("%s:  Inferring prerequistes and recipes using:\n%s:  ... %s\n",
		 Pname, Pname, tmp );
	 FREE(tmp);
      }

      pmatch = NIL(ICELL);
      prereq = NIL(CELL);
      first  = TRUE;

      while( match ) {
         CELLPTR infcell=NIL(CELL);

	 /* Compute the inferred prerequisite first. */
	 if( match->ic_name ) {
	    if( match->ic_meta )
	       infcell = Def_cell( match->ic_name );
	    else
	       infcell = cp;

	    infcell->ce_flag |= F_TARGET;

	    if( infcell != cp ) {
	       infcell->ce_flag |= F_INFER;
	       if( !first ) infcell->ce_flag |= F_REMOVE;
	    }

	    if( !match->ic_flag )
	       infcell->ce_attr |= A_NOINFER;

	    first = FALSE;
	 }

	 /* Add global prerequisites from previous rule if there are any and
	  * the recipe. */
	 if( pmatch ) {
	    CELLPTR imeta = pmatch->ic_meta;
	    LINKPTR lp;

	    infcell->ce_per   = pmatch->ic_dfa->dl_per;
	    infcell->ce_attr |= (imeta->ce_attr & A_TRANSFER);

	    if( !(infcell->ce_flag & F_RULES) ) {
	       infcell->ce_flag |= (imeta->ce_flag&(F_SINGLE|F_GROUP))|F_RULES;
	       infcell->ce_recipe = imeta->ce_recipe;
	    }

	    pmatch->ic_dfa->dl_per = NIL(char);

	    /* If infcell already had a directory set then modify it based on
	     * whether it was the original cell or some intermediary. */
	    if( imeta->ce_dir )
	       if( infcell->ce_dir && infcell == cp ) {
		  /* cp->ce_dir was set and we have pushed the directory prior
		   * to calling this routine.  We should therefore pop it and
		   * push the new concatenated directory required by the
		   * inference. */
		  infcell->ce_dir=_strdup(Build_path(infcell->ce_dir,
						     imeta->ce_dir));
	       }
	       else
		  infcell->ce_dir = imeta->ce_dir;

	    for( lp=imeta->ce_indprq; lp != NIL(LINK); lp=lp->cl_next ) {
	       char    *name = lp->cl_prq->CE_NAME;
	       CELLPTR tcp;

	       name = _build_name( cp->CE_NAME, name, infcell->ce_per );
	       tcp  = Def_cell( name );
	       tcp->ce_flag |= F_REMOVE;
	       Add_prerequisite( infcell, tcp, FALSE, FALSE );

	       if( Verbose & V_INFER )
		  printf( "%s:  Inferred indirect prerequisite [%s]\n",
			  Pname, name );
	       FREE(name);
	    }
	 }

	 /* Add the previous cell as the prerequisite */
	 if( prereq )
	    (Add_prerequisite(infcell,prereq,FALSE,FALSE))->cl_flag |= F_TARGET;

	 pmatch = match;
	 prereq = infcell;
	 match  = match->ic_parent;
      }

      DB_PRINT("inf", ("Terminated due to a match"));
      break;
   }

all_done:
   _free_icells();

   DB_VOID_RETURN;
}


static ICELLPTR
_derive_prerequisites( ic, nnmp )/*
===================================
   Take a cell and derive a set of prerequisites from the cell.  Categorize
   them into those that MATCH (ie. those that we found in the file system),
   and those that do not match NOMATCH that we may possibly have a look at
   later.  When we process the next level of the breadth-first search.
   
   Once MATCH is non-empty we will stop inserting elements into NOMATCH
   since we know that either MATCH is successful and unique or it will
   issue an ambiguity error.  We will never go on to look at elements
   in NOMATCH after wards. */
ICELLPTR ic;
ICELLPTR *nnmp;
{
   ICELLPTR   match = NIL(ICELL);
   DFALINKPTR pdfa;
   DFALINKPTR dfas;

   DB_ENTER("_derive_prerequisites");

   /* If none of the inference nodes match then forget about the inference.
    * The user did not tell us how to make such a target.  We also stop the
    * Inference if the new set of DFA's is a proper subset of a previous
    * subset and it's PREP counts exceed the value of Prep.
    */
   dfas = _dfa_subset( Match_dfa(ic->ic_name), &ic->ic_dfastack );

   DB_EXECUTE("inf", _dump_dfa_stack(dfas, &ic->ic_dfastack); );

   /* Ok, we have nothing here to work with so return an empty cell. */
   if( dfas == NIL(DFALINK) ) {
      DB_PRINT( "mem", ("%s:<- mem %ld",ic->ic_name, (long)coreleft()));
      DB_PRINT( "inf", ("<<< Exit, no dfas, cp = %04x", NIL(CELL)) );
      DB_RETURN( NIL(ICELL) );
   }

   /* Save the dfas, we are going to use on the stack for this cell. */
   ic->ic_dfastack.df_set = dfas;

   /* Run through the %-meta cells, build the prerequisite cells.  For each
    * %-meta go through it's list of edges and try to use each in turn to
    * decuce a likely prerequisite.  We perform a breadth-first search
    * matching the first path that results in a unique method for making the
    * target. */
   for( pdfa = dfas; pdfa != NIL(DFALINK); pdfa = pdfa->dl_next ) {
      LINK tl;
      LINKPTR edge;
      CELLPTR pmeta;

      pmeta = pdfa->dl_meta;
      DB_PRINT( "inf", ("Using dfa:  [%s]", pmeta->CE_NAME) );

      /* If the %-meta is a singleton meta then deal with it differently from
       * the case when it is a bunch of %-meta's found on the original entries
       * prerequisite list. */
      if( pmeta->ce_flag & F_MULTI )
	 edge = pmeta->ce_prq;
      else {
	 tl.cl_prq = pmeta;
	 tl.cl_next = NIL(LINK);
	 edge = &tl;
      }

      /* Now run through the list of prerequisite edge's for the %-meta. */
      for( ; edge != NIL(LINK); edge = edge->cl_next ) {
	 HASHPTR  thp;		/* temporary hash table pointer		*/
	 HASH     iprqh;	/* hash cell for new prerequisite	*/
	 CELL     iprq;		/* inferred prerequisite to look for	*/
	 CELLPTR  idirroot;	/* Inferred prerequisite root		*/
	 CELLPTR  nidirroot;	/* Inferred prerequisite root		*/
	 STRINGPTR ircp;	/* Inferred prerequisites recipe	*/
	 char     *idir;	/* directory to CD to.			*/
	 int      ipush = 0;	/* flag for push on inferred prereq     */
	 char     *name = NIL(char); 		/* prerequisite name    */
	 CELLPTR  meta = edge->cl_prq;
	 int	  dmax_fix;
	 int      trans;
	 int      noinf;
	 int	  exists;
	 
	 if( meta->ce_prq ) name = meta->ce_prq->cl_prq->CE_NAME;

	 DB_PRINT( "inf", ("Trying edge from [%s] to [%s] for [%s]",
		   meta->CE_NAME, name?name:"(nil)", ic->ic_name) );

	 /* Set the temp CELL used for building prerequisite candidates to
	  * all zero so that we don't have to keep initializing all the
	  * fields. */
	 {
	    register char *s = (char *) &iprq;
	    register int   n = sizeof(CELL);
	    while( n ) { *s++ = '\0'; n--; }
	 }

	 nidirroot = idirroot = ic->ic_setdirroot;
	 iprq.ce_name = &iprqh;

	 if( name ) {
	    /* Build the prerequisite name from the %-meta prerequisite given
	     * for the %-meta rule. */
	    iprqh.ht_name = _build_name( ic->ic_name, name, pdfa->dl_per );
	    if((dmax_fix = (_count_dots(name)-_count_dots(meta->CE_NAME))) < 0)
	       dmax_fix = 0;

	    if( !strcmp(ic->ic_name, iprqh.ht_name) ||
		(_count_dots(iprqh.ht_name) > ic->ic_dmax + dmax_fix) ) {
	       FREE( iprqh.ht_name );
	       continue;
	    }

	    DB_PRINT( "inf", ("Checking prerequisite [%s]", iprqh.ht_name) );

	    /* See if the prerequisite CELL has been previously defined.  If
	     * it has, then make a copy of it into iprq, and use it to try
	     * the inference.  We make the copy so that we don't modify the
	     * stat of the inferred cell if the inference fails.
	     */
	    thp = Get_name( iprqh.ht_name, Defs, FALSE );
	    if(thp != NIL(HASH)) {
	       iprq = *thp->CP_OWNR;
	       ircp = iprq.ce_recipe;
	    }
	    else
	       ircp = NIL(STRING);
	 }
	 else
	    iprqh.ht_name = NIL(char);


	 /* If the %-meta has a .SETDIR set then we change to the new
	  * directory prior to performing the stat of the new prerequisite.
	  * If the change of directory fails then the rule is droped from
	  * further consideration.
	  */
	 if( iprq.ce_dir ) {
	    if( ipush = Push_dir(iprq.ce_dir, iprqh.ht_name, TRUE) ) {
	       nidirroot = thp->CP_OWNR;
	       idir      = Pwd;
	    }
	    else {
	       if( iprqh.ht_name ) FREE( iprqh.ht_name );
	       continue;
	    }
	 }
	 else
	    idir = NIL(char);


	 /* Stat the inferred prerequisite.
	  */
	 if( name ) {
	    if( Verbose & V_INFER )
	       printf( "%s:  Trying prerequisite [%s] for [%s]\n", Pname,
		       iprqh.ht_name, ic->ic_name );

	    if( !(iprq.ce_flag & F_STAT) ) Stat_target(&iprq, FALSE);
	 }


	 /* If the STAT succeeded or if the prerequisite has a recipe for
	  * making it then it's a match and a candidate for getting infered.
	  * Otherwise it is not a match, and we cannot yet tell if it is
	  * going to be a successful path to follow, so we save it for
	  * later consideration.
	  */
	 noinf = ((Glob_attr)&A_NOINFER);
	 if( meta->ce_prq )
	    noinf |= ((meta->ce_prq->cl_prq->ce_attr)&A_NOINFER);
	 trans = Transitive || !noinf;
	 exists = (iprq.ce_time != (time_t)0L);

	 if( exists || (ircp != NIL(STRING)) || !name ) {
	    match = _add_iset( match, ic, meta, pdfa, idirroot, ic->ic_dmax,
			       trans, iprq.ce_name->ht_name, idir, exists );
	    DB_PRINT("inf",("Added to MATCH %s",iprq.ce_name->ht_name));
	 }
	 else if( !noinf && match == NIL(ICELL) ) {
	    *nnmp = _add_iset( *nnmp, ic, meta, pdfa, nidirroot, ic->ic_dmax,
			       trans, iprq.ce_name->ht_name, idir, exists );
	    DB_PRINT("inf",("Added to NOMATCH %s",iprq.ce_name->ht_name));
	 }

	 /* If we pushed a directory for the inferred prerequisite then
	  * pop it.
	  */
	 if( ipush ) Pop_dir(FALSE);
	 if( iprqh.ht_name ) FREE(iprqh.ht_name);
      }
   }

   DB_RETURN(match);
}


static char *
_build_name( tg, meta, per )
char *tg;
char *meta;
char *per;
{
   char    *name;

   name = Apply_edit( meta, "%", per, FALSE, FALSE );
   if( strchr(name, '$') ) {
      HASHPTR m_at;
      char *tmp;

      m_at = Def_macro( "@", tg, M_MULTI );
      tmp = Expand( name );

      if( m_at->ht_value != NIL(char) ) {
	 FREE( m_at->ht_value );
	 m_at->ht_value = NIL(char);
      }

      if( name != meta ) FREE( name );
      name = tmp;
   }
   else if( name == meta )
      name = _strdup( name );

   return(name);
}


static DFALINKPTR
_dfa_subset( pdfa, stack )/*
============================
   This is the valid DFA subset computation.  Whenever a CELL has a Match_dfa
   subset computed this algorithm is run to see if any of the previously
   computed sets on the DFA stack are proper subsets of the new set.  If they
   are, then any elements of the matching subset whose Prep counts exceed
   the allowed maximum given by Prep are removed from the computed DFA set,
   and hence from consideration, thereby cutting off the cycle in the
   inference graph. */
DFALINKPTR	   pdfa;
register DFASETPTR stack;
{
   register DFALINKPTR element;
   DFALINKPTR          nelement;

   DB_ENTER( "_dfa_subset" );

   for(; pdfa != NIL(DFALINK) && stack != NIL(DFASET); stack = stack->df_next) {
      int subset = TRUE;

      for( element=stack->df_set; subset && element != NIL(DFALINK);
           element=element->dl_next ) {
         register DFALINKPTR subel;

	 for( subel = pdfa;
	      subel != NIL(DFALINK) && (subel->dl_meta != element->dl_meta);
	      subel = subel->dl_next );

	 if( subset = (subel != NIL(DFALINK)) ) element->dl_member = subel;
      }

      if( subset )
	 for( element=stack->df_set; element != NIL(DFALINK);
	      element=element->dl_next ) {
	    DFALINKPTR mem = element->dl_member;
	    int        npr = element->dl_prep + 1;

	    if( npr > _prep )
	       mem->dl_delete++;
	    else
	       mem->dl_prep = npr;
	 }
   }

   for( element = pdfa; element != NIL(DFALINK); element = nelement ) {
      nelement = element->dl_next;

      if( element->dl_delete ) {
	 /* A member of the subset has a PREP count equal to PREP, so
	  * it should not be considered further in the inference, hence
	  * we remove it from the doubly linked set list */
	 if( element == pdfa )
	    pdfa = element->dl_next;
	 else
	    element->dl_prev->dl_next = element->dl_next;

	 if( element->dl_next != NIL(DFALINK) )
	    element->dl_next->dl_prev = element->dl_prev;

	 DB_PRINT("inf", ("deleting dfa [%s]", element->dl_meta->CE_NAME));
	 FREE( element->dl_per );
	 FREE( element );
      }
   }

   DB_RETURN( pdfa );
}



static void
_free_dfas( chain )/*
=====================
   Free the list of DFA's constructed by Match_dfa, and linked together by
   LINK cells.  FREE the % value as well, as long as it isn't NIL. */
DFALINKPTR chain;
{
   register DFALINKPTR tl;

   DB_ENTER( "_free_dfas" );

   for( tl=chain; tl != NIL(DFALINK); chain = tl ) {
      tl = tl->dl_next;

      DB_PRINT( "inf", ("Freeing DFA [%s], %% = [%s]", chain->dl_meta->CE_NAME,
                chain->dl_per) );

      if( chain->dl_per != NIL(char) ) FREE( chain->dl_per );
      FREE( chain );
   }

   DB_VOID_RETURN;
}


static int
_count_dots( name )/*
=====================*/
char *name;
{
   register char *p;
   register int  i = 0;

   for( p = name; *p; p++ ) if(*p == '.') i++;

   return( i );
}


static ICELLPTR _icells = NIL(ICELL);
#ifdef DBUG
static int _icell_cost = 0;
#endif

static ICELLPTR
_add_iset( iset, parent, meta, dfa, setdirroot, dmax, noinf, name, dir, exists)
ICELLPTR   iset;
ICELLPTR   parent;
CELLPTR    meta;
DFALINKPTR dfa;
CELLPTR    setdirroot;
int	   dmax;
int	   noinf;
char	  *name;
char	  *dir;
int	   exists;
{
   ICELLPTR icell;

   DB_ENTER("_add_iset");
   TALLOC(icell, 1, ICELL);

   DB_EXECUTE("inf", _icell_cost+=(sizeof(ICELL)+strlen(dir?dir:"")+strlen(name?name:"")+2););

   icell->ic_meta = meta;
   icell->ic_dfa  = dfa;
   icell->ic_setdirroot = setdirroot;

   if( parent ) icell->ic_dfastack.df_next = &parent->ic_dfastack;

   icell->ic_dmax = dmax;
   icell->ic_dir = _strdup(dir);
   icell->ic_name = _strdup(name);
   icell->ic_parent = parent;
   icell->ic_next = iset;
   icell->ic_flag = noinf;
   icell->ic_exists = exists;

   icell->ic_link = _icells;
   _icells = icell;

   DB_RETURN(icell);
}


static void
_free_icells()
{
   register ICELLPTR ic;

   DB_ENTER("_free_icells");

   for( ; _icells; _icells = ic ) {
      ic = _icells->ic_link;

      _free_dfas(_icells->ic_dfastack.df_set);
      if( _icells->ic_dir ) FREE(_icells->ic_dir);
      if( _icells->ic_name) FREE(_icells->ic_name);
      FREE(_icells);
   }

   DB_PRINT("inf",("Used %d memory for icells",_icell_cost));
   DB_EXECUTE("inf", _icell_cost=0; );

   DB_VOID_RETURN;
}


static ICELLPTR
_union_iset( iset, uset )
ICELLPTR iset;
ICELLPTR uset;
{
   register ICELLPTR ic;

   if( iset == NIL(ICELL) ) return(uset);

   for( ic=iset; ic->ic_next != NIL(ICELL); ic=ic->ic_next );
   ic->ic_next = uset;

   return(iset);
}


static char *
_dump_inf_chain( ip, flag, print )/*
====================================*/
ICELLPTR ip;
int      flag;
int	 print;
{
   char *tmp;

   if( ip == NIL(ICELL) ) return(NIL(char));

   tmp = _dump_inf_chain(ip->ic_parent, FALSE, FALSE);

   if( ip->ic_meta ) {
      tmp = _strjoin(tmp, "(", -1, TRUE);
      tmp = _strjoin(tmp, ip->ic_meta->CE_NAME, -1, TRUE);

      if( ip->ic_dir && !*ip->ic_dir ) {
	 tmp = _strjoin(tmp, "[", -1, TRUE);
	 if( strncmp(Makedir,ip->ic_dir, strlen(Makedir)) )
	    tmp = _strjoin(tmp, ip->ic_dir, -1, TRUE);
	 else
	    tmp = _strjoin(tmp, ip->ic_dir+strlen(Makedir)+1, -1, TRUE);
	 tmp = _strjoin(tmp, "]", -1, TRUE);
      }
      tmp = _strjoin(tmp, (ip->ic_name)?") -->":")", -1, TRUE);
   }
   
   if( ip->ic_name ) tmp = _strapp( tmp, ip->ic_name );

   if( flag && ip->ic_meta->ce_prq) {
      tmp = _strjoin(tmp, "(", -1, TRUE);
      tmp = _strjoin(tmp, ip->ic_meta->ce_prq->cl_prq->CE_NAME, -1, TRUE);
      tmp = _strjoin(tmp, ")", -1, TRUE);
   }

   if( print ) {
      fprintf( stderr, "%s:  %2d. %s\n", Pname, print, tmp );
      FREE(tmp);
      tmp = NIL(char);
   }

   return(tmp);
}


#ifdef DBUG
_dump_dfa_stack(dfas, dfa_stack)
DFALINKPTR dfas;
DFASETPTR  dfa_stack;
{
   register DFALINKPTR pdfa;
   char      *tmp = NIL(char);
   DFASETPTR ds;

   for( pdfa = dfas; pdfa != NIL(DFALINK); pdfa = pdfa->dl_next )
      tmp = _strapp( tmp, pdfa->dl_meta->CE_NAME );

   tmp = _strapp( tmp, ":: {" );
   for( ds = dfa_stack; ds != NIL(DFASET); ds = ds->df_next ) {
      tmp = _strapp( tmp, "[" );
      for( pdfa = ds->df_set; pdfa != NIL(DFALINK); pdfa = pdfa->dl_next )
	 tmp = _strapp( tmp, pdfa->dl_meta->CE_NAME );
      tmp = _strapp( tmp, "]" );
   }
   tmp = _strapp( tmp, "}" );

   printf( "DFA set and stack contents:\n%s\n", tmp );
   FREE(tmp);
}


_dump_iset( name, iset )
char     *name;
ICELLPTR iset;
{
   int cell = 0;

   printf( "**** ISET for %s\n", name );
   for( ; iset != NIL(ICELL); iset = iset->ic_next ){
      printf( "cell %d\n", cell++ );
      if( iset->ic_meta )
	 printf( "edge: %s --> %s\n", iset->ic_meta->CE_NAME,
		 iset->ic_meta->ce_prq ?
		 iset->ic_meta->ce_prq->cl_prq->CE_NAME :
		 "(nil)" );
      else
	 printf( "edge: (nil)\n" );

      if( iset->ic_dfa )
	 printf( "dfa: %s\n", iset->ic_dfa->dl_meta->CE_NAME );
      else
	 printf( "dfa: (nil)\n" );

      printf( "sdr: %04x\n", iset->ic_setdirroot );
      _dump_dfa_stack(iset->ic_dfastack.df_set, &iset->ic_dfastack);

      printf( "dmax: %d\n", iset->ic_dmax );
      printf( "name: %s\n", iset->ic_name );
      printf( "dir:  %s\n", iset->ic_dir?iset->ic_dir:"(nil)" );

      printf( "parent: " );
      if( iset->ic_parent )
	if( iset->ic_parent->ic_meta )
	   printf( "%s --> %s\n",
	           iset->ic_parent->ic_meta->CE_NAME,
		   iset->ic_parent->ic_meta->ce_prq ?
		   iset->ic_parent->ic_meta->ce_prq->cl_prq->CE_NAME :
		   "(nil)" );
	else
	   printf( "(nil)\n" );
      else
	 printf( "(nil)\n" );
   }
   printf( "==================================\n" );
}
#endif
