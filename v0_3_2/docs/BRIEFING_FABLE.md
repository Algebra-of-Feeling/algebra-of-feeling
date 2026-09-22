# Briefing de revisão profunda — Álgebra do Sentir v0.3.2

Para colar no Fable, junto com o PDF (ou o .md) do preprint e, se possível, o suplemento executável.

## Contexto em três linhas

Manuscrito teórico e computacional que propõe: composição não associativa de eventos relacionais numa álgebra de Cayley–Dickson; estado relacional C como seção de uma fibra sobre a base dos participantes; divisor de zero como anulação relacional; leitura do estado por instrumentos de Lüders. Versão 0.3.2 acrescenta a distinção posição/troca e a identidade dos efeitos de ordem sob decoerência. Alvo imediato: preprint no SSRN; alvo posterior: periódico a definir.

## O que já foi verificado, e como

- Associador octoniônico 2e₇ e par sedeniônico de produto nulo: provas Lean 4 com `decide` (kernel), sem `native_decide`.
- Separação associador × defeito operacional: `shifted_frozen_associative`, `associative_contexts_nonzero_operational`, `faithful_updates_force_associativity`.
- Realização de Bloch: C++23 em racionais exatos, reproduzida em plataforma independente com saída idêntica byte a byte; executada também em Sounio.
- Δ_Q = λx/2 e Δ_P = z/2: derivadas e verificadas em racionais exatos. **Sem prova Lean.**
- Auditoria de axiomas: `Classical.choice`, `Quot.sound`, `propext`, com controle negativo executado sobre o portão de admissão de fontes.

## Pontos frágeis conhecidos — é aqui que a revisão profunda rende mais

1. **Δ como teorema.** A identidade dos efeitos de ordem é candidata natural a teorema no núcleo RCDA. Vale formalizar para \(2\times2\) reais, ou já no caso complexo com fase? Qual enunciado é o mais forte que ainda é curto?

2. **O experimento que decide.** A §5 propõe separar o modelo do associador do modelo associativo com operador de espera W, manipulando o agrupamento por enquadramento com intervalo fixo. A pergunta dura: existe algum modelo associativo com estado ampliado (memória, contexto, canal dependente do enquadramento) que reproduza o resultado previsto? Se existir, o experimento não decide, e a §7 precisa mudar.

3. **Base de posição.** Propõe-se escolher a base pela estabilidade da atribuição de lugar, no espírito da einseleção. Isso é suficiente para fixar a base de modo não arbitrário, ou é preciso um critério operacional mais forte? Sem base fixada, "coerência" não tem referente.

4. **Complementaridade das perguntas.** A predição de dissociação supõe que a pergunta de posição e a de troca sejam complementares. Isso é propriedade empírica dos itens. Como testá-la sem circularidade?

5. **Divisores de zero e frequência.** Fora do texto, há um resultado calculado: pares aleatórios de sedênios unitários quase nunca têm produto pequeno (mínimo 0,44 em 10⁶ amostras); já entre os 210 elementos esparsos e₍ᵢ₎±e₍ⱼ₎, 84 têm parceiro anulador, com 336 pares ordenados e 168 não ordenados. Isso sugere que a anulação só é frequente se os estados vividos forem esparsos e equilibrados. **Essa hipótese não está no manuscrito.** Ela merece entrar, e com que estatuto?

6. **Os dois níveis.** Todos os 42 pares de índices dos divisores primitivos combinam um índice octoniônico (1–7) com um da metade da duplicação (8–15). A leitura fenomenológica proposta (nível reflexivo) depende de essa divisão ser invariante. Os automorfismos dos sedênios preservam essa decomposição? Se não, a leitura precisa ser reformulada.

7. **Fragilidade.** Anulações por divisor de zero são estruturalmente frágeis (o conjunto tem dimensão menor). Isso entra em tensão com qualquer tese de irreversibilidade forte. A formulação atual da §7 está correta, ou ainda concede demais?

8. **Originalidade da leitura posição/troca.** Busca rápida não encontrou modelo quântico de díades com essas duas componentes. A busca não foi sistemática. Vale um levantamento adversarial: quem já fez isso, em cognição quântica, em psicometria diádica ou em modelagem de apego?

## Material que existe fora do manuscrito e ainda não foi incorporado

- Taxonomia de fins de relação em três vias: decaimento, repulsão crescente e divisor de zero, com a quarta possibilidade de ordem de ligação nula (ambivalência).
- Distinção entre afeto dirigido à pessoa (componente ligante) e dirigido à saída (barreira), que reencontra o modelo de coesão de Levinger.
- Definição de compatibilidade de lugares por existência de um ato compartilhado, com a consequência de que pares incompatíveis não têm fonte de troca.
- Setores tipo dupla contabilidade em B, com a predição de que efeitos de ordem entre setores desaparecem sob superseleção.

Pergunta para o Fable: algo disso deveria entrar já na v0.3.2, ou é material de um segundo artigo? Se entrar, entra como hipótese com condição de inadequação, ou só como discussão?

## O que não pedir

Revisão de vírgula, formatação e referências já foi feita. Os números do manuscrito já foram recalculados e não precisam de recálculo em Python — a regra do projeto (ADR009) é C++23, F#, F*, Futhark ou Koka para cálculo e Lean 4 para teorema.
