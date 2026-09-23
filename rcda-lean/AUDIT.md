# Claim audit

All [P] and [C] entries below have complete compiled proofs. Tags distinguish the
kind of premise/application, not different levels of Lean proof acceptance.

| Tag | Statement / declaration | Exact premise or boundary |
|---|---|---|
| [P] | `associator_eq_zero_iff` | Additive-group cancellation; no multiplicative associativity. |
| [P] | `associative_iff_associator_zero`, `Sector.associative_iff` | Global versus sector-restricted quantification is explicit. |
| [P] | `nonAssociative_iff_nonzero_associator` | An explicit existential witness, not a classical conversion of a negated universal. |
| [P] | `division_noZeroDivisors` | Unique left division for every nonzero left factor. Classical case split on zero. |
| [P] | `embedded_sector_no_annihilation` | Injective product map, zero preservation, and no zero divisors in its domain. |
| [P] | `nontrivial_annihilation_requires_sector_escape` | Both factors nonzero and their ordered product zero. Concludes “not both inside.” |
| [P] | `Double.embed_mul` | Derived from the displayed Cayley–Dickson formula. |
| [C] | `Double.division_sector_escape` | A division-algebra instance is supplied, not constructed. |
| [C] | `RCDAModel.witness_escapes` | Supplied octonion-like structure and a supplied annihilating pair. No full model instance is asserted. |
| [P] | `annihilation_obstructs_left_inverse` | No universal inverse for this multiplication map; no temporal irreversibility claim. |
| [P] | `concrete_annihilation`, `concrete_hasZeroDivisors` | Kernel-computed integer-coordinate pair in the fourfold doubling. |
| [P] | `concrete_octonion_associator` | Explicit nonzero associator in the threefold integer doubling. |
| [P] | `associativity_does_not_decide_annihilation` | Constructed F₂ and F₂ × F₂ examples. |
| [P] | `curvature_deformation` | Algebraic curvature formula with an arbitrary direction bracket. |
| [P] | `curvature_scaled_deformation` | Constant scalar and explicitly declared central action laws. |
| [P] | `curvature_zero_deformation` | Zero correction preserves, but need not erase, reference curvature. |
| [C] | `AssociatorCoupling.associative_preserves_reference` | Model assumption: globally zero associator implies zero correction. |
| [P] | `nonAssociative_with_zero_correction` | Concrete non-associativity plus a zero coupling; no forced nonzero transfer. |
| [P] | `Transport.memory_iff_loop_displacement` | Groupoid action and a specified initial state; no infinitesimal or smooth holonomy theorem. |
| [H] | Relational fibre as psychological ontology | Requires a measurement/interpretation model and empirical justification. |
| [H] | Annihilation as clinical rupture | Not inferred from the algebraic zero product. |
| [H] | Associator → observed curvature → memory mediation | Nonzero transfer and empirical identifiability are separate requirements. |
| [H] | Every post-rupture relationship is unrecoverably distinct | Not implied by noninvertibility of one multiplication map. |
| [H] | Scientific novelty / priority | No literature or priority theorem is established by this development. |

## Declared structures and their obligations

- `AbelianGroup`: associative, commutative addition, zero, additive inverse.
- `NonAssocRing`: distributive multiplication with a unit. No associativity law.
- `Ring`: adds multiplicative associativity.
- `DivisionAlgebra`: nonzero unit and unique left/right solvability for nonzero factors.
- `ProductEmbedding`: injectivity and preservation of zero/product.
- `Sector`: a predicate closed under zero, one, addition, negation and multiplication.
- `Conjugation`: a unary operation preserving zero, sufficient for the embedding.
- `AdditiveUnitalConjugation`: adds additive and unit preservation for ring doubling.
- `OctonionLike`: division, alternativity, non-associativity, involutive additive
  anti-multiplicative conjugation preserving the unit; dimension is not specified.
- `RCDAModel`: an explicit annihilating pair in the double of a supplied `OctonionLike`.
- `AddEnd`: a function with a proved additive law; all ring laws are derived.
- `CentralAction`: module laws plus compatibility with multiplication on both sides.
- `PathGroupoid`: paths, identities, composition, inverses and their algebraic laws.
- `Transport`: an action of that groupoid on a dependent family of fibres.
- `AssociatorCoupling`: a chosen correction with the explicit zero-associator model law.

No structure field asserts sector escape, the curvature expansion, or the memory
equivalence being proved. Scientific hypothesis text is never used as a premise.

## Audit receipt

Run `sh scripts/verify.sh` to regenerate the current receipt. It records the
Lean version, build exit status, source hashes, transitive logical axioms, and
counts. Declaration counts include generated projections, recursors and auxiliary
theorems; they must not be advertised as counts of original mathematical results.

## Sounio evidence extension

The named algebraic instances additionally run in `sounio/rcda_core.sio` under
Madaros. Its finite `[V]` assertions do not alter the universal Lean
statements or discharge the [C]/[H] obligations. The exact associator value
`concrete_octonion_associator_value` was added as a kernel-checked Lean theorem
to match the Sounio coordinate output. See `sounio/verification/results.tsv` for
positive execution and controlled rejection cases, and `sounio/README.md` for
numeric-domain and compiler-provenance limits.

## Universal algorithm bridge [P]

- `SounioBridge.multiplication_refines`: sign/XOR convolution equals fourfold
  Cayley–Dickson multiplication for arbitrary integer coordinates.
- `SounioBridge.loop_refines`: an explicit model of the source array-update loops
  computes that convolution.
- `SounioBridge.basisSign_abs`, `xor_index_bound`: sign magnitude and valid destinations.
- `IntegerBounds.guard_implies_bound`, `term_bound`: guard and scalar-product bounds.
- `SounioBridge.innerLoop_bound`, `each_update_fits_i64`: every intermediate
  accumulator prefix fits, including partially completed rows.

These are complete Lean proofs about the explicit mathematical model. The
source-to-model translation is manually specified; compilation correctness is
not proved. Neither fact changes the [C] model or [H] scientific obligations.

## [V] finite computational validation

[V] entries are listed in `EVIDENCE_LEDGER.json` and mirrored as text metadata
in `Audit.lean`. They never become axioms or theorem premises. The Lean audit
rejects [V] and [H] tags in the theorem catalogue. The older execution-ledger
checker is legacy and is not part of the current ADR009-compliant proof audit.

`V-LEAN1` records that the concrete replay was executed. The two concrete
propositions proved during that event are [P]; these are different assertions.

## [P] typed arithmetic translation

`ArithmeticFragment.translation_correct` universally relates a declared typed
arithmetic AST to a declared array-oriented AST, including explicit modeled
multiplication loops. It covers all expressions in that fragment and arbitrary
integer environments. It is not a theorem about the Sounio parser, partial i64
semantics or native compilation. See `REFINEMENT.md` for exact scope.

## Typed contextual composition extension

All names below are in `RCDA.RelationalComposition`. Nine new entries bring the
compiled catalogue to 44 [P]/[C] claims. Eight [V] metadata entries remain separate.

| Tag | Declaration | Premise/boundary |
|---|---|---|
| [P] | `frozen_protocol_reduces` | Both protocol contexts equal initial C. |
| [C] | `fixed_product_reduces` | All contextual products equal the supplied RCDA product. |
| [C] | `contextual_annihilation_escapes` | Contextual product agrees at the pair with the embedding product; lower carrier has no zero divisors. |
| [P] | `shifted_frozen_associative` | Integer shifted addition; not claimed to satisfy ring multiplication laws. |
| [P] | `associative_contexts_nonzero_operational` | Explicit protocol produces defect 1 despite frozen associativity. |
| [P] | `encoding_changes` | A concrete encoding changes after update. |
| [P] | `encoding_change_without_defect` | In that additive model, every protocol has defect zero. |
| [P] | `update_functions_associative` | Ordinary composition of state transformations. |
| [P] | `faithful_updates_force_associativity` | Faithful, product-preserving function representation. |

Two additional [H] texts concern contextual model comparison and an identifiable
observation term beyond conditioning plus ordinary intervention. The scientific
registry now contains eight texts; none is a theorem premise.

## ADR009 audit migration

The active proof-audit entry point is scripts/verify.sh, which builds the C++23
verifier. Lean checks proofs and computes catalogue counts; C++ writes the
receipt and captures hashes. Old runtime-ledger/differential orchestration is
legacy, is not executed by this entry point and is not revalidated by its PASS.
The self-contained manuscript snapshot is delivered in
../retomada-algebra-do-sentir/v0_3_1/realizacao/nucleo-lean/.
