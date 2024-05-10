# xDSL - Open Earth Compiler comparison

This repository contains the 10 kernels used in [the original Open Earth Compiler (OEC) paper](https://dl.acm.org/doi/abs/10.1145/3469030), with an additional trivial laplace kernel, to have a simple kernel to compare on too.

[xDSL](https://xdsl.dev/) [reimplements the stencil dialect with a few variations and additions](https://dl.acm.org/doi/10.1145/3620666.3651344), but also missing features at the time of writing.

This repository intends to provide comparative benchmarking between xDSL's reimplementation and the original OEC, the latter being more polished at the time of writing, hoping to bring the reimplementation on par or better.

## How to

It's very rough at this point.

There is some kind of version hell going on: OEC is built against a specific, years-old MLIR commit, and xDSL expect a more recent specific MLIR commit availabe. I'm used to work with [Lmod](https://lmod.readthedocs.io/en/latest/) so I used it to solve this.

Thus, the benchmarking assumes:
- both LLVM project versions are built against CUDA.
- `module load oec` makes the OEC and corresponding `clang` version available in the env and ready to use.
- `module load llvm` makes xDSL's expected MLIR and clang version available in the env and ready to use.
- CUDA is available, with its main header hardcoded at `/opt/cuda/include` for now.


Right now, a simple Makefile is provided. `make run -j N` builds all kernels parallely and run them sequentially printing basic overall runtime of each compilation flow.

## Where to

- `kernels/oec/` and `kernels/xdsl/` contain the OEC and xDSL version resp. of each kernel.

  In case you are interested in comparing another kernel from OEC, some OEC->xDSL translation comments are given in [conversion.md](conversion.md); those are best effort and probably not very more complete than the rest of this repo yet, please tell me if you missed something, or add it directly!

- `python/` contain some python scripts for things I preferred to do this way than in Makefile's shell commands; for now just the xDSL -> LLVMIR lowering driver.

- `src/` contain some cpp files for a simple main function to time each kernel. Probably will move around at some point as it's 100% boilerplate.

- `include/` contains utilites, like defining a proper nD memref descriptor in Cpp, some initialization and boilerplate in `util.h`, barely adapted from [OEC's benchmarks](https://github.com/spcl/open-earth-benchmarks). TODO: licensing chores?
  
  `cuda-util.h` is new and wraps some MLIR-CUDA wrappers and boilerplate.