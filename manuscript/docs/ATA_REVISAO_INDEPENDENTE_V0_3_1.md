# Ata de revisão independente — Álgebra do Sentir v0.3.1

**Objeto.** Pacote `GPT_ALGEBRA_DO_SENTIR_V0_3_1_PACOTE_REVISAO.zip`
(SHA-256 `234919c637ffddf8e9b1e16288fa034ca952c4b5e4a31c58d53e58bd6e051df1`, 62 arquivos).

**Data da revisão.** 2026-09-20 (UTC).

**Pendência que originou a revisão.** Na leitura anterior, as §§3 e 5 do manuscrito citavam
proposições de `RCDA.RelationalComposition` que não acompanhavam o pacote entregue. A objeção
era de verificabilidade, não de conteúdo: o material remetido não permitia corroborar as
citações. Esta ata registra o resultado da reinspeção do pacote corrigido.

**Conclusão.** A pendência está sanada. As três proposições citadas existem, batem com o texto
que as invoca e integram uma auditoria Lean internamente consistente. O suplemento numérico de
§5 foi **reproduzido de forma independente**, em plataforma e compilador distintos dos do autor,
com concordância byte a byte. As pendências (a) e (b) do programa estão fechadas; (c) permanece
aberta. Os limites do que **não** pôde ser corroborado estão na seção 5 desta ata.

---

## 1. Método e estatuto das afirmações

Esta ata distingue três níveis de evidência, e cada achado abaixo declara o seu:

- **[R] Reproduzido.** O revisor reexecutou o artefato e comparou o resultado com o entregue.
- **[C] Conferido.** O revisor recalculou uma propriedade verificável do artefato entregue
  (hash, contagem, presença de declaração) sem reexecutar o processo que o gerou.
- **[A] Aceito por registro.** O revisor leu o registro e o considerou internamente coerente,
  sem ter meios de reexecutar o processo no ambiente de revisão.

**Conformidade com o ADR009.** A verificação independente empregou C++23 (`g++` 13.3.0,
Ubuntu 24.04) para o recálculo numérico e utilitários de sistema (`sha256sum`, `diff`, `awk`)
para conferência de integridade e contagens. Nenhum resultado registrado nesta ata depende de
Python, Rust ou Julia.

---

## 2. Núcleo Lean 4

O ambiente de revisão não dispõe de toolchain Lean; **a recompilação do núcleo não foi
reexecutada** pelo revisor. O que foi possível estabelecer:

| Achado | Estatuto |
|---|---|
| As três proposições citadas — `shifted_frozen_associative`, `associative_contexts_nonzero_operational`, `faithful_updates_force_associativity` — estão presentes em `RCDA/RelationalComposition.lean`, com assinaturas compatíveis com o uso que o manuscrito faz delas | [C] |
| Os SHA-256 dos 22 arquivos-fonte rastreados foram recalculados e coincidem integralmente com `verification/sha256.txt` — o entregue é exatamente o que o recibo descreve | [C] |
| Contagens do `receipt.json` conferem com o log: 44 linhas `CLAIM` e 391 linhas `THEOREM` em `verification/axioms.log` | [C] |
| O conjunto de axiomas transitivos extraído de todo o `axioms.log` é exatamente `{Classical.choice, Quot.sound, propext}`; nenhuma dependência fora da lista permitida | [C] |
| Nenhuma ocorrência de `sorry` nos fontes do núcleo ou em `Audit.lean` | [C] |
| `RCDA/Concrete.lean` usa `decide` (três ocorrências) e não `native_decide`, como o manuscrito afirma em §3 | [C] |
| A recompilação limpa em Lean 4.33.0 e a emissão do PASS ocorreram conforme `verification/receipt.json`, `build.log`, `clean.log` e `toolchain.log` | [A] |

**Sobre o desenho da auditoria.** `Audit.lean` foi lido integralmente. Duas propriedades merecem
registro por serem o que dá força ao PASS:

1. O catálogo referencia declarações por `` `` ``Nome`` ``, resolvido na elaboração. Uma entrada
   apontando para teorema inexistente **impede a compilação** — o catálogo não pode divergir
   silenciosamente do núcleo.
2. As contagens de declarações, teoremas e axiomas são produzidas por `run_cmd` percorrendo o
   ambiente, não são números redigidos à mão no recibo.

O verificador `scripts/verify.cpp` (C++23) foi lido linha a linha. Ele barra, antes de qualquer
compilação: os tokens `sorry`, `admit`, `native_decide`, `sorryAx`, `unsafe`, `implemented_by`;
declarações `axiom` e `opaque`; imports fora de `Init`/`Lean`/`RCDA.*`; e qualquer pacote externo
no `lake-manifest.json`. Remove o recibo anterior antes de iniciar as checagens, de modo que uma
falha não possa deixar um PASS obsoleto no lugar. Exige explicitamente a presença das três
proposições das §§3 e 5 antes de emitir o recibo. O desenho é adequado ao que se propõe.

**Ressalva de leitura sobre as contagens.** As 848 declarações e 391 teoremas incluem auxiliares
gerados pelo elaborador (`.injEq`, `.sizeOf_spec`, `_proof_N`, equações de definição). Não são
uma contagem de resultados matemáticos originais, e tanto a `RESPOSTA_REVISAO` quanto o
`PROOF_LEDGER.md` já registram isso. A ata confirma que a ressalva está correta e é necessária.

---

## 3. Suplemento numérico de §5 — reprodução independente

Este é o item de evidência mais forte do pacote.

**Procedimento.** `cd_bloch.cpp` foi recompilado no ambiente de revisão com
`g++ -std=c++23 -Wall -Wextra -O1 -g -fsanitize=address,undefined -fno-sanitize-recover=all`,
em Linux/x86-64 — plataforma, compilador e cadeia de ferramentas **distintos** dos do autor
(Apple clang 21.0.0, arm64-darwin). Compilação sem diagnósticos, código de saída 0. O binário
resultante foi executado; `stderr` vazio, código de saída 0.

**Resultado.** A saída foi comparada com `cd-cpp-run.log` por `diff`: **idêntica, byte a byte**.
O SHA-256 da saída recomputada é `289ee521143606376adb3789c43b57e484bbaba9cc8ebebeec405142cf457a88`,
coincidindo com `run_log_sha256` declarado em `CD_CPP_VERIFICACAO.json`. O SHA-256 do fonte
também coincide com o declarado. O hash do executável difere do declarado, como esperado, por
se tratar de outro compilador e outra arquitetura; isso não afeta a concordância dos resultados.

**Estatuto: [R].** Os números que entram em §5 não são valores transcritos: são saída de código
que roda e produz esses valores em ambiente independente. Em particular ficam reproduzidos o
associador octoniônico `+2 e₇`, o produto sedeniônico nulo `(e₁+e₁₀)(e₄−e₁₅)=0`, as diagonais
`(5/8, 3/8)` e `(3/8, 5/8)` com fora-de-diagonal `±1/16`, os determinantes `59/256`, as tabelas
sequenciais completas nos dois agrupamentos, `q_QQ = 0` em todos os casos calculados, o regime
simétrico de controle e a rejeição do código `r7=9` por violação do domínio de positividade.

**Verificação manual complementar [C].** O contraexemplo aritmético de §3 foi refeito à mão:
com `x⋆_C y = x+y+C`, `C=0`, `(x,y,z)=(1,2,3)`, `U(C,r)=C+r`, obtém-se `C_L=3`, `C_R=2`,
agrupamento esquerdo `9`, direito `8`, `Δ_op = 1`, com `A_C = 0` para todo `C`. Coincide com o
texto do manuscrito e com o enunciado de `associative_contexts_nonzero_operational`.

**Ressalva de escopo, já declarada e confirmada como pertinente.** O tipo racional `Q` usa `long`
e não precisão arbitrária; a exatidão vale para os casos finitos executados. `cdmul` pressupõe
dimensões iguais e potências de dois. A validade é dos casos executados, não universal.

---

## 4. Núcleo Sounio

**Estatuto: [A].** O compilador Sounio não está disponível no ambiente de revisão; nenhuma
reexecução foi feita. Os hashes de compilador, biblioteca, fonte e executável em
`sounio-execution.log` são internamente coerentes com `RECIBO_VERIFICACAO.json`, e as seis
linhas `PASS` mais o marcador `ALGEBRA_V031_PASS` correspondem aos resultados que o C++ produz.
Isso é consistência de registro, não reprodução.

O recibo declara `sounio_rerun_during_ADR009_update: false` e o `LEIA-ME.md` afirma que não houve
comparação automatizada célula a célula entre as duas linguagens nesta atualização. **Ambas as
declarações são honestas e devem permanecer no pacote.** Enquanto a concordância C++/Sounio for
avaliada por leitura e não por comparador, o item 5 do ADR009 está cumprido apenas em parte: há
duas implementações canônicas, mas a concordância entre elas não é ela própria verificada por
máquina. Fica como melhoria identificada, não como defeito da entrega.

---

## 5. O que não foi possível corroborar

A `RESPOSTA_REVISAO_NUCLEO_V0_3_1.md` afirma: *"Um controle negativo com um token de admissão de
prova foi rejeitado, sem emitir recibo PASS."* A afirmação é **plausível** — o regex de
`verify.cpp` de fato barra `sorry`/`admit`/`sorryAx` e o recibo anterior é removido antes de
qualquer checagem, de modo que a falha não deixaria PASS residual. Mas **não há log, recibo de
falha ou artefato do controle negativo no pacote**. O revisor não tem como distinguir um teste
efetivamente executado de uma inferência sobre o comportamento esperado do código.

Recomendação: gravar a saída dessa execução (por exemplo, `verification/negative-control.log`
com o `FAIL:` emitido e a ausência de `receipt.json`) numa próxima adequação. É barato e converte
um [A] num [R].

---

## 6. Pendências do programa

| Pendência | Situação |
|---|---|
| **(a)** Frase órfã em §7 sobre degradação inevitável após reconciliação | **Fechada.** Não foi apresentada como hipótese com condição dinâmica: foi convertida em negação explícita ("não impõe degradação inevitável após uma reconciliação"). A tese de irreversibilidade temporal aparece em §3 e §7 como dependente de dinâmica, informações preservadas e transformações disponíveis, nunca asserida. `PROOF_LEDGER.md` reforça o estatuto: a irrecuperabilidade de toda relação pós-ruptura permanece **[H]**, e não consequência da não injetividade da multiplicação. A separação entre resultado matemático e hipótese fenomenológica está correta neste ponto. |
| **(b)** DOI e paginação de Dubois & Toffano (2026) | **Fechada e confirmada por fonte externa.** *Entropy* 2026, **28**(5), 522; DOI `10.3390/e28050522`; publicado em 5 de maio de 2026; recebido 3 de março, revisado 27 de abril, aceito 29 de abril de 2026. Confere com `REFERENCIAS.md` e com a citação em §7. Registro adicional útil: o artigo integra a special issue *Quantum Information and Probability: From Foundations to Engineering IV*, editada por Jerome Busemeyer — o mesmo Busemeyer que figura entre os interlocutores obrigatórios do programa. |
| **(c)** Tradução para inglês, com decisão de venue prévia | **Aberta.** O pacote é integralmente em português. Nada nesta entrega altera o estado dessa pendência. |

---

## 7. Integridade documental do pacote

[C] O manuscrito monolítico `ALGEBRA_DO_SENTIR_MANUSCRITO_V0_3_1.md` foi comparado por `diff`
contra os arquivos por seção de `v0_3_1/`. As §§1, 3 e 5 coincidem integralmente; as únicas
divergências são de nível de cabeçalho (`#` → `##`, `##` → `###`) e as linhas de enquadramento
editorial no topo dos arquivos avulsos. Não há divergência de conteúdo entre as duas formas de
entrega — isto é, revisar uma delas é revisar a outra.

[C] Busca exaustiva no pacote não encontrou nenhum arquivo `.py` ou `.ipynb`, e nenhuma menção a
Python, NumPy, SciPy ou SymPy fora das passagens que registram a própria proibição do ADR009.
A afirmação "o pacote não contém arquivos Python" está correta.

---

## 8. Registro final

O pacote v0.3.1 responde adequadamente à objeção de verificabilidade levantada na revisão
anterior. As citações das §§3 e 5 estão corroboradas pelos artefatos entregues, e a realização
numérica de §5 sustenta reprodução independente fora da máquina do autor. As declarações de
escopo do pacote — ausência de nova prova Lean para a ponte de Bloch e para QQ, ausência de
reexecução Sounio, ausência de validação empírica ou clínica, ausência de comparador automatizado
entre linguagens — foram conferidas uma a uma e nenhuma se mostrou otimista em relação ao que os
artefatos suportam. Essa calibragem é, em si, um resultado da revisão.

Permanecem como trabalho subsequente: o log do controle negativo (§5 desta ata), a comparação
automatizada C++/Sounio (§4), e a tradução com decisão de venue (§6, item c).
