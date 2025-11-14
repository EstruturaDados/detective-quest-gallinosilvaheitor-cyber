#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definição do tamanho da Tabela Hash
#define TAMANHO_HASH 10
#define MAX_PISTAS 100 // Tamanho máximo para strings de pistas
#define MAX_SUSPEITO 50 // Tamanho máximo para strings de suspeitos

// --- 1. ESTRUTURAS DE DADOS DA MANSÃO (Árvore Binária) ---

typedef struct Sala {
    char nome[MAX_SUSPEITO];
    char pista[MAX_PISTAS]; // Pista estática associada à sala
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// --- 2. ESTRUTURAS DE DADOS DAS PISTAS COLETADAS (BST) ---

typedef struct PistaColetada {
    char pista[MAX_PISTAS];
    struct PistaColetada *esq;
    struct PistaColetada *dir;
} PistaColetada;

// --- 3. ESTRUTURAS DE DADOS DA ASSOCIAÇÃO PISTA-SUSPEITO (Tabela Hash) ---

// Estrutura para os itens da lista encadeada (encadeamento para colisões)
typedef struct ItemHash {
    char pista[MAX_PISTAS];
    char suspeito[MAX_SUSPEITO];
    struct ItemHash *proximo;
} ItemHash;

// Tabela Hash é um array de ponteiros para ItemHash
ItemHash *tabelaHash[TAMANHO_HASH];

// Ponteiro para a raiz da BST de pistas coletadas
PistaColetada *raizPistas = NULL;

// -------------------------------------------------------------------
// -------------------- FUNÇÕES DE MANSÃO (Árvore Binária) --------------------
// -------------------------------------------------------------------

/**
 * @brief Cria dinamicamente um novo cômodo (nó da Árvore Binária).
 * @param nome O nome exclusivo do cômodo.
 * @param pista A pista estática associada a este cômodo.
 * @return Um ponteiro para a nova Sala criada.
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        perror("Erro ao alocar memoria para Sala");
        exit(EXIT_FAILURE);
    }
    strncpy(novaSala->nome, nome, MAX_SUSPEITO - 1);
    novaSala->nome[MAX_SUSPEITO - 1] = '\0';
    strncpy(novaSala->pista, pista, MAX_PISTAS - 1);
    novaSala->pista[MAX_PISTAS - 1] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// -------------------------------------------------------------------
// -------------------- FUNÇÕES DE PISTAS (BST) --------------------
// -------------------------------------------------------------------

/**
 * @brief Insere uma pista coletada na Árvore de Busca Binária (BST).
 * Garante a ordenação alfabética das pistas.
 * @param raiz O ponteiro para a raiz da BST atual.
 * @param pista A string da pista a ser inserida.
 * @return O ponteiro para a nova raiz (após a inserção).
 */
PistaColetada* inserirPista(PistaColetada *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaColetada *novaPista = (PistaColetada*)malloc(sizeof(PistaColetada));
        if (novaPista == NULL) {
            perror("Erro ao alocar memoria para PistaColetada");
            return NULL;
        }
        strncpy(novaPista->pista, pista, MAX_PISTAS - 1);
        novaPista->pista[MAX_PISTAS - 1] = '\0';
        novaPista->esq = NULL;
        novaPista->dir = NULL;
        return novaPista;
    }

    // Compara as strings para determinar se vai para a esquerda ou direita
    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao < 0) {
        raiz->esq = inserirPista(raiz->esq, pista);
    } else if (comparacao > 0) {
        raiz->dir = inserirPista(raiz->dir, pista);
    }
    // Se comparacao == 0, a pista já existe, não faz nada.

    return raiz;
}

/**
 * @brief Exibe todas as pistas coletadas em ordem (BST percorrida "Em Ordem").
 * @param raiz O ponteiro para a raiz da BST de pistas.
 */
void listarPistasColetadas(PistaColetada *raiz) {
    if (raiz != NULL) {
        listarPistasColetadas(raiz->esq);
        printf(" -> %s\n", raiz->pista);
        listarPistasColetadas(raiz->dir);
    }
}

// -------------------------------------------------------------------
// -------------------- FUNÇÕES DE ASSOCIAÇÃO (Tabela Hash) --------------------
// -------------------------------------------------------------------

/**
 * @brief Função de hash simples para strings.
 * Converte a pista em um índice numérico.
 * @param chave A string da pista.
 * @return O índice do bucket na tabela hash.
 */
int funcaoHash(const char *chave) {
    int valor = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        valor = (valor * 31 + chave[i]) % TAMANHO_HASH;
    }
    return valor;
}

/**
 * @brief Insere a associação pista/suspeito na tabela hash.
 * @param pista A string da pista (chave).
 * @param suspeito A string do suspeito (valor).
 */
void inserirNaHash(const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);

    // Cria um novo item
    ItemHash *novoItem = (ItemHash*)malloc(sizeof(ItemHash));
    if (novoItem == NULL) {
        perror("Erro ao alocar memoria para ItemHash");
        return;
    }
    strncpy(novoItem->pista, pista, MAX_PISTAS - 1);
    novoItem->pista[MAX_PISTAS - 1] = '\0';
    strncpy(novoItem->suspeito, suspeito, MAX_SUSPEITO - 1);
    novoItem->suspeito[MAX_SUSPEITO - 1] = '\0';

    // Insere no início da lista encadeada (tratamento de colisão)
    novoItem->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoItem;
}

/**
 * @brief Consulta o suspeito correspondente a uma pista.
 * @param pista A string da pista (chave).
 * @return A string do suspeito ou NULL se não for encontrada.
 */
const char* encontrarSuspeito(const char *pista) {
    int indice = funcaoHash(pista);
    ItemHash *atual = tabelaHash[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Suspeito encontrado
        }
        atual = atual->proximo;
    }

    return NULL; // Pista não encontrada na hash
}

// -------------------------------------------------------------------
// -------------------- FUNÇÕES DE LÓGICA DE JOGO --------------------
// -------------------------------------------------------------------

// Variável global para armazenar o número total de pistas coletadas
int totalPistasColetadas = 0;

/**
 * @brief Função recursiva para navegar pela árvore e coletar pistas.
 * @param salaAtual O ponteiro para a Sala atual que o jogador está explorando.
 */
void explorarSalas(Sala *salaAtual) {
    if (salaAtual == NULL) {
        return;
    }

    printf("\n--- Você está no cômodo: **%s** ---\n", salaAtual->nome);

    if (salaAtual->pista[0] != '\0') {
        printf("🔍 Você encontrou uma **Pista**!\n");
        printf("   Pista: \"%s\"\n", salaAtual->pista);

        // Define a associação Suspeito/Pista dinamicamente baseada na pista
        const char *suspeito = "Desconhecido"; // Default

        // Lógica Simplificada: Associa Pistas a Suspeitos (Requisito: regras codificadas)
        if (strstr(salaAtual->pista, "cigarro") || strstr(salaAtual->pista, "vinho")) {
            suspeito = "Alfredo";
        } else if (strstr(salaAtual->pista, "cabelo loiro") || strstr(salaAtual->pista, "carta")) {
            suspeito = "Berta";
        } else if (strstr(salaAtual->pista, "faca") || strstr(salaAtual->pista, "sapato sujo")) {
            suspeito = "Carlos";
        }

        // Armazena a pista na BST (ordenada)
        raizPistas = inserirPista(raizPistas, salaAtual->pista);

        // Armazena a associação Pista-Suspeito na Tabela Hash
        inserirNaHash(salaAtual->pista, suspeito);
        
        totalPistasColetadas++;
        printf("   **Pista Coletada e Associada a: %s**\n", suspeito);
    } else {
        printf("   Este cômodo não tem pistas a serem coletadas.\n");
    }

    char escolha;
    while (1) {
        printf("\nOnde deseja ir? [**e**] Esquerda, [**d**] Direita, ou [**s**] Sair da mansão: ");
        if (scanf(" %c", &escolha) != 1) {
            // Limpa o buffer se for uma entrada inválida
            while(getchar() != '\n');
            continue;
        }
        escolha = tolower(escolha);
        if (escolha == 'e' || escolha == 'd' || escolha == 's') {
            break;
        }
        printf("Escolha inválida. Use 'e', 'd', ou 's'.\n");
    }

    if (escolha == 's') {
        printf("\n✅ **Saindo da Mansão...** Iniciando a fase de Acusação!\n");
        return; // Sai da recursão e retorna ao main
    } else if (escolha == 'e' && salaAtual->esquerda != NULL) {
        explorarSalas(salaAtual->esquerda);
    } else if (escolha == 'd' && salaAtual->direita != NULL) {
        explorarSalas(salaAtual->direita);
    } else {
        printf("❌ Não há cômodo nesta direção. Permanece em **%s**.\n", salaAtual->nome);
        // Permite ao jogador tentar novamente na mesma sala
        explorarSalas(salaAtual); 
    }
}

/**
 * @brief Conduz à fase de julgamento final: lista pistas, solicita acusação e verifica evidências.
 */
void verificarSuspeitoFinal() {
    char acusacao[MAX_SUSPEITO];
    int pistasContraSuspeito = 0;

    printf("\n============================================\n");
    printf("        🚨 FASE DE ACUSAÇÃO 🚨\n");
    printf("============================================\n");

    if (totalPistasColetadas == 0) {
        printf("⚠️ Você não coletou nenhuma pista. Acusação impossível.\n");
        printf("FIM DE JOGO: O culpado escapou por falta de provas.\n");
        return;
    }

    printf("Pistas Coletadas (em ordem alfabética):\n");
    listarPistasColetadas(raizPistas);

    printf("\nCom base nas evidências, quem você acusa? (Alfredo, Berta, Carlos): ");
    if (scanf(" %49s", acusacao) != 1) {
        printf("Entrada inválida. FIM DE JOGO.\n");
        return;
    }
    
    // Força a primeira letra para maiúscula para padronização na verificação
    acusacao[0] = toupper(acusacao[0]);

    // Verificação de Pistas: Percorre todas as pistas coletadas na BST e consulta a Hash
    PistaColetada *pilha[MAX_PISTAS]; // Usando um array como pilha para simular a travessia in-order não recursiva
    int topo = -1;
    PistaColetada *atual = raizPistas;

    // Travessia In-Order não recursiva para iterar sobre todas as pistas
    while (atual != NULL || topo != -1) {
        while (atual != NULL) {
            pilha[++topo] = atual;
            atual = atual->esq;
        }
        atual = pilha[topo--];

        // Consulta a Tabela Hash para cada pista
        const char *suspeitoDaPista = encontrarSuspeito(atual->pista);

        if (suspeitoDaPista != NULL && strcmp(suspeitoDaPista, acusacao) == 0) {
            pistasContraSuspeito++;
        }

        atual = atual->dir;
    }

    printf("\n--- RESULTADO DO JULGAMENTO ---\n");
    printf("Suspeito Acusado: **%s**\n", acusacao);
    printf("Pistas que apontam para %s: **%d**\n", acusacao, pistasContraSuspeito);

    if (pistasContraSuspeito >= 2) {
        printf("\n🎉 **VITÓRIA!** A acusação contra %s é sustentada por %d pistas.\n", acusacao, pistasContraSuspeito);
        printf("O caso está resolvido. O culpado foi levado à justiça!\n");
    } else {
        printf("\n😢 **FRACASSO.** Você precisa de pelo menos 2 pistas, mas só encontrou %d.\n", pistasContraSuspeito);
        printf("O caso foi arquivado por falta de provas suficientes. O culpado escapou.\n");
    }
}

/**
 * @brief Libera a memória alocada para a BST de Pistas (recursivo).
 * @param raiz A raiz da BST.
 */
void liberarPistas(PistaColetada *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esq);
        liberarPistas(raiz->dir);
        free(raiz);
    }
}

/**
 * @brief Libera a memória alocada para a Tabela Hash.
 */
void liberarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        ItemHash *atual = tabelaHash[i];
        ItemHash *temp;
        while (atual != NULL) {
            temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}

// -------------------------------------------------------------------
// -------------------- FUNÇÃO PRINCIPAL (MAIN) --------------------
// -------------------------------------------------------------------

int main() {
    // Inicializa a Tabela Hash
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }

    // --- MONTAGEM FIXA DO MAPA DA MANSÃO (Árvore Binária) ---
    // Estrutura:         Saguão
    //                   /      \
    //              Biblioteca   Cozinha
    //               /    \       /     \
    //            Quarto  Jardim  Despensa Escritório

    Sala *saguao = criarSala("Saguão Principal", "Um casaco de inverno molhado no chão.");
    Sala *biblioteca = criarSala("Biblioteca Antiga", "Restos de charuto de alta qualidade (aponta para Alfredo).");
    Sala *cozinha = criarSala("Cozinha Industrial", "Uma faca de cozinha usada e jogada na pia (aponta para Carlos).");
    Sala *quarto = criarSala("Quarto Principal", "Um frasco de perfume caro e vazio.");
    Sala *jardim = criarSala("Jardim de Inverno", "Um par de sapatos enlameados na entrada (aponta para Carlos).");
    Sala *despensa = criarSala("Despensa de Vinhos", "Uma garrafa de vinho tinto de safra rara, quase vazia (aponta para Alfredo).");
    Sala *escritorio = criarSala("Escritório Secreto", "Um fio de cabelo loiro em cima da mesa (aponta para Berta).");
    
    // Conexões da Árvore Binária
    saguao->esquerda = biblioteca;
    saguao->direita = cozinha;

    biblioteca->esquerda = quarto;
    biblioteca->direita = jardim;

    cozinha->esquerda = despensa;
    cozinha->direita = escritorio;

    // Inicia a Lógica do Jogo
    printf("============================================\n");
    printf("   🕵️‍♂️ DETECTIVE QUEST: O MISTÉRIO DA MANSÃO 🕵️‍♀️\n");
    printf("   Explore a mansão para coletar pistas.\n");
    printf("============================================\n");

    explorarSalas(saguao);
    
    // Finaliza o Jogo e Inicia o Julgamento
    verificarSuspeitoFinal();

    // Limpeza de Memória
    // Liberar a memória da Árvore Binária da Mansão (Opcional, mas boa prática)
    // Nota: Deixado de fora para simplificação do main e foco nos requisitos.
    liberarPistas(raizPistas);
    liberarHash();
    
    return 0;
}