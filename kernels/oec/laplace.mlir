

module {
  func @laplace(%arg0: !stencil.field<?x?x?xf64>, %arg1: !stencil.field<?x?x?xf64>) attributes {stencil.program} {
    %0 = stencil.cast %arg0 ([-4, -4, -4] : [68, 68, 68]) : (!stencil.field<?x?x?xf64>) -> (!stencil.field<72x72x72xf64>)
    %1 = stencil.cast %arg1 ([-4, -4, -4] : [68, 68, 68]) : (!stencil.field<?x?x?xf64>) -> (!stencil.field<72x72x72xf64>)
    %2 = stencil.load %0 : (!stencil.field<72x72x72xf64>) -> !stencil.temp<?x?x?xf64>
    %3 = stencil.apply (%arg11 = %2 : !stencil.temp<?x?x?xf64>) -> !stencil.temp<?x?x?xf64> {
      %4 = stencil.access %arg11 [0, 0, 0] : (!stencil.temp<?x?x?xf64>) -> f64
      %5 = stencil.access %arg11 [-1, 0, 0] : (!stencil.temp<?x?x?xf64>) -> f64
      %6 = stencil.access %arg11 [1, 0, 0] : (!stencil.temp<?x?x?xf64>) -> f64
      %7 = stencil.access %arg11 [0, -1, 0] : (!stencil.temp<?x?x?xf64>) -> f64
      %8 = stencil.access %arg11 [0, 1, 0] : (!stencil.temp<?x?x?xf64>) -> f64
      %9 = stencil.access %arg11 [0, 0, -1] : (!stencil.temp<?x?x?xf64>) -> f64
      %10 = stencil.access %arg11 [0, 0, 1] : (!stencil.temp<?x?x?xf64>) -> f64
      %cst = constant -6.000000e+00 : f64
      %11 = mulf %cst, %4 : f64
      %12 = addf %5, %6 : f64
      %13 = addf %7, %8 : f64
      %14 = addf %9, %10 : f64
      %15 = addf %12, %13 : f64
      %16 = addf %14, %15 : f64
      %17 = addf %11, %16 : f64
      %18 = stencil.store_result %17 : (f64) -> (!stencil.result<f64>)
      stencil.return %18 : !stencil.result<f64>
    }
    stencil.store %3 to %1([0, 0, 0] : [64, 64, 64]) : !stencil.temp<?x?x?xf64> to !stencil.field<72x72x72xf64>
    return
  }
}
