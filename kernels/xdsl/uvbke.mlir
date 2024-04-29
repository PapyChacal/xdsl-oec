

module {
  func.func @uvbke(%arg0: !stencil.field<?x?x?xf64>, %arg1: !stencil.field<?x?x?xf64>, %arg2: !stencil.field<?x?x?xf64>, %arg3: !stencil.field<?x?x?xf64>, %arg4: !stencil.field<?x?x?xf64>, %arg5: !stencil.field<?x?x?xf64>) attributes {stencil.program} {
    %0 = stencil.cast %arg0 : !stencil.field<?x?x?xf64> -> !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>
    %1 = stencil.cast %arg1 : !stencil.field<?x?x?xf64> -> !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>
    %2 = stencil.cast %arg2 : !stencil.field<?x?x?xf64> -> !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>
    %3 = stencil.cast %arg3 : !stencil.field<?x?x?xf64> -> !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>
    %4 = stencil.cast %arg4 : !stencil.field<?x?x?xf64> -> !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>
    %5 = stencil.cast %arg5 : !stencil.field<?x?x?xf64> -> !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>
    %6 = stencil.load %0 : !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64> -> !stencil.temp<?x?x?xf64>
    %7 = stencil.load %1 : !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64> -> !stencil.temp<?x?x?xf64>
    %8 = stencil.load %2 : !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64> -> !stencil.temp<?x?x?xf64>
    %9 = stencil.load %3 : !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64> -> !stencil.temp<?x?x?xf64>
    %10 = stencil.apply (%arg6 = %6 : !stencil.temp<?x?x?xf64>, %arg7 = %7 : !stencil.temp<?x?x?xf64>, %arg8 = %8 : !stencil.temp<?x?x?xf64>, %arg9 = %9 : !stencil.temp<?x?x?xf64>) -> (!stencil.temp<?x?x?xf64>) {
      %cst = arith.constant 1.125000e+02 : f64
      %12 = stencil.access %arg7 [-1, 0, 0] : !stencil.temp<?x?x?xf64>
      %13 = stencil.access %arg7 [0, 0, 0] : !stencil.temp<?x?x?xf64>
      %14 = arith.addf %12, %13 : f64
      %15 = stencil.access %arg8 [0, 0, 0] : !stencil.temp<?x?x?xf64>
      %16 = arith.mulf %14, %15 : f64
      %17 = stencil.access %arg6 [0, -1, 0] : !stencil.temp<?x?x?xf64>
      %18 = stencil.access %arg6 [0, 0, 0] : !stencil.temp<?x?x?xf64>
      %19 = arith.addf %17, %18 : f64
      %20 = arith.subf %19, %16 : f64
      %21 = arith.mulf %cst, %20 : f64
      %22 = stencil.access %arg9 [0, 0, 0] : !stencil.temp<?x?x?xf64>
      %23 = arith.mulf %22, %21 : f64
      %24 = stencil.store_result %23 : !stencil.result<f64>
      stencil.return %24 : !stencil.result<f64>
    }
    %11 = stencil.apply (%arg6 = %6 : !stencil.temp<?x?x?xf64>, %arg7 = %7 : !stencil.temp<?x?x?xf64>, %arg8 = %8 : !stencil.temp<?x?x?xf64>, %arg9 = %9 : !stencil.temp<?x?x?xf64>) -> (!stencil.temp<?x?x?xf64>) {
      %cst = arith.constant 1.125000e+02 : f64
      %12 = stencil.access %arg6 [0, -1, 0] : !stencil.temp<?x?x?xf64>
      %13 = stencil.access %arg6 [0, 0, 0] : !stencil.temp<?x?x?xf64>
      %14 = arith.addf %12, %13 : f64
      %15 = stencil.access %arg8 [0, 0, 0] : !stencil.temp<?x?x?xf64>
      %16 = arith.mulf %14, %15 : f64
      %17 = stencil.access %arg7 [-1, 0, 0] : !stencil.temp<?x?x?xf64>
      %18 = stencil.access %arg7 [0, 0, 0] : !stencil.temp<?x?x?xf64>
      %19 = arith.addf %17, %18 : f64
      %20 = arith.subf %19, %16 : f64
      %21 = arith.mulf %cst, %20 : f64
      %22 = stencil.access %arg9 [0, 0, 0] : !stencil.temp<?x?x?xf64>
      %23 = arith.mulf %22, %21 : f64
      %24 = stencil.store_result %23 : !stencil.result<f64>
      stencil.return %24 : !stencil.result<f64>
    }
    stencil.store %10 to %4([0, 0, 0] : [64, 64, 64]) : !stencil.temp<?x?x?xf64> to !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>
    stencil.store %11 to %5([0, 0, 0] : [64, 64, 64]) : !stencil.temp<?x?x?xf64> to !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>
    return
  }
}
