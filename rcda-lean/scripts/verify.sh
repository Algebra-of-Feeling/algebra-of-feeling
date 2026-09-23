#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p verification
rm -f verification/receipt.json
if [ "$(uname -s)" = Darwin ]; then
  xcrun clang++ -std=c++23 -Wall -Wextra -Wpedantic -Werror scripts/verify.cpp -o verification/verify
else
  "${CXX:-c++}" -std=c++23 -Wall -Wextra -Wpedantic -Werror scripts/verify.cpp -o verification/verify
fi
exec verification/verify
