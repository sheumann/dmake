char *
memcpy(t, f, n)
register char *t, *f;
register n;
{
   register char *p = t;

   while( --n >= 0 ) *t++ = *f++;

   return (p);
}
