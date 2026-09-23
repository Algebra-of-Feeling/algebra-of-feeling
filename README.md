# Algebra of Feeling — verification core

Verification core for the *Algebra of Feeling* preprint (v0.3.2): the Lean 4
formalisation, the C++23 realisation, and the Sounio comparator that checks
the two against each other field by field.

## Layout

- `rcda-lean/` — the `RCDA` Lean 4 package (toolchain `leanprover/lean4:v4.33.0`,
  no Mathlib), with `Audit.lean` cataloguing every proved/conditional claim
  and disclosing the foundational axioms in use.
- `manuscript/` — the C++23 realisation (`cpp/`), the Sounio dump used for
  cross-language comparison (`sounio/`), supporting documentation (`docs/`),
  and the execution logs (`logs/`) for v0.3.2.

## Reproducing the verification

Lean core (proofs, axiom audit, source-admission gate with a negative
control):

```sh
cd rcda-lean
sh scripts/verify.sh
```

C++23 realisation (Cayley–Dickson product, Bloch binding, decoherence,
zero-divisor census):

```sh
cd manuscript/cpp
xcrun clang++ -std=c++23 -Wall -Wextra -Wpedantic -Werror cd_bloch.cpp -o cd_bloch && ./cd_bloch
xcrun clang++ -std=c++23 -Wall -Wextra -Wpedantic -Werror decoherence.cpp -o decoherence && ./decoherence
xcrun clang++ -std=c++23 -Wall -Wextra -Wpedantic -Werror levels3.cpp -o levels3 && ./levels3
```

C++23/Sounio comparator (needs a Sounio checkout with a working `souc`):

```sh
souc run manuscript/sounio/dump_sounio_v032.sio > sounio_dump.tsv
xcrun clang++ -std=c++23 -O2 manuscript/cpp/compare_cpp_sounio.cpp -o compare_cpp_sounio
./compare_cpp_sounio manuscript/logs/cpp_dump.tsv sounio_dump.tsv
```

## Language policy (ADR009)

Calculation is written in C++23, F#, F\*, Futhark, or Koka. Theorems are
formalised and machine-checked in Lean 4. Python and Rust are not used
anywhere in this repository, including for spot checks.
