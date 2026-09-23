# Short theorem dependency map

```text
Foundation: AbelianGroup → NonAssocRing → Ring
    │
    ├─ Associator: zero associator ↔ equality of bracketings
    │    └─ Sector: associative sector ↔ vanishing associator on the sector
    │
    ├─ Embedding: unique division → NoZeroDivisors
    │    + ProductEmbedding (injective, preserves zero/product)
    │    └─ embedded_sector_no_annihilation
    │         └─ nontrivial_annihilation_requires_sector_escape [P]
    │              + proved Double.canonicalEmbedding
    │              + supplied DivisionAlgebra / OctonionLike
    │              └─ Double.division_sector_escape [C]
    │                   + supplied RCDAModel annihilating pair
    │                   └─ RCDAModel.witness_escapes [C]
    │
    ├─ CayleyDickson + additive unital conjugation
    │    └─ Concrete: Int → CD1 → CD2 → CD3 → CD4
    │         ├─ concrete_octonion_associator [P]
    │         └─ concrete_annihilation [P]
    │              + annihilation_obstructs_left_inverse
    │              └─ concrete_no_universal_recovery [P]
    │
    ├─ Curvature: distributivity + abelian-group identities
    │    ├─ AddEnd M: associative ring of additive operators
    │    └─ curvature_deformation [P]
    │         + CentralAction module/centrality laws
    │         └─ curvature_scaled_deformation [P]
    │
    └─ Bundle
         ├─ PathGroupoid + Transport action laws
         │    └─ undo / redo
         │         └─ memory_iff_loop_displacement [P]
         └─ AssociatorCoupling model law + associative composition
              + curvature_zero_deformation
              └─ associative_preserves_reference [C]

Examples: F₂ and F₂ × F₂ → associativity_does_not_decide_annihilation [P]
Examples: CD3 witness + zero coupling → nonAssociative_with_zero_correction [P]

Audit: compiled declarations → transitive axiom audit + [P]/[C] catalogue
Scientific interpretations [H]: text only; no arrows into mathematical proofs
```

File imports are acyclic. `RCDA.lean` imports the complete proof library;
`Audit.lean` imports that library and Lean's bundled inspection tools.

**Open realization obligations:** construct a suitable scalar field, construct
and certify the intended octonion division algebra and its dimension, supply an
inhabitant of the complete `RCDAModel`, and connect algebraic transport to smooth
geometry and empirical measurements. The concrete integer-coordinate witnesses
do not discharge these distinct obligations.

## Sounio execution map

```text
Sounio CD basis-sign recurrence → bounded-integer CD product
    ├─ embedding: all 64 basis pairs + sector membership
    ├─ associator: 64 quaternion triples + octonion value 2e7
    └─ annihilating pair: zero product + nonzero factors + escape
Sounio 2x2 operators → scaled curvature witness + relative-loop memory
Sounio domain guards / native assertions → positive run + 6 rejection cases
Lean modeled update loops → sign/XOR convolution → Cayley–Dickson product
    └─ universal equality for arbitrary integer coefficients, dimension 16
Lean input guard → term bounds → every partial sum fits i64
Sounio source ↔ explicit hand-modeled loop (no verified translation/compiler theorem)
```

The Sounio executable is the runtime claim authority; shell only orchestrates
compilation and captures exit codes and receipts.

The new bridge lives in `RCDA/SounioBridge.lean`, `RCDA/LoopRefinement.lean`
and `RCDA/IntegerBounds.lean`. See `PROOF_BRIDGE.md` for its exact statement,
source correspondence and remaining obligations.

## Executable differential evidence (outside the theorem DAG)

```text
Sounio kernel + differential/runner.sio
    -> default Madaros check/compile -> native input/output transcript

Cayley–Dickson pair specification
    -> differential/reference.cpp + Boost cpp_int
    -> GCC C++23 / Clang C++23 + ASan/UBSan -> exact reference outputs

Independent corpus schedule (Python, no algebra evaluator)
    -> validates all emitted input records
Native transcript + both C++ outputs
    -> finite agreement receipt + deliberate-disagreement replay
```

These arrows denote execution and comparison, not proof dependencies. None of
these executables is imported by Lean, and no runtime result becomes an axiom.

The isolated sign-mutation counterexample is also replayed by
`differential/replay_lean.py`. It generates two concrete propositions and proves
them with kernel-checked `decide`. `lean-replay-receipt.json` records the separate
axiom footprint (`propext`). External outputs supply proposition data, not axioms.

## Typed arithmetic translation [P]

```text
decode_add / decode_sub / decode_conj / decode_join1..4 ─┐
SounioBridge.loop_refines ──────────────────────────────┤
                          induction on typed Expr ─────┴→ translation_correct
```

`Audit.lean` checks [P]/[C] proofs separately from [V] evidence metadata.
`verify_ledger.py` checks execution receipts; its success is not a proof premise.

## Typed relational composition

```text
System: Base / F b / E b / R b + explicit Protocol
    ├─ frozen contexts → frozen_protocol_reduces [P]
    ├─ fixed RCDA product compatibility → fixed_product_reduces [C]
    ├─ product compatibility + no-zero-divisor embedding
    │    → contextual_annihilation_escapes [C]
    ├─ shifted addition → frozen associative AND operational defect = 1 [P]
    └─ ordinary addition → encoding changes AND every protocol defect = 0 [P]
Functions compose associatively [P]
    + faithful product-preserving realization → associative code product [P]
```

`RelationalComposition` imports `Concrete`; no empirical text is imported. The
new contextual interface is not automatically a non-associative ring, an action,
or a dynamical/geometric realization. Compatibility assumptions are explicit.
