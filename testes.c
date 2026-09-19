/* Reutiliza as funcoes do programa sem executar o menu principal. */
#define main mainEstacao
#include "estacao.c"
#undef main
#include <assert.h>

static void testarBusca(void) {
    Sessao sessoes[] = {{3, 30, 3, 10, 1, 30},
                       {1, 10, 1, 10, 1, 10},
                       {2, 20, 2, 10, 1, 20}};
    assert(buscaLinear(sessoes, 0, 1) == -1);
    assert(buscaLinear(sessoes, 3, 3) == 0);
    assert(buscaLinear(sessoes, 3, 2) == 2);
    assert(buscaLinear(sessoes, 3, 99) == -1);
}

static void testarOrdenacao(void) {
    const Sessao original[] = {{1, 30, 2, 15, 1, 30},
                              {2, 10, 3, 10.0 / 3, 4, 40},
                              {3, 20, 1, 20, 1, 20}};
    const int esperados[4][3] = {{1, 2, 3}, {2, 3, 1}, {3, 1, 2}, {3, 1, 2}};
    for (int criterio = 1; criterio <= 4; ++criterio) {
        Sessao copia[3];
        memcpy(copia, original, sizeof copia);
        insertionSort(copia, 3, criterio);
        for (int i = 0; i < 3; ++i) {
            assert(copia[i].id == esperados[criterio - 1][i]);
            const Sessao *referencia = &original[copia[i].id - 1];
            assert(copia[i].energia == referencia->energia);
            assert(copia[i].tempo == referencia->tempo);
            assert(copia[i].potencia == referencia->potencia);
            assert(copia[i].tarifa == referencia->tarifa);
            assert(copia[i].custo == referencia->custo);
            assert(buscaLinear(copia, 3, copia[i].id) == i);
        }
        insertionSort(copia, 3, criterio);
        for (int i = 0; i < 3; ++i) assert(copia[i].id == esperados[criterio - 1][i]);
    }
    Sessao iguais[] = {{2, 10, 1, 10, 1, 10}, {1, 10, 1, 10, 1, 10}};
    insertionSort(iguais, 2, 2);
    assert(iguais[0].id == 2 && iguais[1].id == 1);
    insertionSort(iguais, 0, 1);
    insertionSort(iguais, 1, 1);
    assert(iguais[0].id == 2);
    Sessao inversas[MAX_SESSOES];
    for (int i = 0; i < MAX_SESSOES; ++i) inversas[i] = (Sessao){.id = MAX_SESSOES - i};
    insertionSort(inversas, MAX_SESSOES, 1);
    for (int i = 0; i < MAX_SESSOES; ++i) assert(inversas[i].id == i + 1);
}

int main(void) {
    testarBusca();
    testarOrdenacao();
    puts("Todos os testes de busca e ordenacao passaram.");
    return 0;
}
