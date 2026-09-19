# Roteiro de testes

Execute cada cenário em uma nova execução quando indicado. Os testes de algoritmos podem ser repetidos pela tarefa **Executar testes C** no VS Code; o resultado deve informar que todos passaram.

## 1. Vetor vazio

Em uma execução nova, use listar, buscar e ordenar. Todas devem informar que não há sessões. Estatísticas devem mostrar quantidade, energia, faturamento e média iguais a zero, sem extremos disponíveis.

## 2. Cálculos e estatísticas

Cadastre estes registros, nessa ordem:

| ID esperado | Potência (kW) | Tempo (h) | Tarifa (R$/kWh) | Energia (kWh) | Custo (R$) |
|---|---:|---:|---:|---:|---:|
| 1 | 10 | 2 | 1,5 | 20 | 30 |
| 2 | 5 | 1 | 2 | 5 | 10 |
| 3 | 20 | 3 | 1 | 60 | 60 |

Estatísticas esperadas: **3 sessões**, **85 kWh**, **R$ 100,00** de faturamento, **R$ 33,33** de média, maior consumo **60 kWh** e menor **5 kWh**. A saída usa ponto decimal.

## 3. Busca e ordenação

Na mesma execução, busque ID 2: deve retornar 5 kWh e R$ 10,00. Busque ID 99: deve informar que não encontrou.

Ordene por energia, custo e tempo: para esses dados, a sequência deve ser **2, 1, 3**. Ordene por ID: a sequência deve ser **1, 2, 3**. Os campos de cada registro devem permanecer associados ao mesmo ID. Repita uma busca após ordenar.

## 4. Cadastro após ordenar

Depois de ordenar por energia, cadastre potência 7, tempo 1 e tarifa zero. Deve gerar **ID 4**, energia **7 kWh** e custo zero. Busque o ID 4. Os totais passam a 4 sessões, 92 kWh, R$ 100,00 e média R$ 25,00.

## 5. Validação de entrada

No menu, tente `abc`, `1.5`, `0` e `7`. Nenhum deve selecionar uma operação. No campo potência, tente `nan`, `inf`, `-1`, `0` e `10x`; o programa deve pedir novamente. Depois use potência 10, tempo 2 e tarifa `1,5`: deve registrar 20 kWh e R$ 30,00. Uma linha de 300 dígitos deve ser descartada por inteiro.

## 6. Capacidade e encerramento

Em uma execução nova, cadastre 100 sessões com potência 1, tempo 1 e tarifa 1. Os IDs devem ir de 1 a 100 e os totais devem ser 100 kWh e R$ 100,00. A tentativa de abrir o cadastro seguinte deve informar o limite sem pedir os campos nem alterar os totais.

A opção 6 deve encerrar. Reiniciar deve mostrar o vetor vazio, pois os dados são mantidos apenas em memória. Fim da entrada, inclusive durante o cadastro, deve encerrar sem ficar repetindo mensagens indefinidamente.
