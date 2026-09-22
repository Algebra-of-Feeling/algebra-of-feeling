#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p verification
rm -f verification/receipt.json verification/negative-control.log
if [ "$(uname -s)" = Darwin ]; then
  CXX_BIN="xcrun clang++"
else
  CXX_BIN="${CXX:-c++}"
fi
$CXX_BIN -std=c++23 -Wall -Wextra -Wpedantic -Werror scripts/negative_control.cpp -o verification/negative_control
$CXX_BIN -std=c++23 -Wall -Wextra -Wpedantic -Werror scripts/verify.cpp -o verification/verify
# O controle negativo roda primeiro e precisa passar por conta própria:
# um portão nunca visto rejeitando uma admissão não corrobora nada sobre
# esse portão. verify recusa-se a emitir PASS sem este log já presente.
verification/negative_control .
exec verification/verify
