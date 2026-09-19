# Estação de Recarga

Projeto acadêmico em **C11** para gerenciar até 100 sessões de recarga em um vetor de estruturas. Inclui cadastro, listagem, busca por ID, ordenação manual e estatísticas.

## Executar no VS Code (Windows)

1. Baixe ou clone o repositório e abra a pasta `estacao-recarga` pelo menu **Arquivo > Abrir Pasta** do VS Code. Abra a pasta que contém `estacao.c` e `.vscode`.
2. O compilador GCC deve estar no PATH. Neste computador ele está instalado em `C:\msys64\mingw64\bin`. No terminal, `gcc --version` confirma se está disponível. Se necessário, acrescente essa pasta ao PATH do Windows e reabra o VS Code.
3. Selecione **Terminal > Executar Tarefa > Executar estacao de recarga**. A tarefa compila o arquivo C e abre o programa no terminal integrado.
4. Digite a opção desejada e pressione Enter. Escolha `6` para encerrar.

`Ctrl+Shift+B` apenas compila. Para testar os algoritmos, use **Terminal > Executar Tarefa > Executar testes C**. Essas tarefas não exigem extensões nem Python. Não use “compilar todos os arquivos .c juntos”, pois o programa e os testes têm entradas próprias.

Também é possível executar no terminal do VS Code:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror estacao.c -o estacao.exe -lm
.\estacao.exe
```

Para os testes:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror testes.c -o testes.exe -lm
.\testes.exe
```

Em Linux ou macOS com GCC instalado, compile com `-o estacao` e execute `./estacao`. As tarefas fornecidas são voltadas ao Windows.

## Exemplo de uso

Cadastre uma sessão com potência de `10` kW, tempo de `2` horas e tarifa de `1,50` R$/kWh. O programa registra **20 kWh** e **R$ 30,00**, atribuindo automaticamente o ID 1. Consulte esse ID pela opção 3 e veja os totais pela opção 5.

O menu permite ordenar por ID, energia, custo ou tempo, sempre em ordem crescente. Entradas decimais aceitam ponto ou vírgula, sem separador de milhar. A saída utiliza ponto decimal e texto sem acentos para facilitar a exibição em terminais Windows.

As sessões ficam na memória durante a execução e são descartadas ao encerrar. A simulação considera potência constante, sem perdas nem curva de carregamento.

## Arquivos

- `estacao.c`: programa completo em C.
- `testes.c`: testes em C de busca, ordenação e preservação dos registros.
- `RELATORIO.md`: estrutura, funcionamento, algoritmos e análise Big-O.
- `ROTEIRO_TESTES.md`: cenários de validação e resultados esperados.
- `.vscode/tasks.json`: tarefas para compilar, executar e testar.

Os executáveis são gerados localmente e não entram nos commits.
