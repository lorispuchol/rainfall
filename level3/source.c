void v(void)

{
  char local_20c [520];
  
  fgets(local_20c, 512, stdin);
  printf(local_20c);
  if (m == 64) { // m is a global variable
    fwrite("Wait what?!\n", 1, 12, stdout);
    system("/bin/sh");
  }
  return;
}

void main(void)

{
  v();
  return;
}