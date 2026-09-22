# Especificação do despejo Sounio para o comparador C++/Sounio

Este documento é um contrato, não código. Eu não tenho `souc` neste ambiente e não tenho
visibilidade da API de formatação/impressão numérica do Sounio além do que aparece em
`exemplo_minimo.sio` e em `stdlib/algebra/cayley_dickson.sio` — nenhum dos dois usa impressão
formatada de número (só `println` de literais). Por isso não escrevo aqui o `.sio` que produz o
despejo: eu não teria como compilar nem rodar para confirmar que funciona, e entregar Sounio não
testado seria exatamente o tipo de afirmação não verificada que este projeto pede para eu evitar.
O que segue é preciso o bastante para você (ou eu, numa sessão com acesso a `souc`) escrever isso
em poucos minutos.

## O que o lado C++ já produz

`realizacao/dump_bloch.cpp`, compilado e executado nesta sessão, escreve
`verification/cpp_dump.tsv`: 115 linhas, uma por campo, formato

```
chave<TAB>valor_double<TAB>fracao_exata
```

O comparador (`compare_cpp_sounio.cpp`) só lê as duas primeiras colunas. A terceira é para
conferência humana.

## O que o lado Sounio precisa produzir

Um arquivo no mesmo formato de chave (a terceira coluna pode ser omitida — o comparador aceita
duas colunas), com as **mesmas 115 chaves**, na notação `<regime>.<codigo>.<campo>`:

- `regime` ∈ `{asym, sym}` — `asym` é `kx=1/2` (quebra de simetria), `sym` é `kx=1` (controle
  simétrico). **`exemplo_minimo.sio` hoje só calcula o regime `asym`** (via `update()`, que fixa
  os coeficientes `0.125` e `0.0625` — que são `kx·s/2` e `s/2` para `kx=1/2`). Rodar o regime
  `sym` exige generalizar `update()` para receber `kx` como parâmetro, como `cd_bloch.cpp` faz.
- `codigo` ∈ `{rho_L, rho_R, rho_0, invalid}` — os quatro casos já calculados em
  `exemplo_minimo.sio` (`rho_l`, `rho_r`, `rho_zero`, código `r7=9`).
- `campo`, para os três primeiros códigos (quando `admissible=1`):

  | campo | significado | onde já existe em `exemplo_minimo.sio` |
  |---|---|---|
  | `admissible` | 1 se `5·kx²·s²≤4` (aqui, `(1+kx²)s²≤1`), senão 0 | `admissible(coefficient7)` |
  | `rho_a`, `rho_b`, `rho_c`, `rho_d` | entradas de `rho` pós-atualização | campos de `Mat2` após `update(...)` |
  | `det` | determinante de `rho` | `determinant(rho)` |
  | `p_Pplus` | `trace(mul(P, rho))` | primeira metade de `PASS code-to-state bridge` |
  | `p_Qplus` | `trace(mul(Q, rho))`, `Q` = `Qp` (projetor `Q+`) | `trace(mul(q,rho_l))` etc. |
  | `pq_Pplus_Qplus`, `pq_Pplus_Qminus`, `pq_Pminus_Qplus`, `pq_Pminus_Qminus` | `prob(rho, P_i, Q_j)` nas quatro combinações de sinal | `prob(rho_l,p,q)` etc. |
  | `qp_Qplus_Pplus`, `qp_Qplus_Pminus`, `qp_Qminus_Pplus`, `qp_Qminus_Pminus` | `prob(rho, Q_j, P_i)` — ordem invertida | `prob(rho_l,q,p)` etc. |
  | `marginal_P_after_Q` | soma de `qp_Qplus_Pplus + qp_Qminus_Pplus` | não calculado explicitamente hoje |
  | `qQQ` | `pq_Pplus_Qminus + pq_Pminus_Qplus − qp_Qplus_Pminus − qp_Qminus_Pplus` | `qq_l`/`qq_r` já calculados |

  Para `invalid` (código `r7=9`, sempre rejeitado nos dois regimes testados): apenas
  `admissible` (deve ser 0) e `det_sigma` — o determinante da matriz `sigma(r7,kx)` antes da
  rejeição, hoje não calculado em `exemplo_minimo.sio` porque `update()` já recusa antes.

- Três chaves fora do padrão `<regime>.<codigo>.<campo>`: `r_L7`, `r_R7`, `r_07` — os
  coeficientes de `e₇` já calculados por `seventh(left,3)`, `seventh(right,3)`, `seventh(ab,4)`.

## Tolerância esperada

Todas as frações desta realização têm denominador potência de 2 (`1/16`, `9/32`, `59/256`, …),
logo são exatamente representáveis em `f64`. Uma comparação correta deveria fechar com desvio
absoluto **zero**, não apenas dentro de uma tolerância. Se o comparador reportar desvio não nulo
em algum campo, é sinal de divergência real de cálculo (ordem de operações, arredondamento
intermediário) entre as duas linguagens — exatamente o que o comparador existe para pegar — e não
deve ser descartado como ruído numérico.

## Como fechar isto

1. Estender `exemplo_minimo.sio` (ou uma cópia) para calcular também o regime `sym` e o caso
   `invalid` explicitamente, e para escrever os 115 valores num arquivo no formato acima.
2. Rodar com `souc` na sua máquina, gerando `sounio_dump.tsv`.
3. `g++ -std=c++23 -O2 scripts/compare_cpp_sounio.cpp -o compare && ./compare cpp_dump.tsv sounio_dump.tsv`
4. Se `COMPARATOR PASS`: o item 5 do ADR009 passa a ser verificado por máquina, não por leitura.
   Se `COMPARATOR FAIL`: o comparador aponta exatamente a chave e o desvio — é o resultado, não
   um bug do comparador (que já foi autotestado nesta sessão contra um caso idêntico e contra um
   caso deliberadamente adulterado, e reagiu corretamente aos dois).

Passo 1 é o único que eu não fiz. Se você colar aqui o retorno do `souc` sobre a impressão
formatada de `f64` (ou apontar onde essa função vive na stdlib), eu escrevo o `.sio` na mesma
sessão em que você pode compilá-lo.
