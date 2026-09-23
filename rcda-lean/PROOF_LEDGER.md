# Proposições citadas no manuscrito

O catálogo autoritativo completo é `RCDAAudit.catalogue`, em `Audit.lean`. Este mapa destaca a pendência levantada na revisão; não redefine a contagem de resultados matemáticos originais.

| Manuscrito | Declaração em RCDA.RelationalComposition | Fonte e alcance |
|---|---|---|
| §3 | `shifted_frozen_associative` | `RCDA/RelationalComposition.lean`: associatividade de x+y+C com C fixo, para inteiros arbitrários. |
| §3 | `associative_contexts_nonzero_operational` | Mesmo arquivo: defeito operacional igual a 1 para o protocolo explicitamente definido e entradas 0,1,2,3. |
| §5 | `faithful_updates_force_associativity` | Mesmo arquivo: uma realização fiel e preservadora do produto por funções força associatividade. |

As três entradas são [P], importadas por `RCDA.lean`, presentes no catálogo de `Audit.lean` e exigidas pelo verificador C++23 antes da emissão do PASS. O log de axiomas identifica suas dependências lógicas transitivas.

`RCDA.concrete_octonion_associator_value` e `RCDA.concrete_annihilation` estão em `RCDA/Concrete.lean`: utilizam `decide`, com redução conferida pelo kernel. A segunda proposição inclui a não nulidade dos dois fatores, além do produto zero. Não se confunde aceitação desses enunciados com prova de interpretação clínica.

`Transport`, em `RCDA/Bundle.lean`, é ação de grupoide sobre família de fibras. Não é uma implementação de fibrado diferenciável. A irrecuperabilidade de toda relação pós-ruptura permanece [H], não consequência da impossibilidade de inversão universal de uma multiplicação.
