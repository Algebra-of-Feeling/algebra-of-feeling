import RCDA.Concrete

/-! Exact integer bounds for the Sounio coefficient guard and accumulator.
These are mathematical bounds; machine-code preservation is a separate obligation. -/
namespace RCDA.IntegerBounds

def inputBound : Nat := 1000000
def termBound : Nat := inputBound * inputBound
def accumulatorBound : Nat := 256 * termBound

/-- [P] The signed product of two admitted coordinates has magnitude at most 10^12. -/
theorem term_bound (sign a b : Int) (hs : sign.natAbs ≤ 1)
    (ha : a.natAbs ≤ inputBound) (hb : b.natAbs ≤ inputBound) :
    (sign * a * b).natAbs ≤ termBound := by
  rw [Int.natAbs_mul, Int.natAbs_mul]
  have hsa := Nat.mul_le_mul hs ha
  simp only [Nat.one_mul] at hsa
  exact Nat.mul_le_mul hsa hb

/-- [P] The runtime comparison guard implies the absolute-value bound. -/
theorem guard_implies_bound (a : Int) (lo : -1000000 ≤ a) (hi : a ≤ 1000000) :
    a.natAbs ≤ inputBound := by
  cases a <;> simp_all [inputBound] <;> omega

/-- [P] A conservative full 16x16 traversal bound fits signed 64-bit arithmetic. -/
theorem accumulator_fits_i64 (a : Int) (ha : a.natAbs ≤ accumulatorBound) :
    -9223372036854775808 ≤ a ∧ a ≤ 9223372036854775807 := by
  cases a <;> simp_all [accumulatorBound, termBound, inputBound] <;> omega
end RCDA.IntegerBounds
