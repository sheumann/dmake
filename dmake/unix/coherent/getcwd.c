#if __STDC__
char *getcwd(char *buffer, int length)
#else
char *getcwd (buffer, length)
char *buffer;
int length;
#endif
{
  extern char *getwd();

  char *dir;
  dir = getwd();
  if (dir)
  {
    strncpy(buffer,dir,length);
    return buffer;
  }
  else
  {
    *buffer = 0;
    return (char *) 0;
  }
}

