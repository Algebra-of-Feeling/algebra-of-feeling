import RCDA.CayleyDickson

/-!
# Níveis da duplicação e divisores de zero

Proposição: na duplicação `Double D` de uma álgebra de divisão `D`, um elemento
cuja parte imaginária é nula e cuja parte real é não nula não aniquila nenhum
elemento não nulo. Em consequência, todo divisor de zero de `Double D` tem
componentes não nulas nas duas metades: a anulação atravessa os dois níveis.

O caso simétrico (parte real nula) exige injetividade da conjugação, disponível
em `OctonionLike` via `conj_involutive`, e é tratado em `upper_sector_no_annihilation`.
-/

namespace RCDA
namespace Double

universe u
variable {D : Type u} [DivisionAlgebra D] [Conjugation D]

/-- [P] O produto de um elemento da metade inferior por um elemento qualquer. -/
theorem lower_mul (a : D) (y : Double D) :
    (⟨a, 0⟩ : Double D) * y = ⟨a * y.re, y.im * a⟩ := by
  change (⟨AbelianGroup.sub (a * y.re) (Conjugation.conj y.im * 0),
      y.im * a + 0 * Conjugation.conj y.re⟩ : Double D) = _
  simp only [AbelianGroup.sub, NonAssocRing.mul_zero, NonAssocRing.zero_mul,
    AbelianGroup.neg_zero, AbelianGroup.add_zero]

/-- [P] Um elemento não nulo da metade inferior não é divisor de zero. -/
theorem lower_sector_no_annihilation {a : D} (ha : a ≠ 0) {y : Double D}
    (h : (⟨a, 0⟩ : Double D) * y = 0) : y = 0 := by
  rw [lower_mul] at h
  have hre : a * y.re = 0 := congrArg Double.re h
  have him : y.im * a = 0 := congrArg Double.im h
  have hzd := division_noZeroDivisors D
  have hre0 : y.re = 0 := (hzd a y.re hre).resolve_left ha
  have him0 : y.im = 0 := (hzd y.im a him).resolve_right ha
  have : y = ⟨y.re, y.im⟩ := rfl
  rw [this, hre0, him0]
  rfl

end Double

namespace Double

universe v
variable {O : Type v} [OctonionLike O]

/-- [P] O produto de um elemento da metade superior por um elemento qualquer. -/
theorem upper_mul (b : O) (y : Double O) :
    (⟨0, b⟩ : Double O) * y
      = ⟨AbelianGroup.sub 0 (Conjugation.conj y.im * b), b * Conjugation.conj y.re⟩ := by
  change (⟨AbelianGroup.sub (0 * y.re) (Conjugation.conj y.im * b),
      y.im * 0 + b * Conjugation.conj y.re⟩ : Double O) = _
  simp only [NonAssocRing.zero_mul, NonAssocRing.mul_zero, AbelianGroup.zero_add]

/-- [P] Um elemento não nulo da metade superior não é divisor de zero. -/
theorem upper_sector_no_annihilation {b : O} (hb : b ≠ 0) {y : Double O}
    (h : (⟨0, b⟩ : Double O) * y = 0) : y = 0 := by
  rw [upper_mul] at h
  have h1 : AbelianGroup.sub 0 (Conjugation.conj y.im * b) = 0 := congrArg Double.re h
  have h2 : b * Conjugation.conj y.re = 0 := congrArg Double.im h
  have hzd := division_noZeroDivisors O
  have hconj_im : Conjugation.conj y.im = 0 := by
    have : Conjugation.conj y.im * b = 0 := ((AbelianGroup.sub_eq_zero_iff 0 _).mp h1).symm
    exact (hzd _ b this).resolve_right hb
  have hconj_re : Conjugation.conj y.re = 0 := (hzd b _ h2).resolve_left hb
  have him0 : y.im = 0 := by
    have := congrArg Conjugation.conj hconj_im
    rwa [OctonionLike.conj_involutive, Conjugation.conj_zero] at this
  have hre0 : y.re = 0 := by
    have := congrArg Conjugation.conj hconj_re
    rwa [OctonionLike.conj_involutive, Conjugation.conj_zero] at this
  have : y = ⟨y.re, y.im⟩ := rfl
  rw [this, hre0, him0]
  rfl

/-- [P] Consequência: toda aniquilação não trivial tem componentes não nulas nas
duas metades — a anulação atravessa os dois níveis da duplicação. -/
theorem annihilation_crosses_levels {x y : Double O}
    (h : NontrivialAnnihilation x y) : x.re ≠ 0 ∧ x.im ≠ 0 := by
  obtain ⟨hx, hy, hxy⟩ := h
  constructor
  · intro hre
    have hx' : x = ⟨0, x.im⟩ := by
      have : x = ⟨x.re, x.im⟩ := rfl
      rw [this, hre]
    have him : x.im ≠ 0 := by
      intro him
      exact hx (by rw [hx', him]; rfl)
    exact hy (upper_sector_no_annihilation him (by rw [← hx']; exact hxy))
  · intro him
    have hx' : x = ⟨x.re, 0⟩ := by
      have : x = ⟨x.re, x.im⟩ := rfl
      rw [this, him]
    have hre : x.re ≠ 0 := by
      intro hre
      exact hx (by rw [hx', hre]; rfl)
    exact hy (lower_sector_no_annihilation hre (by rw [← hx']; exact hxy))

end Double
end RCDA
