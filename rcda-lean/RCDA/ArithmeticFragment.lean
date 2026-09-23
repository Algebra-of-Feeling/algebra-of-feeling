import RCDA.LoopRefinement

/-! Typed arithmetic fragment with explicitly modeled source/target semantics.
This is not a Sounio parser or a theorem about a native executable.
Arithmetic uses mathematical integers; runtime guards are separate. -/
namespace RCDA.ArithmeticFragment
inductive Level where
  | scalar | complex | quaternion | octonion | sedenion
  deriving DecidableEq

def width : Level → Nat
  | .scalar => 1 | .complex => 2 | .quaternion => 4
  | .octonion => 8 | .sedenion => 16

abbrev Carrier : Level → Type
  | .scalar => CD0 | .complex => CD1 | .quaternion => CD2
  | .octonion => CD3 | .sedenion => CD4
abbrev Coordinates := Nat → Int
abbrev Environment := (d : Level) → Nat → Coordinates

def decode : (d : Level) → Coordinates → Carrier d
  | .scalar, a => a 0
  | .complex, a => ⟨a 0, a 1⟩
  | .quaternion, a => ⟨⟨a 0, a 1⟩, ⟨a 2, a 3⟩⟩
  | .octonion, a => ⟨⟨⟨a 0, a 1⟩, ⟨a 2, a 3⟩⟩, ⟨⟨a 4, a 5⟩, ⟨a 6, a 7⟩⟩⟩
  | .sedenion, a => SounioBridge.encode a

def modelAdd : (d : Level) → Carrier d → Carrier d → Carrier d
  | .scalar => (· + ·) | .complex => (· + ·) | .quaternion => (· + ·)
  | .octonion => (· + ·) | .sedenion => (· + ·)
def modelSub : (d : Level) → Carrier d → Carrier d → Carrier d
  | .scalar => AbelianGroup.sub | .complex => AbelianGroup.sub
  | .quaternion => AbelianGroup.sub | .octonion => AbelianGroup.sub
  | .sedenion => AbelianGroup.sub
def modelConj : (d : Level) → Carrier d → Carrier d
  | .scalar => Conjugation.conj | .complex => Conjugation.conj
  | .quaternion => Conjugation.conj | .octonion => Conjugation.conj
  | .sedenion => Conjugation.conj

def join (half : Nat) (a b : Coordinates) : Coordinates :=
  fun i => if i < half then a i else b (i-half)
def arrayAdd (a b : Coordinates) : Coordinates := fun i => a i + b i
def arraySub (a b : Coordinates) : Coordinates := fun i => a i + -b i
def arrayConj (a : Coordinates) : Coordinates := fun i => if i = 0 then a i else -a i

/-- Source AST. Multiplication is scoped to dimension 16, where the modeled
sign/XOR loop already has a universal refinement proof. -/
inductive Expr : Level → Type where
  | literal : Int → Expr .scalar
  | input {d} : Nat → Expr d
  | basis {d} : Fin (width d) → Expr d
  | pair1 : Expr .scalar → Expr .scalar → Expr .complex
  | pair2 : Expr .complex → Expr .complex → Expr .quaternion
  | pair3 : Expr .quaternion → Expr .quaternion → Expr .octonion
  | pair4 : Expr .octonion → Expr .octonion → Expr .sedenion
  | add {d} : Expr d → Expr d → Expr d
  | sub {d} : Expr d → Expr d → Expr d
  | conjugate {d} : Expr d → Expr d
  | multiply : Expr .sedenion → Expr .sedenion → Expr .sedenion

/-- Array-oriented target AST: pairs become indexed joins; multiplication uses
an explicit modeled update loop. No external compiler is assumed correct. -/
inductive Code : Level → Type where
  | constant {d} : Coordinates → Code d
  | load {d} : Nat → Code d
  | join1 : Code .scalar → Code .scalar → Code .complex
  | join2 : Code .complex → Code .complex → Code .quaternion
  | join3 : Code .quaternion → Code .quaternion → Code .octonion
  | join4 : Code .octonion → Code .octonion → Code .sedenion
  | add {d} : Code d → Code d → Code d
  | sub {d} : Code d → Code d → Code d
  | conjugate {d} : Code d → Code d
  | multiplyLoop : Code .sedenion → Code .sedenion → Code .sedenion

def translate : {d : Level} → Expr d → Code d
  | _, .literal z => .constant (fun i => if i = 0 then z else 0)
  | _, .input n => .load n
  | _, .basis i => .constant (fun k => if k = i.val then 1 else 0)
  | _, .pair1 a b => .join1 (translate a) (translate b)
  | _, .pair2 a b => .join2 (translate a) (translate b)
  | _, .pair3 a b => .join3 (translate a) (translate b)
  | _, .pair4 a b => .join4 (translate a) (translate b)
  | _, .add a b => .add (translate a) (translate b)
  | _, .sub a b => .sub (translate a) (translate b)
  | _, .conjugate a => .conjugate (translate a)
  | _, .multiply a b => .multiplyLoop (translate a) (translate b)

def run (ρ : Environment) : {d : Level} → Code d → Coordinates
  | _, .constant a => a
  | d, .load n => ρ d n
  | _, .join1 a b => join 1 (run ρ a) (run ρ b)
  | _, .join2 a b => join 2 (run ρ a) (run ρ b)
  | _, .join3 a b => join 4 (run ρ a) (run ρ b)
  | _, .join4 a b => join 8 (run ρ a) (run ρ b)
  | _, .add a b => arrayAdd (run ρ a) (run ρ b)
  | _, .sub a b => arraySub (run ρ a) (run ρ b)
  | _, .conjugate a => arrayConj (run ρ a)
  | _, .multiplyLoop a b => SounioBridge.outerLoop (run ρ a) (run ρ b) 16

def denote (ρ : Environment) : {d : Level} → Expr d → Carrier d
  | _, .literal z => z
  | d, .input n => decode d (ρ d n)
  | d, .basis i => decode d (fun k => if k = i.val then 1 else 0)
  | _, .pair1 a b => ⟨denote ρ a, denote ρ b⟩
  | _, .pair2 a b => ⟨denote ρ a, denote ρ b⟩
  | _, .pair3 a b => ⟨denote ρ a, denote ρ b⟩
  | _, .pair4 a b => ⟨denote ρ a, denote ρ b⟩
  | d, .add a b => modelAdd d (denote ρ a) (denote ρ b)
  | d, .sub a b => modelSub d (denote ρ a) (denote ρ b)
  | d, .conjugate a => modelConj d (denote ρ a)
  | _, .multiply a b => denote ρ a * denote ρ b

@[simp] theorem decode_add (d : Level) (a b : Coordinates) :
    decode d (arrayAdd a b) = modelAdd d (decode d a) (decode d b) := by
  cases d <;> rfl
@[simp] theorem decode_sub (d : Level) (a b : Coordinates) :
    decode d (arraySub a b) = modelSub d (decode d a) (decode d b) := by
  cases d <;> rfl
@[simp] theorem decode_conj (d : Level) (a : Coordinates) :
    decode d (arrayConj a) = modelConj d (decode d a) := by
  cases d <;> rfl
@[simp] theorem decode_join1 (a b : Coordinates) :
    decode .complex (join 1 a b) = Double.mk (decode .scalar a) (decode .scalar b) := rfl
@[simp] theorem decode_join2 (a b : Coordinates) :
    decode .quaternion (join 2 a b) = Double.mk (decode .complex a) (decode .complex b) := rfl
@[simp] theorem decode_join3 (a b : Coordinates) :
    decode .octonion (join 4 a b) = Double.mk (decode .quaternion a) (decode .quaternion b) := rfl
@[simp] theorem decode_join4 (a b : Coordinates) :
    decode .sedenion (join 8 a b) = Double.mk (decode .octonion a) (decode .octonion b) := rfl

/-- [P] Compositional translation correctness for every well-typed expression
and every integer input in these ASTs. Not Sounio parsing or native execution. -/
theorem translation_correct {d : Level} (e : Expr d) (ρ : Environment) :
    decode d (run ρ (translate e)) = denote ρ e := by
  induction e with
  | literal z => rfl
  | input n => rfl
  | basis i => rfl
  | pair1 a b ha hb => simp only [translate, run, decode_join1, denote, ha, hb] <;> rfl
  | pair2 a b ha hb => simp only [translate, run, decode_join2, denote, ha, hb] <;> rfl
  | pair3 a b ha hb => simp only [translate, run, decode_join3, denote, ha, hb] <;> rfl
  | pair4 a b ha hb => simp only [translate, run, decode_join4, denote, ha, hb] <;> rfl
  | add a b ha hb => simp only [translate, run, decode_add, denote, ha, hb]
  | sub a b ha hb => simp only [translate, run, decode_sub, denote, ha, hb]
  | conjugate a ha => simp only [translate, run, decode_conj, denote, ha]
  | multiply a b ha hb =>
      change SounioBridge.encode (SounioBridge.outerLoop _ _ 16) = _
      rw [SounioBridge.loop_refines]
      change SounioBridge.encode _ = _ at ha hb
      rw [ha, hb]
      rfl
end RCDA.ArithmeticFragment
