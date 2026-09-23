# RCDA / RCDB — Sounio execution and self-contained Lean proofs

A compiled algebraic foundation with explicit premises, no Mathlib, no project
axiom declarations, and no admitted proofs. Lean is pinned to **4.33.0**.
The proof modules import `Init` and local modules only; the audit additionally
imports the Lean distribution's own metaprogramming library. There are no package
dependencies (`lake-manifest.json` has an empty `packages` array).

## Sounio executable layer

The executable mathematical kernel is now in **Sounio**, checked and compiled
with the default Madaros engine. Its native program evaluates the associator,
canonical embedding, zero-divisor pair, curvature expansion, and relative-loop
memory witness. All runtime assertions execute in Sounio. See
[sounio/README.md](sounio/README.md) for the source-to-proof map, bounded-integer
contract, mutation checks, and exact compiler provenance.

Lean remains the universal mathematical proof layer. Runtime witnesses, logical
proofs and scientific interpretation have distinct receipts. The explicit model
of the sixteen-coordinate Sounio loop now has a universal refinement proof; see
[PROOF_BRIDGE.md](PROOF_BRIDGE.md). Translation from source syntax and compiler
correctness remain separate, unproved obligations.

## Independent C++23 differential reference

The additional [differential layer](differential/README.md) evaluates the pair
formula with arbitrary-precision integers under GCC and sanitized Clang. It
compares 1,310 native Sounio records, checks input transport independently, and
requires controlled discrepancies to be detected. This is finite computational
evidence, not a new Lean theorem or a replacement for Sounio semantics.

## Four evidence categories and modeled translation

The canonical ledger now distinguishes **[P]**, **[C]**, **[H]** and **[V]**.
See [EVIDENCE_LEDGER.md](EVIDENCE_LEDGER.md), its machine-readable JSON companion,
and [PROJECT_STATE.json](PROJECT_STATE.json). Validation records are metadata,
not Lean propositions. The audit rejects [V]/[H] entries in the theorem catalogue.

A first [typed arithmetic translation](REFINEMENT.md) has a universal
compositional proof between two explicitly modeled ASTs. Actual Sounio-source
parsing, partial machine semantics and native compilation remain OPEN.

## Reproduce

With the pinned Lean toolchain installed:

```sh
sh scripts/verify.sh
```

The active verifier uses C++23 under ADR009; proof acceptance and counts remain
in Lean 4. Historical runtime-ledger and differential orchestration are outside
this audit and are not revalidated by its receipt.

The default build includes `Audit.lean`. The verification script additionally
checks source boundaries, performs a fresh axiom audit, and writes receipts in
`verification/`. No network service or solver is required after Lean is installed.

## What is proved

* An associator vanishes exactly when its two bracketings agree; sector-wide
  vanishing is equivalent to associativity on that sector.
* Unique left division implies absence of zero divisors. No inverse
  reassociation is used, and multiplication is not assumed associative.
* A zero-preserving, injective product map transports that obstruction to its
  image. Thus a nonzero annihilating pair cannot have **both** factors in the
  embedded division sector. This does not assert that both factors must escape.
* For the displayed Cayley–Dickson product, `a ↦ (a, 0)` is injective and preserves
  zero and multiplication. These properties are proved from the formula.
  With additive unital conjugation, its image is a proved `Sector`, closed under
  all ring operations; the stronger octonion-like interface supplies these laws.
* The doubled carrier has a non-associative unital ring structure when the
  supplied conjugation is additive, preserves zero, and preserves one.
* The algebraic connection-difference identity and its constant-scalar version
  are proved by distributivity and abelian-group identities. An associative ring
  of additive endomorphisms is constructed, so operator composition is an actual
  realization of the coefficient algebra.
* For an action of an abstract path groupoid, two paths move a specified state
  differently exactly when the corresponding relative loop moves that state.
* An annihilating multiplication map cannot have a universal left inverse.
  This is a precise information-loss result for that map, not a temporal or
  clinical irreversibility theorem.

## Concrete computational content

`Concrete.lean` iterates the same doubling formula four times over `Int`.
It verifies, using kernel-checked `decide` rather than native evaluation:

\[
(e_1+e_{10})(e_4-e_{15})=0,
\qquad e_1+e_{10}\ne0,\quad e_4-e_{15}\ne0.
\]

Indices are zero-based; `e₀` is the unit. The convention is

\[
(a,b)(c,d)=(ac-\overline d\,b,\;da+b\overline c),
\qquad \overline{(a,b)}=(\overline a,-b).
\]

The eight-coordinate integer carrier also has a proved nonzero associator at
`e₁, e₂, e₄`. These are explicit integer-coordinate witnesses, **not** a proof
that an algebra over the real numbers is an octonion division algebra.

`Examples.lean` constructs the two-element field by finite proofs and its
coordinatewise product ring. They establish that associativity is compatible
with either presence or absence of zero divisors. It also combines the concrete
non-associativity witness with a permitted zero coupling, so non-associativity
alone cannot force a nonzero correction.

## Epistemic boundaries

**[P] Standard/proved:** a mathematical statement proved from its displayed
ordinary algebraic premises. This is not a claim that every possible model
satisfies those premises, or a bibliographic novelty classification.

**[C] Conditional on RCDA model data:** also a complete Lean proof, but its RCDA
application uses supplied model structures. `OctonionLike` specifies division,
alternativity, non-associativity and conjugation laws. It is an interface, with
no hidden existence axiom and no dimension-eight certificate. `RCDAModel`
additionally requires an explicit annihilating pair in its double. No inhabitant
of that complete model record is constructed here. The integer tower is not
silently passed off as such an inhabitant.

`AssociatorCoupling` explicitly requires vanishing correction when the
associator vanishes identically. Its corollary gives equality with reference
curvature, **not** vanishing total curvature. The zero coupling always meets the
model law. No implication from nonzero associator to nonzero correction follows.

**[H] Scientific hypothesis:** relational states as psychological fibres,
zero products as clinical rupture, measurable associator–memory mediation,
irreversible relationship loss, and scientific novelty. These appear only as
labelled text in the audit catalogue; they are not Lean axioms or theorems.

The RCDB portion is an algebraic groupoid action on dependent fibres, plus an
operator curvature polynomial. It does not construct manifolds, smooth bundles,
connections satisfying a differential Leibniz law, continuous trajectories,
infinitesimal holonomy, stochastic dynamics, or empirical measurement maps.
Nor does it prove absence of recovery or an equivalence with continuous decay.

## Exact curvature convention

Writing juxtaposition for the coefficient product:

\[
R_D(x,y)=D_xD_y-D_yD_x-D_{[x,y]},
\]
\[
(d_DK)(x,y)=D_xK_y-K_yD_x+K_xD_y-D_yK_x-K_{[x,y]},
\]
\[
(K\wedge K)(x,y)=K_xK_y-K_yK_x.
\]

The theorems prove

\[
R_{D+K}=R_D+d_DK+K\wedge K,
\]
\[
R_{D+\kappa K}=R_D+\kappa d_DK+\kappa^2(K\wedge K).
\]

The unscaled polynomial identity needs only a non-associative ring. The scaled
version additionally requires a ring of scalars and the explicitly declared
`CentralAction` module/central-action laws. The scalar is constant in the
direction argument. No claim is made for a position-dependent scalar without
additional derivative terms.

## Trust and audit

“No project axioms” does **not** mean “no logical axioms whatsoever.” The current
transitive footprint uses Lean's standard `propext`, `Quot.sound`, and
`Classical.choice`. The executable audit permits only these three, rejects
project axiom declarations, and rejects every other reachable axiom, including
admitted-proof and native-evaluation shortcuts. It scans all declarations under
`RCDA`, including generated declarations, not only the headline catalogue.

The generic sector-escape theorem and the transport memory theorem have empty
axiom footprints. The concrete witnesses and both curvature identities use
`propext`. The division-to-disjunction theorem also uses classical logic; its
conditional specialization inherits that footprint. See `verification/axioms.log`
for the exact per-theorem record and `AUDIT.md` for the claim table.

The algebraic/model laws are structure fields and universally quantified theorem
parameters, not globally postulated constants. Changing a structure can therefore
change which applications are available without manufacturing an instance.

See [DEPENDENCIES.md](DEPENDENCIES.md) for the short dependency map and
[AUDIT.md](AUDIT.md) for the theorem/hypothesis boundary.

## Paper T: typed contextual composition

The [integrated editorial revision](paper/PAPER_T_V1_1.md) responds to the
recovered KIMI review with explicit event/fibre/code types and stronger empirical
baselines. [Provenance](paper/PROVENANCE.md) discloses the truncated v1.0 source.

`RCDA/RelationalComposition.lean` adds nine tagged proofs (seven [P], two [C]).
A countermodel proves that every frozen product can be associative while the
specified operational protocol yields 9 versus 8. Another proves that changing
encodings can coexist with zero defect. Consequently, detecting grouping alone
cannot establish intrinsic non-associativity. A faithful product-preserving
realization by ordinary state-update functions would force associativity.

The current catalogue contains 44 [P]/[C] proofs, eight [V] entries and eight
[H] texts. No new native execution is claimed for the contextual models. The
existing Sounio/C++ corpus remains finite arithmetic evidence. RH/RA/SE/ZA/OC
confirmatory gates remain OPEN. See the [review response](paper/REVIEW_RESPONSE.md).
