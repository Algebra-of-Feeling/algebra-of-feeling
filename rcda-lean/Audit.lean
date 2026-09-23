import Lean
import RCDA

/-! Reproducible axiom and claim-boundary audit. Imports only bundled Lean and RCDA.
The catalogue classifies claims; classification itself is not a truth proof.
Scientific hypotheses are text, never logical axioms. -/
open Lean Elab Command

namespace RCDAAudit
inductive Tag where
  | proved
  | conditional
  | hypothesis
  | validation
  deriving Repr, BEq

def catalogue : List (Name × Tag) := [
  (``RCDA.associator_eq_zero_iff, .proved),
  (``RCDA.associative_iff_associator_zero, .proved),
  (``RCDA.nonAssociative_iff_nonzero_associator, .proved),
  (``RCDA.Sector.associative_iff, .proved),
  (``RCDA.Sector.incompatible, .proved),
  (``RCDA.division_noZeroDivisors, .proved),
  (``RCDA.embedded_sector_no_annihilation, .proved),
  (``RCDA.nontrivial_annihilation_requires_sector_escape, .proved),
  (``RCDA.annihilation_obstructs_left_inverse, .proved),
  (``RCDA.Double.embed_mul, .proved),
  (``RCDA.Double.division_sector_escape, .conditional),
  (``RCDA.RCDAModel.witness_escapes, .conditional),
  (``RCDA.curvature_deformation, .proved),
  (``RCDA.curvature_scaled_deformation, .proved),
  (``RCDA.curvature_zero_deformation, .proved),
  (``RCDA.AssociatorCoupling.associative_preserves_reference, .conditional),
  (``RCDA.Transport.memory_iff_loop_displacement, .proved),
  (``RCDA.concrete_annihilation, .proved),
  (``RCDA.concrete_hasZeroDivisors, .proved),
  (``RCDA.concrete_octonion_associator, .proved),
  (``RCDA.concrete_octonion_associator_value, .proved),
  (``RCDA.concrete_octonion_nonAssociative, .proved),
  (``RCDA.bool_noZeroDivisors, .proved),
  (``RCDA.associativity_does_not_decide_annihilation, .proved),
  (``RCDA.nonAssociative_with_zero_correction, .proved),
  (``RCDA.concrete_no_universal_recovery, .proved),
  (``RCDA.SounioBridge.multiplication_refines, .proved),
  (``RCDA.SounioBridge.loop_refines, .proved),
  (``RCDA.SounioBridge.basisSign_abs, .proved),
  (``RCDA.SounioBridge.xor_index_bound, .proved),
  (``RCDA.IntegerBounds.guard_implies_bound, .proved),
  (``RCDA.IntegerBounds.term_bound, .proved),
  (``RCDA.SounioBridge.innerLoop_bound, .proved),
  (``RCDA.SounioBridge.each_update_fits_i64, .proved),
  (``RCDA.ArithmeticFragment.translation_correct, .proved),
  (``RCDA.RelationalComposition.frozen_protocol_reduces, .proved),
  (``RCDA.RelationalComposition.fixed_product_reduces, .conditional),
  (``RCDA.RelationalComposition.contextual_annihilation_escapes, .conditional),
  (``RCDA.RelationalComposition.shifted_frozen_associative, .proved),
  (``RCDA.RelationalComposition.associative_contexts_nonzero_operational, .proved),
  (``RCDA.RelationalComposition.encoding_changes, .proved),
  (``RCDA.RelationalComposition.encoding_change_without_defect, .proved),
  (``RCDA.RelationalComposition.update_functions_associative, .proved),
  (``RCDA.RelationalComposition.faithful_updates_force_associativity, .proved),
  (``RCDA.Double.lower_mul, .proved),
  (``RCDA.Double.lower_sector_no_annihilation, .proved),
  (``RCDA.Double.upper_mul, .proved),
  (``RCDA.Double.upper_sector_no_annihilation, .proved),
  (``RCDA.Double.annihilation_crosses_levels, .proved)
]

def scientificHypotheses : List (Tag × String) := [
  (.hypothesis, "A dyad's relational state is empirically represented by a fibre."),
  (.hypothesis, "A zero product models psychological or clinical annihilation."),
  (.hypothesis, "Associator-dependent transport has nonzero empirical coupling."),
  (.hypothesis, "Relational holonomy is an identifiable empirical memory mechanism."),
  (.hypothesis, "A post-rupture relationship must be history-distinct and cannot recover."),
  (.hypothesis, "The proposed RCDA/RCDB architecture is scientifically novel."),
  (.hypothesis, "A specified context-deforming protocol outpredicts conventional grouping/state-space models."),
  (.hypothesis, "An identifiable observation-constitutive term outpredicts conditioning plus ordinary intervention/reactivity models.")
]

/-- [V] Evidence metadata only: never a proof or logical premise.
Receipt contents are independently checked by scripts/verify_ledger.py. -/
def validationEvidence : List (Tag × String × String) := [
  (.validation, "V-DIFF1", "Sounio/GCC: 1310 records; differential/verification/receipt.json"),
  (.validation, "V-DIFF2", "Sounio/Clang ASan UBSan: 1310 records; differential/verification/receipt.json"),
  (.validation, "V-EXACT1", "20960 exact coefficients per build; differential/verification/receipt.json"),
  (.validation, "V-NEG1", "7 differential negative controls; differential/verification/receipt.json"),
  (.validation, "V-MUT1", "Compiled sign mutant detected; differential/verification/counterexample.json"),
  (.validation, "V-LEAN1", "Concrete replay checked by Lean; differential/verification/lean-replay-receipt.json"),
  (.validation, "V-ZD1", "Concrete zero-divisor runtime record; differential/verification/sounio-corpus.stdout"),
  (.validation, "V-GATE1", "Sounio positive gate and six rejections; differential/verification/sounio-existing-gate.stdout")
]

/-- Standard foundational axioms are disclosed, not described as algebra assumptions. -/
def allowedAxioms : List Name := [``propext, ``Quot.sound, ``Classical.choice]

run_cmd do
  let env ← getEnv
  let mut count : Nat := 0
  let mut theoremCount : Nat := 0
  let mut used : List Name := []
  let names := (env.constants.toList.map Prod.fst).filter (fun n => (`RCDA).isPrefixOf n)
  let names := names.mergeSort (fun a b => a.toString ≤ b.toString)
  for name in names do
    let some info := env.find? name | throwError "Missing declaration {name}"
    if let .axiomInfo _ := info then
      throwError "Project axiom declaration is forbidden: {name}"
    let axioms ← collectAxioms name
    for ax in axioms do
      if !allowedAxioms.contains ax then
        throwError "Forbidden dependency {ax} in {name}"
      if !used.contains ax then used := ax :: used
    count := count + 1
    if let .thmInfo _ := info then
      theoremCount := theoremCount + 1
      logInfo m!"THEOREM {name}: axioms={axioms}"
  for (name, tag) in catalogue do
    let some (.thmInfo _) := env.find? name |
      throwError "Catalogue entry is not an existing theorem: {name}"
    if tag == .hypothesis || tag == .validation then
      throwError "Hypotheses and finite validation must not be registered as theorems: {name}"
    logInfo m!"CLAIM {repr tag} {name}"
  for (tag, _) in scientificHypotheses do
    unless tag == .hypothesis do throwError "Incorrect scientific hypothesis tag"
  let mut evidenceIds : List String := []
  for (tag, id, _) in validationEvidence do
    unless tag == .validation do throwError "Incorrect validation tag"
    if evidenceIds.contains id then throwError "Duplicate validation id: {id}"
    evidenceIds := id :: evidenceIds
    logInfo m!"EVIDENCE [V] {id}: metadata only"
  logInfo m!"VALIDATION METADATA: {validationEvidence.length} entries; not Lean propositions."
  logInfo m!"AUDIT PASS: {count} RCDA declarations; {theoremCount} theorem declarations; {catalogue.length} tagged claims; {scientificHypotheses.length} scientific hypotheses kept as text."
  logInfo m!"FOUNDATIONAL AXIOMS USED: {used}"
end RCDAAudit
