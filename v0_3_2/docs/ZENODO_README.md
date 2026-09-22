# Álgebra do Sentir — suplemento executável e núcleo formal (v0.3.2)

Suplemento do preprint *Álgebra do Sentir: composição, espaço relacional e transformação da experiência* / *The Algebra of Feeling: composition, relational space and the transformation of experience*, versão 0.3.2, continuação de *The Octonionic Loop* (SSRN 6528182).

Autor: Demetrios Chiuratto Agourakis (Faculdade São Leopoldo Mandic; PUC-SP), ORCID 0009-0001-8671-8878.

## Metadados sugeridos para o formulário do Zenodo

- **Tipo:** Software / Dataset com software (o Zenodo aceita "Software" com arquivos mistos).
- **Título:** Algebra of Feeling v0.3.2 — executable supplement, Lean core and verification records
- **Licença:** a escolher (MIT para o código e CC BY 4.0 para os documentos é a combinação usual).
- **Palavras-chave:** non-associative algebra; Cayley–Dickson; sedenions; zero divisors; Lean 4; formal verification; quantum cognition; phenomenological psychopathology.
- **Related identifiers:** *is supplement to* — SSRN 6528182 (trabalho anterior) e, depois da postagem, o identificador do preprint v0.3.2.
- **Versão:** 0.3.2.

## Conteúdo

**Núcleo formal em Lean 4** (`nucleo-lean/`), toolchain `leanprover/lean4:v4.33.0`, sem Mathlib:
- `RCDA/Concrete.lean` — associador octoniônico \(2e_7\) e par sedeniônico de produto nulo, por `decide`;
- `RCDA/RelationalComposition.lean` — separação entre associador a contexto fixo e defeito operacional;
- `RCDA/Levels.lean` — **rascunho não compilado** da proposição da travessia de níveis;
- `Audit.lean`, `AUDIT.md`, `PROOF_LEDGER.md` — catálogo, auditoria de axiomas e registro de estatutos;
- `scripts/verify.cpp`, `scripts/source_gate.hpp`, `scripts/negative_control.cpp`, `scripts/verify.sh` — verificador em C++23, portão de admissão de fontes e controle negativo executado;
- `verification/` — recibo, logs de build e de axiomas, hashes SHA-256, log do controle negativo.

**Realização executável** (`realizacao/`):
- `bloch_core.hpp`, `cd_bloch.cpp` — produto de Cayley–Dickson, coeficiente \(r_7\), regra de ligação de Bloch e instrumentos de Lüders, em racionais exatos;
- `dump_bloch.cpp`, `compare_cpp_sounio.cpp` — despejo estruturado e comparador automatizado C++/Sounio;
- `decoherence.cpp` — verificação de \(\Delta_Q=\lambda x/2\) e \(\Delta_P=z/2\);
- `exemplo_minimo.sio` e registros de execução em Sounio;
- logs de compilação e execução, com hashes.

**Níveis e divisores de zero** (`niveis/`):
- `levels.cpp` — 15 cópias octoniônicas na base e 2.688 automorfismos monomiais;
- `levels2.cpp` — perfil dos 84 divisores de zero primitivos por cópia;
- `levels3.cpp` — verificação de que nenhum elemento com uma metade nula é divisor de zero;
- `saida.log` — saída das três execuções.

**Frequência e censo** (`frequencia_divisores_zero/`):
- `zd_frequency.cpp` — distribuição de \(\|ab\|\) para pares aleatórios de sedênios unitários;
- `zd_sparse.cpp`, `zd_census.cpp` — censo exato 84 / 336 / 168 em estados esparsos.

## Como reproduzir

```sh
# núcleo formal (requer Lean 4.33.0 e lake no PATH)
cd nucleo-lean && sh scripts/verify.sh

# realização e verificações numéricas (requer compilador C++23)
c++ -std=c++23 -O2 -Wall -Wextra realizacao/cd_bloch.cpp   -o cd_bloch   && ./cd_bloch
c++ -std=c++23 -O2 -Wall -Wextra realizacao/decoherence.cpp -o decoherence && ./decoherence
c++ -std=c++23 -O2 -Wall -Wextra niveis/levels3.cpp         -o levels3     && ./levels3
```

Os cálculos seguem o ADR009 do projeto: C++23, F#, F*, Futhark ou Koka para cálculo, Lean 4 para teorema. Não há código Python neste depósito.

## Estado da verificação

- Reproduzido de forma independente: saída de `cd_bloch.cpp` idêntica byte a byte em GCC 13.3 / Linux x86-64 e em Apple clang / arm64 (SHA-256 `289ee521143606376adb3789c43b57e484bbaba9cc8ebebeec405142cf457a88`).
- Controle negativo do portão de admissão: fonte íntegra aceita; quatro tentativas de admissão rejeitadas.
- Auditoria de axiomas: apenas `Classical.choice`, `Quot.sound` e `propext`.
- Sem prova Lean: a identidade dos efeitos de ordem sob decoerência e a proposição da travessia de níveis; ambas têm prova em prosa e verificação computacional.
- Comparação automatizada C++/Sounio: ferramenta pronta e autotestada; o despejo do lado Sounio ainda não foi gerado.
