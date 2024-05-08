SHELL=/bin/bash

CUDA_INCLUDE_DIR=/opt/cuda/include/

CFLAGS += -I$(CUDA_INCLUDE_DIR) -Iinclude/ -lm -lstdc++ -O3 -fPIE -march=native

%_oec.o: kernels/oec/%.mlir
	module load oec &&\
	oec-opt $< --stencil-storage-materialization --stencil-shape-inference --convert-stencil-to-std --cse --parallel-loop-tiling='parallel-loop-tile-sizes=128,1,1' --canonicalize --test-gpu-greedy-parallel-loop-mapping --convert-parallel-loops-to-gpu --canonicalize --lower-affine --convert-scf-to-std --stencil-kernel-to-cubin | mlir-translate --mlir-to-llvmir | clang -c -x ir - -O3 -fPIE -o $@

%_oec: %_oec.o src/%.cpp include/util.h include/cuda_util.h
	module load oec &&\
	clang -o $@ src/$*.cpp $*_oec.o $(CFLAGS) -lcuda-runtime-wrappers

%_xdsl.o: kernels/xdsl/%.mlir python/xdsl_to_llvm.py
	module load llvm/xdsl &&\
	python python/xdsl_to_llvm.py $< | mlir-translate --mlir-to-llvmir | clang -x ir - -c -o $@

%_xdsl: %_xdsl.o src/%.cpp include/util.h include/cuda_util.h
	module load llvm/xdsl &&\
	clang -o $@ src/$*.cpp $*_xdsl.o $(CFLAGS) -lmlir_cuda_runtime


fastwavesuv: fastwavesuv_xdsl fastwavesuv_oec
	module load llvm/xdsl &&\
	./fastwavesuv_xdsl

	module load oec &&\
	./fastwavesuv_oec

all: fastwavesuv
