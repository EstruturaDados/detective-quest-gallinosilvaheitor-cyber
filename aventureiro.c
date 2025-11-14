#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------
// Estrutura para representar cada sala da mansão
// (nó da árvore binária do mapa)
// ------------------------------------------------------------
typedef struct Sala {
    char nome[50];
    char pista[100];   // pista opcional
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// ------------------------------------------------------------
// Estrutura da árvore BST para armazenar pistas coletadas
// ------------------------------------------------------------
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

// ------------------------------------------------------------
// Função criarSala()
// Cria dinamicamente uma sala com nome e pista opcional.
// ------------------------------------------------------------
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro de memória ao criar sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

// ------------------------------------------------------------
// Função inserirPista()
// Insere uma pista na BST em ordem alfabética.
// ------------------------------------------------------------
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *nova = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(nova->pista, pista);
        nova->esq = nova->dir = NULL;
        return nova;
    }
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esq = inserirPista(raiz->esq, pista);
    } else {
        raiz->dir = inserirPista(raiz->dir, pista);
    }
    return raiz;
}

// ------------------------------------------------------------
// Função exibirPistas()
// Percorre a BST em ordem e exibe todas as pistas coletadas.
// ------------------------------------------------------------
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esq);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

// ------------------------------------------------------------
// Função explorarSalasComPistas()
// Controla a exploração, coleta pistas automaticamente,
// e insere na BST.
// ------------------------------------------------------------
void explorarSalasComPistas(Sala *atual, PistaNode **pistas) {
    char opcao;

    while (1) {
        printf("\nVocê está em: **%s**\n", atual->nome);

        // Se a sala tiver uma pista, coleta automaticamente
        if (strlen(atual->pista) > 0) {
            printf(">> Você encontrou uma pista: \"%s\"\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        } else {
            printf(">> Nenhuma pista neste cômodo.\n");
        }

        // Opções de navegação
        printf("\nAções disponíveis:\n");
        if (atual->esq != NULL) 
            printf("  (e) Ir para %s\n", atual->esq->nome);
        if (atual->dir != NULL) 
            printf("  (d) Ir para %s\n", atual->dir->nome);

        printf("  (s) Sair da mansão\n");
        printf("Escolha: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esq != NULL) {
            atual = atual->esq;
        } 
        else if (opcao == 'd' && atual->dir != NULL) {
            atual = atual->dir;
        } 
        else if (opcao == 's') {
            printf("\nExploração encerrada.\n");
            return;
        } 
        else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

// ------------------------------------------------------------
// Função main()
// Monta o mapa fixo da mansão, inicia exploração e mostra pistas.
// ------------------------------------------------------------
int main() {
    // ----------------- Criação da Mansão (Árvore Binária) -----------------

    Sala *hall        = criarSala("Hall de Entrada", "Pegada de lama");
    Sala *estar       = criarSala("Sala de Estar", "Copo quebrado");
    Sala *cozinha     = criarSala("Cozinha", "Faca fora do lugar");
    Sala *biblioteca  = criarSala("Biblioteca", "Livro arrancado");
    Sala *jardim      = criarSala("Jardim", "");
    Sala *despensa    = criarSala("Despensa", "Lata caída");
    Sala *secreta     = criarSala("Sala Secreta", "Documento misterioso");

    // Ligações da árvore da mansão
    hall->esq = estar;
    hall->dir = cozinha;

    estar->esq = biblioteca;
    estar->dir = jardim;

    cozinha->esq = despensa;
    cozinha->dir = secreta;

    // ----------------- Exploração -----------------
    PistaNode *pistas = NULL;

    printf("===== Detective Quest: Coleta de Pistas =====\n");
    explorarSalasComPistas(hall, &pistas);

    // ----------------- Exibição das Pistas Coletadas -----------------
    printf("\n===== Pistas coletadas (ordem alfabética) =====\n");
    if (pistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistas);
    }

    return 0;
}
