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

static int cadastrarSessao(Sessao sessoes[], int *quantidade) {
    Sessao nova;
    if (*quantidade == MAX_SESSOES) {
        puts("Limite de 100 sessoes atingido.");
        return 1;
    }
    /* So altera o vetor depois de receber todos os dados validos. */
    if (!lerNumero("Potencia do carregador (kW, 0.01 a 1000): ",
                   0.01, 1000, 0, &nova.potencia) ||
        !lerNumero("Tempo de recarga (horas, 0.01 a 168): ",
                   0.01, 168, 0, &nova.tempo) ||
        !lerNumero("Tarifa (R$/kWh, 0 a 100): ",
                   0, 100, 0, &nova.tarifa)) return 0;
    nova.id = *quantidade + 1;
    nova.energia = nova.potencia * nova.tempo;
    nova.custo = nova.energia * nova.tarifa;
    sessoes[*quantidade] = nova;
    ++*quantidade;
    printf("Sessao %d cadastrada. Energia: %.2f kWh | Custo: R$ %.2f\n",
           nova.id, nova.energia, nova.custo);
    return 1;
}

static void mostrarSessao(const Sessao *sessao) {
    printf("ID: %d | Energia: %.2f kWh | Tempo: %.2f h | "
           "Potencia: %.2f kW | Tarifa: R$ %.2f/kWh | Custo: R$ %.2f\n",
           sessao->id, sessao->energia, sessao->tempo,
           sessao->potencia, sessao->tarifa, sessao->custo);
}

static void listarSessoes(const Sessao sessoes[], int quantidade) {
    if (!quantidade) {
        puts("Nenhuma sessao cadastrada.");
        return;
    }
    puts("\n========= SESSOES =========");
    for (int i = 0; i < quantidade; ++i) mostrarSessao(&sessoes[i]);
}

static int buscaLinear(const Sessao sessoes[], int quantidade, int id) {
    for (int i = 0; i < quantidade; ++i) {
        if (sessoes[i].id == id) return i;
    }
    return -1;
}

static int buscarSessao(const Sessao sessoes[], int quantidade) {
    double id;
    int indice;
    if (!quantidade) {
        puts("Nenhuma sessao cadastrada.");
        return 1;
    }
    if (!lerNumero("Digite o ID da sessao (1 a 100): ", 1, MAX_SESSOES, 1, &id))
        return 0;
    indice = buscaLinear(sessoes, quantidade, (int)id);
    if (indice < 0) puts("Sessao nao encontrada.");
    else mostrarSessao(&sessoes[indice]);
    return 1;
}

static double valorOrdenacao(const Sessao *sessao, int criterio) {
    switch (criterio) {
        case 1: return sessao->id;
        case 2: return sessao->energia;
        case 3: return sessao->custo;
        default: return sessao->tempo;
    }
}

static void insertionSort(Sessao sessoes[], int quantidade, int criterio) {
    for (int i = 1; i < quantidade; ++i) {
        Sessao atual = sessoes[i];
        int j = i - 1;
        /* Move a estrutura inteira para preservar os dados de cada sessao. */
        while (j >= 0 && valorOrdenacao(&sessoes[j], criterio) >
                         valorOrdenacao(&atual, criterio)) {
            sessoes[j + 1] = sessoes[j];
            --j;
        }
        sessoes[j + 1] = atual;
    }
}

static int ordenarSessoes(Sessao sessoes[], int quantidade) {
    double criterio;
    if (!quantidade) {
        puts("Nenhuma sessao cadastrada.");
        return 1;
    }
    puts("Ordenar em ordem crescente por:");
    puts("1 - ID\n2 - Energia\n3 - Custo\n4 - Tempo");
    if (!lerNumero("Criterio: ", 1, 4, 1, &criterio)) return 0;
    insertionSort(sessoes, quantidade, (int)criterio);
    puts("Sessoes ordenadas em ordem crescente.");
    listarSessoes(sessoes, quantidade);
    return 1;
}

static void mostrarEstatisticas(const Sessao sessoes[], int quantidade) {
    double energiaTotal = 0, faturamento = 0;
    double maior = 0, menor = 0;
    if (quantidade) maior = menor = sessoes[0].energia;
    for (int i = 0; i < quantidade; ++i) {
        energiaTotal += sessoes[i].energia;
        faturamento += sessoes[i].custo;
        if (sessoes[i].energia > maior) maior = sessoes[i].energia;
        if (sessoes[i].energia < menor) menor = sessoes[i].energia;
    }
    puts("\n========= ESTATISTICAS =========");
    printf("Sessoes realizadas: %d\n", quantidade);
    printf("Energia fornecida: %.2f kWh\n", energiaTotal);
    printf("Faturamento: R$ %.2f\n", faturamento);
    printf("Ticket medio: R$ %.2f\n", quantidade ? faturamento / quantidade : 0);
    if (quantidade) {
        printf("Maior consumo: %.2f kWh\nMenor consumo: %.2f kWh\n", maior, menor);
    } else puts("Maior e menor consumo: indisponiveis (nenhuma sessao).");
}

int main(void) {
    Sessao sessoes[MAX_SESSOES];
    int quantidade = 0;
    double opcao;
    while (1) {
        puts("\n=====================================");
        puts("         ESTACAO DE RECARGA");
        puts("=====================================");
        puts("1 - Nova sessao de recarga\n2 - Listar sessoes\n3 - Buscar sessao");
        puts("4 - Ordenar sessoes\n5 - Estatisticas\n6 - Encerrar");
        if (!lerNumero("Escolha: ", 1, 6, 1, &opcao) || opcao == 6) break;
        if (opcao == 1) {
            if (!cadastrarSessao(sessoes, &quantidade)) break;
        } else if (opcao == 2) listarSessoes(sessoes, quantidade);
        else if (opcao == 3) {
            if (!buscarSessao(sessoes, quantidade)) break;
        } else if (opcao == 4) {
            if (!ordenarSessoes(sessoes, quantidade)) break;
        } else if (opcao == 5) mostrarEstatisticas(sessoes, quantidade);
    }
    puts("Programa encerrado. As sessoes desta execucao nao sao salvas em arquivo.");
    return 0;
}
