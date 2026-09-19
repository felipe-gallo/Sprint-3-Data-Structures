# Relatório da sprint — Sistema de Gerenciamento de Estação de Recarga

## 1. Objetivo e escopo

O sistema registra e manipula múltiplas sessões de recarga usando os fundamentos de C: `typedef`, `struct`, vetor de estruturas, funções, busca e ordenação. Todos os recursos do programa estão em `estacao.c` e usam a biblioteca padrão de C. O armazenamento é em memória, limitado a 100 sessões por execução; persistência em arquivo não faz parte desta entrega.

## 2. Estrutura criada

```c
typedef struct {
    int id;
    double energia;
    double tempo;
    double potencia;
    double tarifa;
    double custo;
} Sessao;
```

`struct` reúne os dados de uma sessão e `typedef` permite usar o nome `Sessao` diretamente. O ID identifica o registro; energia é medida em kWh, tempo em horas, potência em kW, tarifa em R$/kWh e custo em reais. Os valores reais usam `double` para maior precisão que `float`; a exibição arredonda para duas casas decimais, mas os cálculos mantêm a precisão disponível. Trata-se de uma simulação acadêmica, sem regras contábeis de arredondamento por transação.

Na função principal, `Sessao sessoes[MAX_SESSOES]` reserva o vetor e `quantidade` controla quantas posições estão ocupadas. Somente os índices de zero até `quantidade - 1` são consultados. O cadastro verifica a capacidade antes de inserir.

O ID é `quantidade + 1`. Como não há exclusão de registros, isso mantém IDs únicos mesmo depois de ordenar o vetor. A ordenação muda a posição de cada estrutura, não o seu ID.

## 3. Funcionamento e funções

O menu se repete até a opção 6 ou até o fim da entrada. As operações são:

| Função | Responsabilidade |
|---|---|
| `lerNumero` | Ler uma linha, validar número e faixa, aceitar ponto ou vírgula e detectar fim da entrada |
| `cadastrarSessao` | Validar capacidade, receber dados, calcular energia/custo e inserir o registro |
| `mostrarSessao` e `listarSessoes` | Exibir os campos e percorrer os registros ocupados |
| `buscaLinear` e `buscarSessao` | Localizar um ID e apresentar o resultado ou a ausência |
| `valorOrdenacao` | Selecionar o campo comparado |
| `insertionSort` e `ordenarSessoes` | Ordenar manualmente e mostrar o vetor resultante |
| `mostrarEstatisticas` | Acumular totais e identificar os extremos |

O cadastro considera potência constante:

**energia = potência × tempo**

**custo = energia × tarifa**

Os limites didáticos são potência de 0,01 a 1000 kW, tempo de 0,01 a 168 horas e tarifa de 0 a 100 R$/kWh. Tarifa zero permite simular recargas gratuitas. Esses limites são escolhas do projeto, não exigências do enunciado.

Textos, números fora da faixa, valores não finitos, opções fracionárias e entradas longas são rejeitados. Um cadastro só modifica o vetor depois de receber todos os campos válidos; fim da entrada durante o cadastro não insere uma sessão incompleta.

As estatísticas apresentam quantidade, soma das energias, soma dos custos, custo médio, maior energia e menor energia. O custo médio é faturamento dividido pela quantidade. Sem sessões, os totais são zero e os extremos são indisponíveis; não ocorre divisão por zero. Maior e menor são inicializados com a primeira sessão quando ela existe.

## 4. Busca linear e análise Big-O

Trecho da função `buscaLinear`:

```c
for (int i = 0; i < quantidade; ++i) {
    if (sessoes[i].id == id) return i;
}
return -1;
```

Para `n = quantidade`, cada passagem compara um ID. Se o ID estiver na última posição ou não existir, o laço faz `n` comparações. Portanto, o pior caso é **O(n)**. Se estiver na primeira posição, a função retorna na primeira comparação: melhor caso **O(1)**. Considerando buscas bem-sucedidas igualmente prováveis entre as posições, o número médio de comparações é `(n + 1) / 2`, ou **O(n)**.

A busca retorna o índice encontrado ou `-1`. Usa apenas um índice e não cria outro vetor, portanto o espaço auxiliar é **O(1)**. Foi escolhida porque funciona qualquer que seja a ordenação atual; não exige ordenar por ID antes da consulta.

## 5. Insertion Sort e análise Big-O

Trecho da função `insertionSort`:

```c
for (int i = 1; i < quantidade; ++i) {
    Sessao atual = sessoes[i];
    int j = i - 1;
    while (j >= 0 && valorOrdenacao(&sessoes[j], criterio) >
                     valorOrdenacao(&atual, criterio)) {
        sessoes[j + 1] = sessoes[j];
        --j;
    }
    sessoes[j + 1] = atual;
}
```

A cada iteração, o prefixo anterior está ordenado. O algoritmo guarda a sessão atual, desloca as sessões maiores para a direita e insere o registro na posição correta. Ao terminar, todo o vetor está ordenado pelo critério escolhido. Não utiliza `qsort` nem outra ordenação pronta.

No pior caso, um vetor em ordem inversa exige 1 deslocamento na primeira inserção, 2 na segunda, até `n - 1` na última. A soma é:

**1 + 2 + … + (n − 1) = n(n − 1)/2**

O termo dominante é quadrático: pior caso **O(n²)**. Para uma ordem inicial aleatória, o caso médio também é **O(n²)**. No melhor caso, já ordenado, o `while` não desloca registros e há somente as `n - 1` verificações do laço externo: **O(n)**.

O algoritmo guarda uma estrutura temporária e índices: espaço auxiliar **O(1)**. A comparação estrita `>` mantém a ordem relativa das sessões com valores iguais, tornando a ordenação estável. Cada deslocamento copia a estrutura inteira, preservando a associação entre ID, energia, tempo e custo.

Embora esta implementação limite `n` a 100, a análise expressa como o trabalho cresce em função da quantidade de sessões, caso a capacidade fosse ampliada. A função `valorOrdenacao` e a cópia de uma estrutura de tamanho fixo têm custo constante.

## 6. Validação

O programa e os testes foram compilados como C11 com `-Wall -Wextra -Wpedantic -Werror`, sem avisos. Os testes em C verificam busca, os quatro critérios, registros preservados, empates estáveis, vetor vazio, uma sessão, vetor já ordenado e 100 elementos em ordem inversa.

Também foram executadas entradas de menu para conferir estatísticas conhecidas, cadastro após ordenação, busca ausente, tarifa zero, separador decimal com vírgula, entradas inválidas, limite de capacidade, linhas longas e encerramento por fim da entrada. O roteiro acompanha o projeto para reprodução manual.
