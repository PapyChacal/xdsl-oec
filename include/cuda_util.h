
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

void toDevice(){};

template<typename StorageH, typename...StorageT>
void toDevice(StorageH& hostStorage, StorageT&...hostStorages){
    void* hptr = hostStorage.allocatedPtr;
    int64_t size = hostStorage.size();
    int64_t byte_size = (size + (4 - halo_width))*sizeof(ElementType);
    auto stream = mgpuStreamCreate();
    void* dptr = mgpuMemAlloc(byte_size, stream);
    mgpuMemcpy(dptr, hptr, size, stream);
    mgpuStreamSynchronize(stream);
    mgpuStreamDestroy(stream);
    hostStorage.allocatedPtr = reinterpret_cast<ElementType*>(dptr);
    hostStorage.alignedPtr = &hostStorage.allocatedPtr[(4 - halo_width)];

    toDevice(hostStorages...);
}

void toHost(){};

template<typename StorageH, typename...StorageT>
void toHost(StorageH& hostStorage, StorageT&...hostStorages){
    ElementType* dptr = hostStorage.allocatedPtr;
    int64_t size = hostStorage.size();
    void* hptr = new ElementType[size + (4 - halo_width)];
    auto stream = mgpuStreamCreate();
    mgpuMemcpy(hptr, dptr, size, stream);
    mgpuStreamSynchronize(stream);
    mgpuStreamDestroy(stream);
    hostStorage.allocatedPtr = reinterpret_cast<ElementType*>(hptr);
    hostStorage.alignedPtr = &hostStorage.allocatedPtr[(4 - halo_width)];
    mgpuMemFree(dptr, nullptr);

    toHost(hostStorages...);
}