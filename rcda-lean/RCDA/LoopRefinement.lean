import RCDA.SounioBridge
import RCDA.IntegerBounds

/-! Functional model of the two ascending array-update loops in cd_mul.
Array writes are modeled by a coordinate function updated at i XOR j. -/
namespace RCDA.SounioBridge

def rowLoop (a b : Nat → Int) (i : Nat) (start : Nat → Int) : Nat → Nat → Int
  | 0 => start
  | j+1 => fun k =>
    if Nat.xor i j = k then rowLoop a b i start j k + basisSign 4 16 i j * a i * b j
    else rowLoop a b i start j k

/-- The coordinate invariant for each individual row of array updates. -/
theorem rowLoop_project (a b : Nat → Int) (i : Nat) (start : Nat → Int) (n k : Nat) :
    rowLoop a b i start n k = start k + sumLoop n (fun j =>
      if Nat.xor i j = k then basisSign 4 16 i j * a i * b j else 0) := by
  induction n with
  | zero => simp [rowLoop, sumLoop]
  | succ n ih =>
    simp only [rowLoop, sumLoop]
    rw [ih]
    split <;> simp_all [AbelianGroup.add_assoc]

def outerLoop (a b : Nat → Int) : Nat → Nat → Int
  | 0 => fun _ => 0
  | i+1 => rowLoop a b i (outerLoop a b i) 16

/-- The invariant relating the full ascending loop nest to the convolution. -/
theorem outerLoop_project (a b : Nat → Int) (n k : Nat) :
    outerLoop a b n k = sumLoop n (fun i => sumLoop 16 fun j =>
      if Nat.xor i j = k then basisSign 4 16 i j * a i * b j else 0) := by
  induction n with
  | zero => rfl
  | succ n ih =>
    simp only [outerLoop, rowLoop_project, sumLoop, ih]

/-- [P] The modeled Sounio array-update algorithm refines the Cayley-Dickson
product for arbitrary integer input coefficients in dimension sixteen. -/
theorem loop_refines (a b : Nat → Int) :
    encode (outerLoop a b 16) = encode a * encode b := by
  have h : outerLoop a b 16 = coefficient a b := funext (outerLoop_project a b 16)
  rw [h]
  exact multiplication_refines a b

/-- [P] Every XOR destination in the 16-coordinate loop is an in-bounds array index. -/
theorem xor_index_bound : ∀ i j : Fin 16, Nat.xor i.val j.val < 16 := by
  decide

/-- Every recursively generated basis sign has magnitude exactly one. -/
theorem basisSign_abs (fuel dim i j : Nat) : (basisSign fuel dim i j).natAbs = 1 := by
  induction fuel generalizing dim i j with
  | zero => rfl
  | succ fuel ih =>
    simp only [basisSign]
    split
    · rfl
    · split
      · exact ih _ _ _
      · split
        · exact ih _ _ _
        · split
          · rw [Int.natAbs_mul, ih]
            split <;> decide
          · rw [Int.natAbs_mul, ih]
            split <;> decide

/-- Bound for every prefix, not only the final sum. -/
theorem sumLoop_bound (n : Nat) (f : Nat → Int) (bound : Nat)
    (h : ∀ i, i < n → (f i).natAbs ≤ bound) :
    (sumLoop n f).natAbs ≤ n * bound := by
  induction n with
  | zero => simp [sumLoop]
  | succ n ih =>
    have hp := ih (fun i hi => h i (Nat.lt_trans hi (Nat.lt_succ_self n)))
    have hn := h n (Nat.lt_succ_self n)
    have hs := Nat.le_trans (Int.natAbs_add_le (sumLoop n f) (f n)) (Nat.add_le_add hp hn)
    simpa only [sumLoop, Nat.succ_mul] using hs

/-- A full-row prefix bound retaining its exact row count. -/
theorem outerLoop_count_bound (a b : Nat → Int) (n k : Nat) (hn : n ≤ 16)
    (ha : ∀ i, i < 16 → (a i).natAbs ≤ IntegerBounds.inputBound)
    (hb : ∀ j, j < 16 → (b j).natAbs ≤ IntegerBounds.inputBound) :
    (outerLoop a b n k).natAbs ≤ n * (16 * IntegerBounds.termBound) := by
  rw [outerLoop_project]
  apply sumLoop_bound
  intro i hi
  apply sumLoop_bound
  intro j hj
  split
  · exact IntegerBounds.term_bound _ _ _ (by rw [basisSign_abs]; decide)
      (ha i (Nat.lt_of_lt_of_le hi hn)) (hb j hj)
  · exact Nat.zero_le _

/-- [P] Every full-row prefix satisfies the conservative 256-term bound. -/
theorem outerLoop_bound (a b : Nat → Int) (n k : Nat) (hn : n ≤ 16)
    (ha : ∀ i, i < 16 → (a i).natAbs ≤ IntegerBounds.inputBound)
    (hb : ∀ j, j < 16 → (b j).natAbs ≤ IntegerBounds.inputBound) :
    (outerLoop a b n k).natAbs ≤ IntegerBounds.accumulatorBound := by
  have hout := outerLoop_count_bound a b n k hn ha hb
  have hmul := Nat.mul_le_mul_right (16 * IntegerBounds.termBound) hn
  exact Nat.le_trans hout (by
    simpa [IntegerBounds.accumulatorBound, IntegerBounds.termBound, IntegerBounds.inputBound]
      using hmul)

/-- [P] Every individual array-update prefix, including a partially completed row,
is bounded before the next write. This avoids relying solely on a final bound. -/
theorem innerLoop_bound (a b : Nat → Int) (i j k : Nat) (hi : i < 16) (hj : j ≤ 16)
    (ha : ∀ n, n < 16 → (a n).natAbs ≤ IntegerBounds.inputBound)
    (hb : ∀ n, n < 16 → (b n).natAbs ≤ IntegerBounds.inputBound) :
    (rowLoop a b i (outerLoop a b i) j k).natAbs ≤ IntegerBounds.accumulatorBound := by
  rw [rowLoop_project]
  have hstart := outerLoop_count_bound a b i k (Nat.le_of_lt hi) ha hb
  have hrow := sumLoop_bound j (fun n =>
      if Nat.xor i n = k then basisSign 4 16 i n * a i * b n else 0)
      IntegerBounds.termBound (by
    intro n hn
    split
    · exact IntegerBounds.term_bound _ _ _ (by rw [basisSign_abs]; decide)
        (ha i hi) (hb n (Nat.lt_of_lt_of_le hn hj))
    · exact Nat.zero_le _)
  have hsum := Nat.le_trans (Int.natAbs_add_le _ _) (Nat.add_le_add hstart hrow)
  simp only [IntegerBounds.accumulatorBound, IntegerBounds.termBound, IntegerBounds.inputBound] at *
  omega

/-- [P] Signed 64-bit range for each modeled accumulator write. -/
theorem each_update_fits_i64 (a b : Nat → Int) (i j k : Nat) (hi : i < 16) (hj : j ≤ 16)
    (ha : ∀ n, n < 16 → (a n).natAbs ≤ IntegerBounds.inputBound)
    (hb : ∀ n, n < 16 → (b n).natAbs ≤ IntegerBounds.inputBound) :
    -9223372036854775808 ≤ rowLoop a b i (outerLoop a b i) j k ∧
    rowLoop a b i (outerLoop a b i) j k ≤ 9223372036854775807 :=
  IntegerBounds.accumulator_fits_i64 _ (innerLoop_bound a b i j k hi hj ha hb)
end RCDA.SounioBridge
