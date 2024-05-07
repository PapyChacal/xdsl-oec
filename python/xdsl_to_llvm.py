XDSL_PIPELINE = """\
stencil-storage-materialization,\
stencil-shape-inference,\
convert-stencil-to-ll-mlir,\
scf-parallel-loop-tiling{\
    parallel-loop-tile-sizes=64,0\
},\
printf-to-llvm,\
canonicalize\
""".translate(
    str.maketrans("", "", " \n\t\r")
)
MLIR_PIPELINE = """\
canonicalize,\
cse,\
loop-invariant-code-motion,\
canonicalize,\
cse,\
loop-invariant-code-motion,\
cse,\
canonicalize,\
fold-memref-alias-ops,\
expand-strided-metadata,\
loop-invariant-code-motion,\
lower-affine,\
convert-scf-to-cf,\
convert-math-to-llvm,\
finalize-memref-to-llvm,\
convert-func-to-llvm{\
    use-bare-ptr-memref-call-conv\
},\
convert-func-to-llvm,\
canonicalize,\
cse\
""".translate(
    str.maketrans("", "", " \n\t\r")
)

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
