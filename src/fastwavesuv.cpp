#include <cmath>
#include <chrono>

// define the domain size and the halo width
int64_t domain_size = 64;
int64_t domain_height = 60;
int64_t halo_width = 4;

typedef double ElementType;

#include "cuda_util.h"

#define Expand1dT ElementType*, ElementType*, int64_t, int64_t, int64_t
#define Expand2dT ElementType*, ElementType*, int64_t, int64_t, int64_t, int64_t, int64_t
#define Expand3dT ElementType*, ElementType*, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t
#define Expand1d(X) (X.allocatedPtr), (X.alignedPtr), (X.offset), (X.sizes[0]), (X.strides[0])
#define Expand2d(X) (X.allocatedPtr), (X.alignedPtr), (X.offset), (X.sizes[0]), (X.sizes[1]), (X.strides[0]), (X.strides[1])
#define Expand3d(X) (X.allocatedPtr), (X.alignedPtr), (X.offset), (X.sizes[0]), (X.sizes[1]), (X.sizes[2]), (X.strides[0]), (X.strides[1]), (X.strides[2])

extern "C"{
void fastwavesuv(Expand3dT, Expand3dT, Expand3dT, Expand3dT, Expand3dT, Expand3dT, Expand3dT, Expand3dT, Expand3dT, Expand3dT, Expand1dT);
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

  const int64_t size1D = domain_size + 2 * halo_width;
  const std::array<int64_t, 3> sizes3D = {domain_size + 2 * halo_width,
                                      domain_size + 2 * halo_width,
                                      domain_height + 2 * halo_width};

  // allocate the storage
  Storage3D uin = allocateStorage(sizes3D);
  Storage3D utens = allocateStorage(sizes3D);
  Storage3D vin = allocateStorage(sizes3D);
  Storage3D vtens = allocateStorage(sizes3D);
  Storage3D wgtfac = allocateStorage(sizes3D);
  Storage3D ppuv = allocateStorage(sizes3D);
  Storage3D hhl = allocateStorage(sizes3D);
  Storage3D rho = allocateStorage(sizes3D);
  Storage3D uout = allocateStorage(sizes3D);
  Storage3D vout = allocateStorage(sizes3D);
  Storage1D fx = allocateStorage(size1D);

  fillMath(1.0, 3.3, 1.5, 1.5, 2.0, 4.0, uin, domain_size, domain_height);
  fillMath(1.1, 2.0, 1.5, 2.8, 2.0, 4.1, utens, domain_size, domain_height);
  fillMath(1.1, 2.0, 1.5, 2.8, 2.0, 4.1, vin, domain_size, domain_height);
  fillMath(8.0, 9.4, 1.5, 1.7, 2.0, 3.5, vtens, domain_size, domain_height);
  fillMath(8.0, 9.4, 1.5, 1.7, 2.0, 3.5, ppuv, domain_size, domain_height);
  fillMath(5.0, 8.0, 1.5, 7.1, 2.0, 4.3, wgtfac, domain_size, domain_height);
  fillMath(5.0, 8.0, 1.5, 7.1, 2.0, 4.3, hhl, domain_size, domain_height);
  fillMath(3.2, 7.0, 2.5, 6.1, 3.0, 2.3, rho, domain_size, domain_height);
  fillMath(4.5, 5.0, 2.5, 2.1, 3.0, 2.3, fx, domain_size, domain_height);

  initValue(uout, 0.0, domain_size, domain_height);
  initValue(vout, 0.0, domain_size, domain_height);

  toDevice(uout);
  toDevice(vout);
  toDevice(uin);
  toDevice(vin);
  toDevice(utens);
  toDevice(vtens);
  toDevice(wgtfac);
  toDevice(ppuv);
  toDevice(hhl);
  toDevice(rho);
  toDevice(fx);

  auto start = std::chrono::high_resolution_clock::now();

  for(int i = 0; i < 512; i++)
    fastwavesuv(Expand3d(uout), Expand3d(vout), Expand3d(uin), Expand3d(vin), Expand3d(utens), Expand3d(vtens), Expand3d(wgtfac), Expand3d(ppuv), Expand3d(hhl), Expand3d(rho), Expand1d(fx));

  auto end = std::chrono::high_resolution_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  std::cout << time.count() << "ms" << std::endl;


  toHost(uout);
  toHost(vout);
  toHost(uin);
  toHost(vin);
  toHost(utens);
  toHost(vtens);
  toHost(wgtfac);
  toHost(ppuv);
  toHost(hhl);
  toHost(rho);
  toHost(fx);

  // free the storage
  freeStorage(uin);
  freeStorage(utens);
  freeStorage(vin);
  freeStorage(vtens);
  freeStorage(wgtfac);
  freeStorage(ppuv);
  freeStorage(hhl);
  freeStorage(rho);
  freeStorage(uout);
  freeStorage(vout);
  freeStorage(fx);

  return 0;
}
