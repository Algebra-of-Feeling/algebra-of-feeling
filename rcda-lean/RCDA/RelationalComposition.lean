import RCDA.Concrete

/-! Typed event encoding and explicitly specified contextual composition.
No empirical interpretation, group action law, or dynamics is assumed proved. -/
namespace RCDA.RelationalComposition
universe u

/-- Base, relational states, primitive events and algebraic codes are distinct
sorts. Context updates remain within one fibre. `update` is NOT assumed to be
an algebra action; `compose` need not satisfy ring laws. -/
structure System (Base : Type u) (F E R : Base → Type u) where
  encode : (b : Base) → F b → E b → R b
  update : (b : Base) → F b → R b → F b
  compose : (b : Base) → F b → R b → R b → R b

variable {Base : Type u} {F E R : Base → Type u}

/-- Explicit choices of contexts at the internal nodes of the two trees.
A protocol is model data, not a consequence of having contextual encodings. -/
structure Protocol (F R : Type u) where
  leftContext : F → R → R → R → F
  rightContext : F → R → R → R → F

def afterComposite (s : System Base F E R) (b : Base) : Protocol (F b) (R b) where
  leftContext C x y _ := s.update b C (s.compose b C x y)
  rightContext C _ y _ := s.update b C y

def leftValue (s : System Base F E R) (b : Base) (p : Protocol (F b) (R b))
    (C : F b) (x y z : R b) : R b :=
  s.compose b (p.leftContext C x y z) (s.compose b C x y) z

def rightValue (s : System Base F E R) (b : Base) (p : Protocol (F b) (R b))
    (C : F b) (x y z : R b) : R b :=
  s.compose b C x (s.compose b (p.rightContext C x y z) y z)

def operationalDefect (s : System Base F E R) (b : Base) [AbelianGroup (R b)]
    (p : Protocol (F b) (R b)) (C : F b) (x y z : R b) : R b :=
  AbelianGroup.sub (leftValue s b p C x y z) (rightValue s b p C x y z)

def frozenDefect (s : System Base F E R) (b : Base) [AbelianGroup (R b)]
    (C : F b) (x y z : R b) : R b :=
  AbelianGroup.sub (s.compose b C (s.compose b C x y) z)
    (s.compose b C x (s.compose b C y z))

/-- Primitive event encodings are frozen at the same initial state in both
conditions. Online re-encoding would be a different, separately specified test. -/
def eventDefect (s : System Base F E R) (b : Base) [AbelianGroup (R b)]
    (p : Protocol (F b) (R b)) (C : F b) (e₁ e₂ e₃ : E b) : R b :=
  operationalDefect s b p C (s.encode b C e₁) (s.encode b C e₂) (s.encode b C e₃)

/-- [P] If the protocol freezes both contexts, the operational and frozen
algebraic defects coincide. -/
theorem frozen_protocol_reduces (s : System Base F E R) (b : Base)
    [AbelianGroup (R b)] (p : Protocol (F b) (R b)) (C : F b) (x y z : R b)
    (hl : p.leftContext C x y z = C) (hr : p.rightContext C x y z = C) :
    operationalDefect s b p C x y z = frozenDefect s b C x y z := by
  simp only [operationalDefect, leftValue, rightValue, frozenDefect, hl, hr]

/-- [C] Identifying contextual composition with the fixed RCDA product is an
explicit compatibility premise. The protocol can then be arbitrary. -/
theorem fixed_product_reduces (s : System Base F E R) (b : Base)
    [NonAssocRing (R b)] (p : Protocol (F b) (R b)) (C : F b) (x y z : R b)
    (h : ∀ C a d, s.compose b C a d = a * d) :
    operationalDefect s b p C x y z = associator x y z := by
  simp only [operationalDefect, leftValue, rightValue, h, associator]

/-- [C] The sector theorem transfers only when the contextual product at this
state is identified with the product preserved by the embedding. -/
theorem contextual_annihilation_escapes {D : Type u} [Zero D] [Mul D]
    (s : System Base F E R) (b : Base) [NonAssocRing (R b)]
    (i : ProductEmbedding D (R b)) (hD : NoZeroDivisors D)
    (C : F b) (x z : R b) (hx : x ≠ 0) (hz : z ≠ 0)
    (hcompat : s.compose b C x z = x * z) (hzero : s.compose b C x z = 0) :
    ¬ (InSector i x ∧ InSector i z) := by
  apply nontrivial_annihilation_requires_sector_escape i hD
  exact ⟨hx, hz, hcompat.symm.trans hzero⟩

/-- A conventional state-dependent, associative-at-each-context operation.
This is an operation on an additive group, not asserted to be a ring product. -/
def shifted : System Unit (fun _ => Int) (fun _ => Int) (fun _ => Int) where
  encode _ C e := C + e
  update _ C x := C + x
  compose _ C x y := x + y + C

/-- [P] Every frozen operation in this countermodel is associative. -/
theorem shifted_frozen_associative (C x y z : Int) :
    shifted.compose () C (shifted.compose () C x y) z =
      shifted.compose () C x (shifted.compose () C y z) := by
  simp only [shifted]
  omega

/-- [P] The specified operational protocol nevertheless has nonzero defect. -/
theorem associative_contexts_nonzero_operational :
    operationalDefect shifted () (afterComposite shifted ()) 0 1 2 3 = 1 := by
  decide

/-- Encoding and state may change while composition remains associative. -/
def additive : System Unit (fun _ => Int) (fun _ => Int) (fun _ => Int) where
  encode _ C e := C + e
  update _ C x := C + x
  compose _ _ x y := x + y

/-- [P] Contextual encoding really changes along an update in this model. -/
theorem encoding_changes :
    additive.encode () (additive.update () 0 1) 2 ≠ additive.encode () 0 2 := by
  decide

/-- [P] That change does not force an operational grouping defect. -/
theorem encoding_change_without_defect (p : Protocol Int Int) (C x y z : Int) :
    operationalDefect additive () p C x y z = 0 := by
  simp only [operationalDefect, leftValue, rightValue, additive, AbelianGroup.sub]
  omega

/-- [P] Ordinary composition of state transformations remains associative,
even when the transformations change the state/context. -/
theorem update_functions_associative {T : Type u} (f g h : T → T) :
    (fun x => f (g (h x))) = (fun x => (fun y => f (g y)) (h x)) := rfl

/-- [P] A faithful realization as ordinary function composition would force
associativity. Non-associative codes therefore cannot silently be identified
with faithfully composed state-update functions. -/
theorem faithful_updates_force_associativity {A T : Type u} [Mul A]
    (act : A → T → T)
    (faithful : ∀ x y, (∀ C, act x C = act y C) → x = y)
    (preserves : ∀ x y C, act (x * y) C = act x (act y C)) :
    ∀ x y z : A, (x * y) * z = x * (y * z) := by
  intro x y z
  apply faithful
  intro C
  simp only [preserves]

end RCDA.RelationalComposition
