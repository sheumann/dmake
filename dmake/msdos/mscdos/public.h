/* RCS      -- $Header$
-- WARNING  -- This file is AUTOMATICALLY GENERATED DO NOT EDIT IT
--
-- SYNOPSIS -- Local functions exported to be visible by others.
--
-- DESCRIPTION
--      This file is generated by 'genpub'.  Function declarations
--      that appear in this file are extracted by 'genpub' from
--      source files.  Any function in the source file whose definition
--      appears like:
--
--          PUBLIC return_type
--          function( arg_list );
--          type_expr1 arg1;
--          ...
--
--      has its definition extracted and a line of the form:
--
--          return_type function ANSI((type_expr1,type_expr2,...));
--
--      entered into the output file.
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
--     $Log$
*/

#ifndef _DMAKE_PUBLIC_h
#define _DMAKE_PUBLIC_h

void Infer_recipe ANSI((CELLPTR, CELLPTR));
int Make_targets ANSI(());
int Exec_commands ANSI((CELLPTR));
void Print_cmnd ANSI((char *, int, int));
void Pop_dir ANSI((int));
void Append_line ANSI((char *, int, FILE *, char *, int, int));
void Stat_target ANSI((CELLPTR, int));
char * Expand ANSI((char *));
char * Apply_edit ANSI((char *, char *, char *, int, int));
void Map_esc ANSI((char *));
char* Apply_modifiers ANSI((int, char *));
char* Tokenize ANSI((char *, char *));
char * _strjoin ANSI((char *, char *, int, int));
char * _stradd ANSI((char *, char *, int));
char * _strapp ANSI((char *, char *));
char * _strdup ANSI((char *));
char * _strdup2 ANSI((char *));
char * _strpbrk ANSI((char *, char *));
char * _strspn ANSI((char *, char *));
char * _strstr ANSI((char *, char *));
char * _substr ANSI((char *, char *));
uint16 Hash ANSI((char *, uint32 *));
HASHPTR Get_name ANSI((char *, HASHPTR *, int));
HASHPTR Search_table ANSI((HASHPTR *, char *, uint16 *, uint32 *));
HASHPTR Def_macro ANSI((char *, char *, int));
CELLPTR Def_cell ANSI((char *));
LINKPTR Add_prerequisite ANSI((CELLPTR, CELLPTR, int, int));
void Clear_prerequisites ANSI((CELLPTR));
int Test_circle ANSI((CELLPTR, int));
STRINGPTR Def_recipe ANSI((char *, STRINGPTR, int, int));
t_attr Rcp_attribute ANSI((char *));
void main ANSI((int, char **));
FILE * Openfile ANSI((char *, int, int));
FILE * Closefile ANSI(());
FILE * Search_file ANSI((char *, char **));
char * Filename ANSI(());
int Nestlevel ANSI(());
void No_ram ANSI(());
int Usage ANSI((int));
int Version ANSI(());
char * Get_suffix ANSI((char *));
char * Build_path ANSI((char *, char *));
void Make_rules ANSI(());
void Create_macro_vars ANSI(());
time_t Do_stat ANSI((char *, char *, char **));
int Do_touch ANSI((char *, char *, char **));
void Void_lib_cache ANSI((char *, char *));
time_t Do_time ANSI(());
int Do_cmnd ANSI((char *, int, int, CELLPTR, int, int, int));
char ** Pack_argv ANSI((int, int, char *));
char * Read_env_string ANSI((char *));
int Write_env_string ANSI((char *, char *));
void ReadEnvironment ANSI(());
void Catch_signals ANSI((void (*)()));
void Clear_signals ANSI(());
void Prolog ANSI((int, char* []));
void Epilog ANSI((int));
char * Get_current_dir ANSI(());
int Set_dir ANSI((char*));
char Get_switch_char ANSI(());
FILE* Get_temp ANSI((char **, char *, int));
FILE * Start_temp ANSI((char *, CELLPTR, char **));
void Open_temp_error ANSI((char *, char *));
void Link_temp ANSI((CELLPTR, FILE *, char *));
void Close_temp ANSI((CELLPTR, FILE *));
void Unlink_temp_files ANSI((CELLPTR));
void Handle_result ANSI((int, int, int, CELLPTR));
void Update_time_stamp ANSI((CELLPTR));
int Remove_file ANSI((char *));
void Parse ANSI((FILE *));
int Get_line ANSI((char *, FILE *));
char * Do_comment ANSI((char *, char **, int));
char * Get_token ANSI((TKSTRPTR, char *, int));
void Quit ANSI(());
void Read_state ANSI(());
void Write_state ANSI(());
int Check_state ANSI((CELLPTR, STRINGPTR *, int));
char* basename ANSI((char *));
void Dump ANSI(());
void Dump_recipe ANSI((STRINGPTR));
int Parse_macro ANSI((char *, int));
int Macro_op ANSI((char *));
int Parse_rule_def ANSI((int *));
int Rule_op ANSI((char *));
void Add_recipe_to_list ANSI((char *, int, int));
void Bind_rules_to_targets ANSI((int));
int Set_group_attributes ANSI((char *));
DFALINKPTR Match_dfa ANSI((char *));
void Check_circle_dfa ANSI(());
void Add_nfa ANSI((char *));
char * Exec_function ANSI((char *));
int If_root_path ANSI((char *));
int runargv ANSI((CELLPTR, int, int, int, int, char *));
void Clean_up_processes ANSI(());
int Wait_for_child ANSI((int, int));
time_t seek_arch ANSI((char*, char*));
int touch_arch ANSI((char*, char*));
int _chdir ANSI((char *));
void Remove_prq ANSI((CELLPTR));
void Hook_std_writes ANSI((char *));

#endif
