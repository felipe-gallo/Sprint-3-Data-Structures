#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>

#define MAX_SESSOES 100

/* Energia em kWh, tempo em horas, potencia em kW e valores em reais. */
typedef struct {
    int id;
    double energia;
    double tempo;
    double potencia;
    double tarifa;
    double custo;
} Sessao;

/* Retorna zero no fim da entrada. Linhas longas sao descartadas por inteiro. */
static int lerNumero(const char *mensagem, double minimo, double maximo,
                     int inteiro, double *resultado) {
    char linha[128];
    char *fim;
    double valor;
    for (;;) {
        printf("%s", mensagem);
        fflush(stdout);
        if (!fgets(linha, sizeof linha, stdin)) return 0;
        if (!strchr(linha, '\n') && !feof(stdin)) {
            int caractere;
            while ((caractere = getchar()) != '\n' && caractere != EOF) {}
            puts("Entrada muito longa. Tente novamente.");
            continue;
        }
        /* Aceita virgula ou ponto como separador decimal, sem milhar. */
        for (char *p = linha; *p; ++p) if (*p == ',') *p = '.';
        errno = 0;
        valor = strtod(linha, &fim);
        if (fim == linha) {
            puts("Entrada invalida. Digite um numero.");
            continue;
        }
        while (isspace((unsigned char)*fim)) ++fim;
        if (errno == ERANGE || *fim || !isfinite(valor) ||
            valor < minimo || valor > maximo ||
            (inteiro && valor != floor(valor))) {
            printf("Valor invalido. Informe %sentre %.2f e %.2f.\n",
                   inteiro ? "um inteiro " : "um numero ", minimo, maximo);
            continue;
        }
        *resultado = valor;
        return 1;
    }
}

int main(void) {
    Sessao sessoes[MAX_SESSOES];
    int quantidade = 0;
    double opcao;
    (void)sessoes;
    (void)quantidade;
    while (1) {
        puts("\n=====================================");
        puts("         ESTACAO DE RECARGA");
        puts("=====================================");
        puts("1 - Nova sessao de recarga\n2 - Listar sessoes\n3 - Buscar sessao");
        puts("4 - Ordenar sessoes\n5 - Estatisticas\n6 - Encerrar");
        if (!lerNumero("Escolha: ", 1, 6, 1, &opcao) || opcao == 6) break;
        puts("Operacao em desenvolvimento.");
    }
    puts("Programa encerrado. As sessoes desta execucao nao sao salvas em arquivo.");
    return 0;
}
