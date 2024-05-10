SHELL=/bin/bash

CUDA_INCLUDE_DIR=/opt/cuda/include/

CFLAGS += -I$(CUDA_INCLUDE_DIR) -Iinclude/ -lm -lstdc++ -O3 -fPIE -march=native

# hdiffsmag does not compile on OEC's side for now, weird sqrt error.
KERNELS=laplace fastwavesuv fvtp2d_flux fvtp2d_qi fvtp2d_qj hadvuv hadvuv5th hdiffsa 
RUN_KERNELS=$(addprefix run_, $(KERNELS))
KERNELS_EXECUTABLES=$(addsuffix _oec, $(KERNELS)) $(addsuffix _xdsl, $(KERNELS))


.PHONY: run $(RUN_KERNELS) clean
.NOTPARALLEL: run_indeed

%_oec.o: kernels/oec/%.mlir
	module load oec &&\
	oec-opt $< --stencil-inlining --stencil-shape-inference --convert-stencil-to-std --cse --parallel-loop-tiling='parallel-loop-tile-sizes=128,1,1' --canonicalize --test-gpu-greedy-parallel-loop-mapping --convert-parallel-loops-to-gpu --canonicalize --cse --lower-affine --convert-scf-to-std --convert-gpu-to-nvvm --stencil-kernel-to-cubin | mlir-translate --mlir-to-llvmir | clang -c -x ir - -O3 -fPIE -o $@

%_oec: %_oec.o src/%.cpp include/util.h include/cuda_util.h
	module load oec &&\
	clang -o $@ src/$*.cpp $*_oec.o $(CFLAGS) -lcuda-runtime-wrappers

%_xdsl.o: kernels/xdsl/%.mlir python/xdsl_to_llvm.py
	module load llvm/xdsl &&\
	python python/xdsl_to_llvm.py $< | mlir-translate --mlir-to-llvmir | clang -x ir - -c -o $@

%_xdsl: %_xdsl.o src/%.cpp include/util.h include/cuda_util.h
	module load llvm/xdsl &&\
	clang -o $@ src/$*.cpp $*_xdsl.o $(CFLAGS) -lmlir_cuda_runtime


$(KERNELS): %: %_xdsl %_oec ;

clean:
	rm -f $(KERNELS_EXECUTABLES) $(addsuffix .o, $(KERNELS_EXECUTABLES))

$(RUN_KERNELS): run_%: %
	module load oec 2>> /dev/null;\
	echo "Running OEC's $*...";\
	./$*_oec 64 64;\
	module load llvm 2>> /dev/null;\
	echo "Running xDSL's $*...";\
	./$*_xdsl 64 64;\

run_indeed: $(RUN_KERNELS);

run: $(KERNELS) .WAIT run_indeed;

all: run;
