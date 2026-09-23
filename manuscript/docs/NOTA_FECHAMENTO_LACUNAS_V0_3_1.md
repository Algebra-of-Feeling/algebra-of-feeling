# Fechamento de lacunas — pós-ata de revisão independente de 20/09/2026

A ata de revisão independente do pacote v0.3.1 identificou duas lacunas. Esta correção fecha a
primeira por completo, com execução real nesta sessão, e entrega a ferramenta pronta para a
segunda — cuja etapa final depende de `souc`, que não está disponível neste ambiente.

## Lacuna 1 — controle negativo do portão de admissão Lean: FECHADA

`nucleo-lean/scripts/negative_control.cpp` (novo) testa o mesmo portão de `verify.cpp` — agora
extraído para `nucleo-lean/scripts/source_gate.hpp`, compartilhado pelos dois, para que nunca
divirjam. Compilado e **executado de verdade** nesta sessão, sem precisar de Lean (a checagem é
textual, anterior a qualquer `lake build`):

- Controle positivo: `RCDA/RelationalComposition.lean` sem mutação → aceito.
- Quatro controles negativos, cada um uma mutação em memória (nenhum arquivo real tocado): token
  `sorry` numa prova real, `native_decide` no lugar de `decide` num teorema real, uma declaração
  `axiom` anexada, um `import` não aprovado → **os quatro rejeitados**, com a mensagem de erro
  exata que `verify.cpp` já usava.
- `verify.cpp` foi atualizado para recusar-se a emitir PASS se `verification/negative-control.log`
  não existir ou não registrar `NEGATIVE CONTROL SUITE PASS`. Testei isso também: rodei
  `scripts/verify.sh` de ponta a ponta e ele parou exatamente no ponto esperado — depois do
  controle negativo passar, ao tentar `lake env lean --version` (que falha aqui por falta de
  `lake`, não por causa desta correção).

Evidência completa em `nucleo-lean/verification/negative-control.log`, gerado nesta sessão.

**Pendente na sua máquina:** rodar `sh scripts/verify.sh` com Lean 4.33.0 instalado, para obter
um `receipt.json` novo que já nasce com o controle negativo como pré-requisito satisfeito. O
`receipt.json` atual do pacote v0.3.1 é anterior a esta correção e não reflete essa exigência.

## Lacuna 2 — comparador automatizado C++/Sounio: FERRAMENTA PRONTA, COMPARAÇÃO REAL PENDENTE

`realizacao/dump_bloch.cpp` e `realizacao/compare_cpp_sounio.cpp` (novos), mais
`realizacao/bloch_core.hpp` (cálculo extraído de `cd_bloch.cpp`, comportamento conferido byte a
byte antes e depois da extração). Nesta sessão:

- Gerei o despejo canônico C++ real: `realizacao/verification/cpp_dump.tsv`, 115 campos.
- Autotestei o comparador: PASS exato contra si mesmo, FAIL corretamente detectado contra uma
  cópia com um campo deliberadamente alterado (`asym.rho_L.det`, desvio 0.01 introduzido).

**Não gerei o despejo do lado Sounio** — não há `souc` neste ambiente, e não tenho visibilidade
da API de impressão formatada de `f64` do Sounio além do que já vi em dois arquivos. Escrever
`.sio` sem poder compilá-lo seria entregar código não verificado, o que este projeto pede
explicitamente para eu não fazer. `ESPECIFICACAO_DESPEJO_SOUNIO.md` define exatamente as 115
chaves; é o que falta para fechar esta lacuna por completo.

## Arquivos entregues nesta correção

```
nucleo-lean/scripts/source_gate.hpp          (novo)
nucleo-lean/scripts/negative_control.cpp     (novo, já executado)
nucleo-lean/scripts/verify.cpp               (atualizado — usa source_gate.hpp; exige o log acima)
nucleo-lean/scripts/verify.sh                (atualizado — compila e roda negative_control antes)
nucleo-lean/README.md                        (atualizado)
nucleo-lean/verification/negative-control.log (novo, execução real desta sessão)
realizacao/bloch_core.hpp                    (novo)
realizacao/cd_bloch.cpp                      (atualizado — mesma saída, extraído para o header)
realizacao/dump_bloch.cpp                    (novo, já executado)
realizacao/compare_cpp_sounio.cpp            (novo, autotestado)
realizacao/LEIA-ME_COMPARADOR.md             (novo)
realizacao/verification/cpp_dump.tsv         (novo, saída real)
realizacao/verification/FECHAMENTO_RECIBO.json (novo)
ESPECIFICACAO_DESPEJO_SOUNIO.md              (novo)
```

Todo o trabalho desta sessão foi em C++23, sem Python, conforme ADR009.
