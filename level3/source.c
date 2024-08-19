void v(void)

{
  char local_20c [520];
  
  fgets(local_20c, 512, stdin);
  printf(local_20c);
  if (m == 0x40) { // m is a global variable supposed equal to 0 if not init ; 0x40 = 64 in decimal
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