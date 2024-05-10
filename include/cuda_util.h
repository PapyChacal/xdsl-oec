
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
    int64_t byte_size = (size)*sizeof(ElementType);
    auto stream = mgpuStreamCreate();
    ElementType* dptr = reinterpret_cast<ElementType*>(mgpuMemAlloc(byte_size, stream));
    mgpuMemcpy(dptr, hptr, size, stream);
    mgpuStreamSynchronize(stream);
    mgpuStreamDestroy(stream);
    hostStorage.allocatedPtr = dptr;
    hostStorage.alignedPtr = &hostStorage.allocatedPtr[0];

    toDevice(hostStorages...);
}

void toHost(){};

template<typename StorageH, typename...StorageT>
void toHost(StorageH& hostStorage, StorageT&...hostStorages){
    ElementType* dptr = hostStorage.allocatedPtr;
    int64_t size = hostStorage.size();
    ElementType* hptr = new ElementType[size];
    auto stream = mgpuStreamCreate();
    mgpuMemcpy(hptr, dptr, size, stream);
    hostStorage.allocatedPtr = hptr;
    hostStorage.alignedPtr = &hostStorage.allocatedPtr[0];
    mgpuMemFree(dptr, stream);
    mgpuStreamSynchronize(stream);
    mgpuStreamDestroy(stream);

    toHost(hostStorages...);
}