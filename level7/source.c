void m(void *param_1,int param_2,char *param_3,int param_4,int param_5)

{
  time_t tVar1;
  
  tVar1 = time((time_t *)0x0);
  printf("%s - %d\n",c,tVar1);
  return;
}


undefined4 main(int argc, char *argv[])

{
  undifined4 *var1;
  undifined4 *var2;

  FILE *__stream;
  

  // allocate 8 bytes for var1
  var1 = malloc(8);
  // set first 4 bytes to 1 integer
  var1[0] = 1;
  // The second 4 bytes are set to pointers to newly allocated 8 byte
  var1[1] = malloc(8);

  // Same as above
  var2 = malloc(8);
  var2[0] = 2;
  var2[1] = malloc(8);

  // Bufferoverflow vulnearbility here
  // It copies argv[1] and argv[2] into 8 bytes of allocated memory
  strcpy((char *)var1[1], argv[1]);
  strcpy((char *)var2[1], argv[2]);
  
  __stream = fopen("/home/user/level8/.pass","r");
  fgets(c, 68, __stream);
  puts("~~");

  return 0;
}