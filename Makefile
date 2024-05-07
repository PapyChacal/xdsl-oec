SHELL=/bin/bash

%_oec.o: kernels/oec/%.mlir
	module load oec &&\
	oec-opt $< --stencil-storage-materialization --stencil-shape-inference --convert-stencil-to-std --cse --parallel-loop-tiling='parallel-loop-tile-sizes=128,1,1' --canonicalize --test-gpu-greedy-parallel-loop-mapping --convert-parallel-loops-to-gpu --canonicalize --lower-affine --convert-scf-to-std --stencil-kernel-to-cubin | mlir-translate --mlir-to-llvmir | clang -c -x ir - -O3 -fPIE -o $@

%_oec: %_oec.o src/%.cpp
	module load oec &&\
	clang -o $@ src/$*.cpp $*_oec.o -Iinclude/ -lm -lstdc++ -lcuda-runtime-wrappers

%_xdsl.o: kernels/xdsl/%.mlir
	module load llvm/xdsl &&\
	python python/xdsl_to_llvm.py $< | mlir-translate --mlir-to-llvmir | clang -x ir - -c -o $@

%_xdsl: %_xdsl.o src/%.cpp
	module load llvm/xdsl &&\
	clang -o $@ src/$*.cpp $*_xdsl.o -Iinclude/ -lm -lstdc++

all: fastwavesuv_xdsl fastwavesuv_oec

fastwavesuv: fastwavesuv_xdsl fastwavesuv_oec
	module load xdsl &&\
	time ./fastwavesuv_xdsl

	module load oec &&\
	time ./fastwavesuv_oec

