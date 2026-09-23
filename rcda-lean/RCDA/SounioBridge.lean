import RCDA.Concrete

/-! Mathematical refinement of the integer-coordinate Sounio algorithm.
The algorithm is modeled explicitly, not inferred from the compiler or ELF.
No compiler correctness theorem is asserted. -/
namespace RCDA.SounioBridge

/-- Sounio's recursive sign rule, with fuel equal to the number of doublings. -/
def basisSign : Nat → Nat → Nat → Nat → Int
  | 0, _, _, _ => 1
  | fuel+1, dim, i, j =>
    if dim = 1 then 1
    else
      let h := dim / 2
      if i < h ∧ j < h then basisSign fuel h i j
      else if i < h ∧ j ≥ h then basisSign fuel h (j-h) i
      else if i ≥ h ∧ j < h then
        (if j = 0 then 1 else -1) * basisSign fuel h (i-h) j
      else (if j-h = 0 then -1 else 1) * basisSign fuel h (j-h) (i-h)

/-- Ascending, left-accumulating loop, starting at zero. -/
def sumLoop (n : Nat) (f : Nat → Int) : Int :=
  match n with
  | 0 => 0
  | k+1 => sumLoop k f + f k

/-- Coordinate projection of the nested i,j loops in cd_mul.
Coordinates with i XOR j different from k leave the accumulator unchanged. -/
def coefficient (a b : Nat → Int) (k : Nat) : Int :=
  sumLoop 16 fun i => sumLoop 16 fun j =>
    if Nat.xor i j = k then basisSign 4 16 i j * a i * b j else 0

/-- The first sixteen arbitrary integer coefficients as four Cayley-Dickson doublings. -/
def encode (a : Nat → Int) : CD4 :=
  ⟨⟨⟨⟨a 0, a 1⟩, ⟨a 2, a 3⟩⟩, ⟨⟨a 4, a 5⟩, ⟨a 6, a 7⟩⟩⟩, ⟨⟨⟨a 8, a 9⟩, ⟨a 10, a 11⟩⟩, ⟨⟨a 12, a 13⟩, ⟨a 14, a 15⟩⟩⟩⟩

set_option maxRecDepth 8192 in
set_option maxHeartbeats 8000000 in
/-- [P] For ALL integer coefficients, the modeled Sounio convolution equals
fourfold Cayley-Dickson multiplication, not merely on sampled inputs. -/
theorem multiplication_refines (a b : Nat → Int) :
    encode (coefficient a b) = encode a * encode b := by
  apply Double.ext <;> apply Double.ext <;> apply Double.ext <;> apply Double.ext
  all_goals
    simp [encode, coefficient, sumLoop, basisSign, Double.mul_re, Double.mul_im,
      AbelianGroup.sub, Conjugation.conj]
    simp only [Int.mul_comm]
    unfold CD0 at *
    omega

end RCDA.SounioBridge
