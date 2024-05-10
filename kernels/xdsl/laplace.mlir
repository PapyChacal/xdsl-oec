

builtin.module {
  func.func @laplace(%arg0: !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>, %arg1: !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>) attributes {stencil.program} {
    %2 = stencil.load %arg0 : !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64> -> !stencil.temp<?x?x?xf64>
    %3 = stencil.apply (%arg11 = %2 : !stencil.temp<?x?x?xf64>) -> (!stencil.temp<?x?x?xf64>) {
      %4 = stencil.access %arg11 [0, 0, 0] : !stencil.temp<?x?x?xf64>
      %5 = stencil.access %arg11 [-1, 0, 0] : !stencil.temp<?x?x?xf64>
      %6 = stencil.access %arg11 [1, 0, 0] : !stencil.temp<?x?x?xf64>
      %7 = stencil.access %arg11 [0, -1, 0] : !stencil.temp<?x?x?xf64>
      %8 = stencil.access %arg11 [0, 1, 0] : !stencil.temp<?x?x?xf64>
      %9 = stencil.access %arg11 [0, 0, -1] : !stencil.temp<?x?x?xf64>
      %10 = stencil.access %arg11 [0, 0, 1] : !stencil.temp<?x?x?xf64>
      %cst = arith.constant -6.000000e+00 : f64
      %11 = arith.mulf %cst, %4 : f64
      %12 = arith.addf %5, %6 : f64
      %13 = arith.addf %7, %8 : f64
      %14 = arith.addf %9, %10 : f64
      %15 = arith.addf %12, %13 : f64
      %16 = arith.addf %14, %15 : f64
      %17 = arith.addf %11, %16 : f64
      %18 = stencil.store_result %17 : !stencil.result<f64>
      stencil.return %18 : !stencil.result<f64>
    }
    stencil.store %3 to %arg1([0, 0, 0] : [64, 64, 64]) : !stencil.temp<?x?x?xf64> to !stencil.field<[-4,68]x[-4,68]x[-4,68]xf64>
    func.return
  }
}
