
#include "cuda.h"
#include "util.h"

extern "C" {
    CUstream mgpuStreamCreate();
    void mgpuStreamDestroy(CUstream stream);
    void mgpuStreamSynchronize(CUstream stream);
    void mgpuMemcpy(void *dst, void *src, uint64_t sizeBytes,
                           CUstream stream);
    void *mgpuMemAlloc(uint64_t sizeBytes, CUstream /*stream*/);
    void mgpuMemFree(void *ptr, CUstream /*stream*/);
}

void toDevice(Storage1D& hostStorage){
    void* hptr = hostStorage.allocatedPtr;
    int64_t size = hostStorage.sizes[0] * sizeof(ElementType);
    int64_t byte_size = (size + (4 - halo_width))*sizeof(ElementType);
    auto stream = mgpuStreamCreate();
    void* dptr = mgpuMemAlloc(byte_size, stream);
    mgpuMemcpy(dptr, hptr, size, stream);
    mgpuStreamSynchronize(stream);
    mgpuStreamDestroy(stream);
    hostStorage.allocatedPtr = reinterpret_cast<ElementType*>(dptr);
    hostStorage.alignedPtr = &hostStorage.allocatedPtr[(32 - halo_width)];
}

void toDevice(Storage2D& hostStorage){
    ElementType* hptr = hostStorage.allocatedPtr;
    int64_t size = hostStorage.sizes[0] * hostStorage.sizes[1] * sizeof(ElementType);
    int64_t byte_size = (size + (4 - halo_width))*sizeof(ElementType);
    auto stream = mgpuStreamCreate();
    void* dptr = mgpuMemAlloc(byte_size, stream);
    mgpuMemcpy(dptr, hptr, size, stream);
    mgpuStreamSynchronize(stream);
    mgpuStreamDestroy(stream);
    hostStorage.allocatedPtr = reinterpret_cast<ElementType*>(dptr);
    hostStorage.alignedPtr = &hostStorage.allocatedPtr[(32 - halo_width)];
    delete[] hptr;
}

void toDevice(Storage3D& hostStorage){
    ElementType* hptr = hostStorage.allocatedPtr;
    int64_t size = hostStorage.sizes[0] * hostStorage.sizes[1] * hostStorage.sizes[2] * sizeof(ElementType);
    int64_t byte_size = (size + (4 - halo_width))*sizeof(ElementType);
    auto stream = mgpuStreamCreate();
    void* dptr = mgpuMemAlloc(byte_size, stream);
    mgpuMemcpy(dptr, hptr, size, stream);
    mgpuStreamSynchronize(stream);
    mgpuStreamDestroy(stream);
    hostStorage.allocatedPtr = reinterpret_cast<ElementType*>(dptr);
    hostStorage.alignedPtr = &hostStorage.allocatedPtr[(32 - halo_width)];
    delete[] hptr;
}

void toHost(Storage1D& hostStorage){
    ElementType* dptr = hostStorage.allocatedPtr;
    int64_t size = hostStorage.sizes[0] * sizeof(ElementType);
    void* hptr = new ElementType[size + (32 - halo_width)];
    auto stream = mgpuStreamCreate();
    mgpuMemcpy(hptr, dptr, size, stream);
    mgpuStreamSynchronize(stream);
    mgpuStreamDestroy(stream);
    hostStorage.allocatedPtr = reinterpret_cast<ElementType*>(hptr);
    hostStorage.alignedPtr = &hostStorage.allocatedPtr[(32 - halo_width)];
    mgpuMemFree(dptr, nullptr);
}

void toHost(Storage2D& hostStorage){
    ElementType* dptr = hostStorage.allocatedPtr;
    int64_t size = hostStorage.sizes[0] * hostStorage.sizes[1] * sizeof(ElementType);
    void* hptr = new ElementType[size + (32 - halo_width)];
    auto stream = mgpuStreamCreate();
    mgpuMemcpy(hptr, dptr, size, stream);
    mgpuStreamSynchronize(stream);
    mgpuStreamDestroy(stream);
    hostStorage.allocatedPtr = reinterpret_cast<ElementType*>(hptr);
    hostStorage.alignedPtr = &hostStorage.allocatedPtr[(32 - halo_width)];
    mgpuMemFree(dptr, nullptr);
}

void toHost(Storage3D& hostStorage){
    ElementType* dptr = hostStorage.allocatedPtr;
    int64_t size = hostStorage.sizes[0] * hostStorage.sizes[1] * hostStorage.sizes[2] * sizeof(ElementType);
    void* hptr = new ElementType[size + (32 - halo_width)];
    auto stream = mgpuStreamCreate();
    mgpuMemcpy(hptr, dptr, size, stream);
    mgpuStreamSynchronize(stream);
    mgpuStreamDestroy(stream);
    hostStorage.allocatedPtr = reinterpret_cast<ElementType*>(hptr);
    hostStorage.alignedPtr = &hostStorage.allocatedPtr[(32 - halo_width)];
    mgpuMemFree(dptr, nullptr);
}