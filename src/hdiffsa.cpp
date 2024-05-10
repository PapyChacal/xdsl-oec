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
void hdiffsa(LLVMMemref3DT, LLVMMemref3DT, LLVMMemref3DT, LLVMMemref1DT, LLVMMemref1DT);
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
  const int64_t size1D = domain_size + 2*halo_width;

  // allocate the storage
  Storage3D arg0  = allocateStorage(sizes3D);
  Storage3D arg1  = allocateStorage(sizes3D);
  Storage3D arg2  = allocateStorage(sizes3D);
  Storage1D arg3  = allocateStorage(size1D);
  Storage1D arg4  = allocateStorage(size1D);


  fillMath(1.0, 3.3, 1.5, 1.5, 2.0, 4.0, arg0, domain_size, domain_height);
  fillMath(1.1, 2.0, 1.5, 2.8, 2.0, 4.1, arg1, domain_size, domain_height);
  initValue(arg2, -1.0, domain_size, domain_height);
  fillMath(8.0, 9.4, 1.5, 1.7, 2.0, 3.5, arg3, domain_size, domain_height);
  fillMath(1.1, 2.0, 1.5, 2.8, 2.0, 4.1, arg4, domain_size, domain_height);


  toDevice(arg0, arg1, arg2, arg3, arg4);


  TIMER_START();

  for(int i = 0; i < 512; i++)
    hdiffsa(LLVMMemref3D(arg0), LLVMMemref3D(arg1), LLVMMemref3D(arg2), LLVMMemref1D(arg3), LLVMMemref1D(arg4));

  TIMER_STOP();

  toHost(arg0, arg1, arg2, arg3, arg4);

  // free the storage
  freeStorage(arg0);
  freeStorage(arg1);
  freeStorage(arg2);
  freeStorage(arg3);
  freeStorage(arg4);

  return 0;
}
