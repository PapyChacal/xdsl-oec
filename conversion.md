# OEC -> xDSL conversion notes

## MLIR Stndard dialect splitting:

### `func`

Replace `(^\s*)func(\s)` by `$1func.func$2`

## arith

Replace `= (constant|divf|addf|mulf|negf|subf|cmpf|select)` by `= arith.$1`

- Replace `arith.cmpf "([^"]+)"` by `arith.cmpf $1` (Comparison mnemonic is now an identifier)

## Custom syntax conversion regexes:

### `stencil.apply`

  - OEC: `stencil.access %temp [0,0] : (!stencil.temp<?x?xf64>) -> f64`
  - xDSL: `stencil.access %temp [0,0] : !stencil.temp<?x?xf64>`


Replace `stencil\.access (%[^ ]+ \[[^\]]*\]) : \((!stencil.temp<[^>]+>)\).*` by  `stencil.access $1 : $2`

### `stencil.sotre_result`

- OEC: `stencil.store_result %r : (f64) -> !stencil.result<f64>`
- xDSL: `stencil.store_result %r : !stencil.result<f64>`

Replace `stencil\.store_result (%[^ ]+) : \([^\)]+\) -> (!stencil.result.*)` by `stencil.store_result $1 : $2`

### `stencil.apply`

- OEC: `%20 = stencil.apply (%arg11 = %15 : !stencil.temp<?x?x?xf64>, %arg12 = %16 : !stencil.temp<?x?x?xf64>) -> (!stencil.temp<?x?x?xf64>) {`
- xDSL: `%20 = stencil.apply (%arg11 = %15 : !stencil.temp<?x?x?xf64>, %arg12 = %16 : !stencil.temp<?x?x?xf64>) -> (!stencil.temp<?x?x?xf64>) {`

(No parenthesis eliding on single result)

Replace `stencil\.apply (\([^/)]*\)) -> \(?([^){]*)\)? \{` by `stencil.apply $1 -> ($2) {`
