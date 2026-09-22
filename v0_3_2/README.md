# v0_3_2 — Álgebra do Sentir

Versão 0.3.2 do manuscrito, em português e em inglês britânico, com as figuras, o material de verificação e a documentação de apoio. Sucede `v0_3_1/`, que fica preservada.

## O que mudou em relação à v0.3.1

- **Nova subseção na §5:** posição e troca como leituras do estado relacional, com a identidade \(\Delta_Q=\lambda x/2\) e \(\Delta_P=z/2\), a predição de dissociação e o experimento de enquadramento que separa a composição não associativa de modelos associativos com operador de espera.
- **Nova proposição na §3:** a anulação atravessa os dois níveis da duplicação. Nenhum elemento com uma metade nula é divisor de zero.
- **Pendência (a) fechada:** a frase sobre degradação após reconciliação ganhou condição dinâmica (exige termo de perda) e registra a fragilidade estrutural das anulações por divisor de zero.
- **Referências conferidas:** Dubois & Toffano 2026, Wang et al. 2014, Zurek 2003. Acrescentadas Blutner & beim Graben 2014 e Kenny & La Voie 1984.
- **Quatro figuras vetoriais**, nas duas línguas, no lugar do diagrama em mermaid.
- **Duas linhas novas** na tabela de condições de inadequação da §7.
- **Controle negativo** do portão de admissão do núcleo Lean, executado.

## Pastas

| Pasta | Conteúdo |
|---|---|
| `pt/` | Manuscrito em português, em `.md` e `.pdf`, e as figuras `fig0`–`fig3` |
| `en/` | Manuscrito em inglês britânico e as figuras `fig0_en`–`fig3_en` |
| `cpp/` | Realização e verificações em C++23 |
| `lean/` | `Levels.lean` (rascunho) e o verificador com o controle negativo |
| `docs/` | README do Zenodo, briefing do Fable, ata de revisão, nota de fechamento, especificação do despejo Sounio |
| `logs/` | Saídas de execução e o despejo canônico C++ |

Os `.md` só compilam com as figuras na mesma pasta:

```sh
pandoc pt/ALGEBRA_DO_SENTIR_v0_3_2_SSRN.md -o saida.pdf --pdf-engine=xelatex \
  -f markdown+tex_math_single_backslash -V mainfont="DejaVu Serif" \
  -V mathfont="DejaVu Math TeX Gyre" -V geometry:margin=2.5cm -V lang=pt-BR
```

## Estado da verificação

| Item | Estado |
|---|---|
| Associador \(2e_7\) e par sedeniônico de produto nulo | Prova Lean por `decide`, no núcleo auditado |
| Separação associador × defeito operacional | Prova Lean |
| Realização de Bloch e instrumentos | C++23 em racionais exatos, reproduzida em plataforma independente, byte a byte |
| \(\Delta_Q=\lambda x/2\), \(\Delta_P=z/2\) | Prova em prosa e verificação exata em C++23; **sem prova Lean** |
| Travessia de níveis | Prova em prosa e verificação em C++23; `lean/Levels.lean` é **rascunho não compilado** |
| Concordância C++ × Sounio | Comparador pronto e autotestado; despejo do lado Sounio **pendente** |

## Pendências

1. Rodar `sh scripts/verify.sh` no núcleo com `Levels.lean` incluído e trazer o resultado.
2. Depositar o suplemento no Zenodo e substituir o marcador de DOI nos dois manuscritos.
3. Passada do Fable, com `docs/BRIEFING_FABLE.md`.
4. Gerar o despejo do lado Sounio, conforme `docs/ESPECIFICACAO_DESPEJO_SOUNIO.md`, e rodar o comparador.

Os cálculos seguem o ADR009: C++23, F#, F*, Futhark ou Koka para cálculo, Lean 4 para teorema. Não há código Python nesta pasta.
