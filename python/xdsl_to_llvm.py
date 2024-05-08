XDSL_PIPELINE = """\
stencil-storage-materialization,\
stencil-shape-inference,\
convert-stencil-to-ll-mlir,\
canonicalize,\
scf-parallel-loop-tiling{\
parallel-loop-tile-sizes=32,4,8\
},\
printf-to-llvm,\
canonicalize,\
memref-to-gpu,\
gpu-map-parallel-loops\
"""
MLIR_PIPELINE = """\
canonicalize,
convert-parallel-loops-to-gpu,\
lower-affine,\
canonicalize,\
cse,\
fold-memref-alias-ops,\
gpu-launch-sink-index-computations,\
gpu-kernel-outlining,\
canonicalize{region-simplify},\
cse,\
fold-memref-alias-ops,\
expand-strided-metadata,\
lower-affine,\
canonicalize,\
cse,\
func.func(gpu-async-region),\
canonicalize,\
cse,\
convert-arith-to-llvm{index-bitwidth=64},\
convert-scf-to-cf,\
convert-cf-to-llvm{index-bitwidth=64},\
canonicalize,\
cse,\
convert-func-to-llvm{use-bare-ptr-memref-call-conv},\
nvvm-attach-target{O=3 ftz fast chip=sm_$(nvidia-smi --query-gpu=compute_cap --format=csv,noheader | sed -e 's/\.//g')},\
gpu.module(convert-gpu-to-nvvm,canonicalize,cse),\
gpu-to-llvm,\
gpu-module-to-binary,\
canonicalize,\
cse\
"""

from contextlib import redirect_stdout
from io import StringIO
from logging import warning
import subprocess
from xdsl.xdsl_opt_main import xDSLOptMain
import sys

if __name__ == "__main__":
    warning(f'"{XDSL_PIPELINE}"')
    xdsl_args = ["-p", XDSL_PIPELINE, *sys.argv[1:]]
    warning(f"> xdsl-opt {" ".join(f'"{a}"' for a in xdsl_args)}")
    xdsl = xDSLOptMain(args=xdsl_args)

    xdsl_out = StringIO()
    with redirect_stdout(xdsl_out):
        xdsl.run()
        assert xdsl_out.seek(0) == 0
    xdsl_out = xdsl_out.read()
    mlir_cmd = f'mlir-opt -p "builtin.module({MLIR_PIPELINE})"'
    warning(mlir_cmd)
    mlir = subprocess.Popen(
        mlir_cmd,
        shell=True,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
    )
    out, err = mlir.communicate(input=xdsl_out.encode())
    print(out.decode())
