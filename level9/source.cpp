
/* N::N(int) */

void __thiscall N::N(N *this,undefined4 param_1)

{
  *(undefined ***)this = &PTR_operator+_08048848;
  *(undefined4 *)(this + 0x68) = param_1;
  return;
}


void main(int param_1,int param_2)

{
  N *this;
  undefined4 *this_00;
  
  if (param_1 < 2) {
                    /* WARNING: Subroutine does not return */
    _exit(1);
  }
  this = (N *)operator.new(0x6c);
  N::N(this,5);
  this_00 = (undefined4 *)operator.new(0x6c);
  N::N((N *)this_00,6);
  N::setAnnotation(this,*(char **)(param_2 + 4));
  (**(code **)*this_00)(this_00,this);
  return;
}


// Decompiled with Hex-Rays


//----- (080485F4) --------------------------------------------------------
int __cdecl main(int argc, const char **argv, const char **envp)
{
  N *v3; // ebx
  N *v4; // ebx
  N *v6; // [esp+1Ch] [ebp-8h]

  if ( argc <= 1 )
    _exit(1);
  v3 = (N *)operator new(0x6Cu);
  N::N(v3, 5);
  v6 = v3;
  v4 = (N *)operator new(0x6Cu);
  N::N(v4, 6);
  N::setAnnotation(v6, (char *)argv[1]);
  return (**(int (__cdecl ***)(N *, N *))v4)(v4, v6);
}