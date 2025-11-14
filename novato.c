#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------
// Estrutura da sala (nó da árvore)
// ------------------------------------------------------------
typedef struct Sala {
    char nome[50];           // Nome do cômodo
    struct Sala *esq;        // Caminho à esquerda
    struct Sala *dir;        // Caminho à direita
} Sala;

// ------------------------------------------------------------
// Função criarSala()
// Cria dinamicamente uma sala com um nome e retorna seu ponteiro
// ------------------------------------------------------------
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

// ------------------------------------------------------------
// Função explorarSalas()
// Permite o jogador navegar pela árvore binária
// ------------------------------------------------------------
void explorarSalas(Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: **%s**\n", atual->nome);

        // Se não houver mais caminhos, termina a exploração
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Você chegou a um cômodo sem saídas. Fim da exploração!\n");
            return;
        }

        printf("Escolha o caminho:\n");
        if (atual->esq != NULL) printf("  (e) - Ir para %s (esquerda)\n", atual->esq->nome);
        if (atual->dir != NULL) printf("  (d) - Ir para %s (direita)\n", atual->dir->nome);
        printf("  (s) - Sair da mansão\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esq != NULL) {
            atual = atual->esq;
        } 
        else if (escolha == 'd' && atual->dir != NULL) {
            atual = atual->dir;
        } 
        else if (escolha == 's') {
            printf("Saindo da mansão...\n");
            return;
        } 
        else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

// ------------------------------------------------------------
// Função main()
// Monta manualmente o mapa da mansão e inicia a exploração
// ------------------------------------------------------------
int main() {

    // Construção da árvore (mansão)
    //
    //                Hall de Entrada
    //                 /            \
    //         Sala de Estar      Cozinha
    //           /      \          /     \
    //     Biblioteca  Jardim  Despensa  Sala Secreta

    Sala *hall        = criarSala("Hall de Entrada");
    Sala *estar       = criarSala("Sala de Estar");
    Sala *cozinha     = criarSala("Cozinha");
    Sala *biblioteca  = criarSala("Biblioteca");
    Sala *jardim      = criarSala("Jardim");
    Sala *despensa    = criarSala("Despensa");
    Sala *secreta     = criarSala("Sala Secreta");

    // Ligações da árvore
    hall->esq = estar;
    hall->dir = cozinha;

    estar->esq = biblioteca;
    estar->dir = jardim;

    cozinha->esq = despensa;
    cozinha->dir = secreta;

    // Iniciar exploração
    printf("===== Detective Quest: Exploração da Mansão =====\n");
    explorarSalas(hall);

    return 0;
}
