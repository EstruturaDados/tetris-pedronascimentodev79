#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // número máximo de peças na fila
#define MAX_PILHA 3  // número máximo de peças na pilha de reserva

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // tipo da peça ('I', 'O', 'T', 'L')
    int id;     // identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// Estrutura da pilha de peças reservadas
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ---------- PROTÓTIPOS DE FUNÇÕES ----------
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
Peca gerarPeca();
void inserirPeca(Fila *f, Peca p);
Peca jogarPeca(Fila *f);
void reservarPeca(Fila *f, Pilha *p);
void usarPecaReservada(Pilha *p);
void empilharPeca(Pilha *p, Peca peca);
Peca desempilharPeca(Pilha *p);
void trocarPecaAtual(Fila *f, Pilha *p);
void trocaMultipla(Fila *f, Pilha *p);
void exibirFila(Fila *f);
void exibirPilha(Pilha *p);
void exibirEstado(Fila *f, Pilha *p);

// ---------- VARIÁVEL GLOBAL PARA ID ----------
int proximoID = 0;

// ---------- FUNÇÃO PRINCIPAL ----------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;

    srand(time(NULL)); // inicializa gerador de números aleatórios
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        printf("\n===== TETRIS STACK =====\n");
        exibirEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar peça atual (fila <-> pilha)\n");
        printf("5 - Troca múltipla (3 primeiras da fila <-> pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                jogarPeca(&fila);
                inserirPeca(&fila, gerarPeca());
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                inserirPeca(&fila, gerarPeca());
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\n⚠️  Opção inválida!\n");
        }
    } while(opcao != 0);

    return 0;
}

// ---------- FUNÇÕES DE INICIALIZAÇÃO ----------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;

    for (int i = 0; i < MAX_FILA; i++) {
        inserirPeca(f, gerarPeca());
    }
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// ---------- FUNÇÕES DE ESTADO ----------
int filaCheia(Fila *f) {
    return (f->quantidade == MAX_FILA);
}

int filaVazia(Fila *f) {
    return (f->quantidade == 0);
}

int pilhaCheia(Pilha *p) {
    return (p->topo == MAX_PILHA - 1);
}

int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

// ---------- GERAÇÃO DE PEÇAS ----------
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = proximoID++;
    return nova;
}

// ---------- FUNÇÕES DE FILA ----------
void inserirPeca(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n⚠️  Fila cheia! Não é possível adicionar nova peça.\n");
        return;
    }

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->quantidade++;
}

Peca jogarPeca(Fila *f) {
    Peca removida = {' ', -1};
    if (filaVazia(f)) {
        printf("\n⚠️  Fila vazia! Nenhuma peça para jogar.\n");
        return removida;
    }

    removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->quantidade--;

    printf("\n🎮 Peça jogada: [%c %d]\n", removida.nome, removida.id);
    return removida;
}

// ---------- FUNÇÕES DE PILHA ----------
void empilharPeca(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) {
        printf("\n⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = peca;
}

Peca desempilharPeca(Pilha *p) {
    Peca removida = {' ', -1};
    if (pilhaVazia(p)) {
        printf("\n⚠️  Pilha vazia! Nenhuma peça reservada para usar.\n");
        return removida;
    }

    removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// ---------- OPERAÇÕES DE JOGO ----------
void reservarPeca(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("\n⚠️  Fila vazia! Nenhuma peça para reservar.\n");
        return;
    }
    if (pilhaCheia(p)) {
        printf("\n⚠️  Pilha cheia! Libere espaço antes de reservar nova peça.\n");
        return;
    }

    Peca peca = jogarPeca(f);
    if (peca.id != -1) {
        empilharPeca(p, peca);
        printf("💾 Peça [%c %d] reservada!\n", peca.nome, peca.id);
    }
}

void usarPecaReservada(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\n⚠️  Nenhuma peça reservada disponível.\n");
        return;
    }

    Peca usada = desempilharPeca(p);
    printf("\n🔧 Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
}

// ---------- NOVAS FUNÇÕES DE TROCA ----------

// 🔁 Troca a peça da frente da fila com o topo da pilha
void trocarPecaAtual(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\n⚠️  É necessário ter pelo menos uma peça na fila e na pilha para trocar.\n");
        return;
    }

    int idx = f->inicio;
    Peca temp = f->itens[idx];
    f->itens[idx] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\n🔄 Troca realizada entre a frente da fila e o topo da pilha!\n");
}

// 🔁 Troca múltipla entre as 3 primeiras da fila e as 3 da pilha
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->quantidade < 3 || p->topo < 2) {
        printf("\n⚠️  É necessário pelo menos 3 peças em cada estrutura para a troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->inicio + i) % MAX_FILA;
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("\n🔁 Troca múltipla realizada entre as 3 primeiras peças da fila e da pilha!\n");
}

// ---------- EXIBIÇÃO ----------
void exibirFila(Fila *f) {
    printf("Fila de peças:\t");
    if (filaVazia(f)) {
        printf("[vazia]");
        return;
    }

    int i = f->inicio;
    for (int c = 0; c < f->quantidade; c++) {
        printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
        i = (i + 1) % MAX_FILA;
    }
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[vazia]");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n--- ESTADO ATUAL ---\n");
    exibirFila(f);
    printf("\n");
    exibirPilha(p);
    printf("\n---------------------\n");
}
