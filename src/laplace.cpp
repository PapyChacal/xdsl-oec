#include <cmath>

// define the domain size and the halo width
int64_t domain_size = 64;
int64_t domain_height = 60;
int64_t halo_width = 4;

typedef double ElementType;

#include "cuda_util.h"

extern "C" {
  void laplace(LLVMMemref3DT in, LLVMMemref3DT out);
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

  const std::array<int64_t, 3> sizes3D = { domain_size + 2 * halo_width,
                                         domain_size + 2 * halo_width,
                                         domain_height + 2 * halo_width };


  // allocate the storage
  Storage3D in = allocateStorage(sizes3D);
  Storage3D out = allocateStorage(sizes3D);
  fillMath(1.1, 2.0, 1.5, 2.8, 2.0, 4.1, in, domain_size, domain_height);
  initValue(out, 0.0, domain_size, domain_height);

  toDevice(in);
  toDevice(out);

  TIMER_START();
  // computing the reference version
  for(int i = 0; i < 8192; i++)
    laplace(LLVMMemref3D(in), LLVMMemref3D(out));
  TIMER_STOP();

  toHost(in);
  toHost(out);

  // free the storage
  freeStorage(in);
  freeStorage(out);

  return 0;
}
