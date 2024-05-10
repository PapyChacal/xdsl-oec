#include <cmath>

// define the domain size and the halo width
int64_t domain_size = 64;
int64_t domain_height = 60;
int64_t halo_width = 4;

typedef double ElementType;

#define p1 ElementType(0.583333)  // 7/12
#define p2 ElementType(0.083333)  // 1/12

#include "cuda_util.h"

extern "C" {
void fvtp2d_qj(LLVMMemref3DT, LLVMMemref3DT, LLVMMemref3DT, LLVMMemref3DT, LLVMMemref3DT, LLVMMemref3DT, LLVMMemref3DT, LLVMMemref3DT, LLVMMemref3DT);
}

// program times the execution of the linked program and times the result
int main(int argc, char **argv) {

  if(argc == 3) {
      domain_size = atoi(argv[1]);
      domain_height = atoi(argv[2]);
  } else if (argc == 1) {
  } else {
      std::cout << "Either provide the domain size and domain height like this \"./kernel 128 60\" or do not provide any arguments at all in which case the program is ran with domain size 64 and domain heigh 60" << std::endl;
      exit(1);
  }

  const std::array<int64_t, 3> sizes3D = { domain_size + 2*halo_width,
                                           domain_size + 2*halo_width,
                                           domain_height + 2*halo_width };

  // allocate the storage
  Storage3D arg0  = allocateStorage(sizes3D);
  Storage3D arg1  = allocateStorage(sizes3D);
  Storage3D arg2  = allocateStorage(sizes3D);
  Storage3D arg3  = allocateStorage(sizes3D);
  Storage3D arg4  = allocateStorage(sizes3D);
  Storage3D arg5  = allocateStorage(sizes3D);
  Storage3D arg6  = allocateStorage(sizes3D);
  Storage3D arg7  = allocateStorage(sizes3D);
  Storage3D arg8  = allocateStorage(sizes3D);


  fillMath(1.0, 3.3, 1.5, 1.5, 2.0, 4.0, arg0, domain_size, domain_height);
  fillMath(1.1, 2.0, 1.5, 2.8, 2.0, 4.1, arg1, domain_size, domain_height);
  fillMath(1.1, 2.0, 1.5, 2.8, 2.0, 4.1, arg2, domain_size, domain_height);
  fillMath(8.0, 9.4, 1.5, 1.7, 2.0, 3.5, arg3, domain_size, domain_height);
  fillMath(8.0, 9.4, 1.5, 1.7, 2.0, 3.5, arg4, domain_size, domain_height);
  fillMath(5.0, 8.0, 1.5, 7.1, 2.0, 4.3, arg5, domain_size, domain_height);
  fillMath(5.0, 8.0, 1.5, 7.1, 2.0, 4.3, arg6, domain_size, domain_height);

  initValue(arg7, -1.0, domain_size, domain_height);
  initValue(arg8, -1.0, domain_size, domain_height);

  toDevice(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);


  TIMER_START();

  for(int i = 0; i < 512; i++)
    fvtp2d_qj(LLVMMemref3D(arg0), LLVMMemref3D(arg1), LLVMMemref3D(arg2), LLVMMemref3D(arg3), LLVMMemref3D(arg4), LLVMMemref3D(arg5), LLVMMemref3D(arg6), LLVMMemref3D(arg7), LLVMMemref3D(arg8));

  TIMER_STOP();

  toHost(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);

  // free the storage
  freeStorage(arg0);
  freeStorage(arg1);
  freeStorage(arg2);
  freeStorage(arg3);
  freeStorage(arg4);
  freeStorage(arg5);
  freeStorage(arg6);
  freeStorage(arg7);
  freeStorage(arg8);

  return 0;
}
