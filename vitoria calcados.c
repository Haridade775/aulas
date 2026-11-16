/*
 * Vitória Calçados - sistema simples de loja em C
 * Compilar: gcc vitoria_calcados.c -o vitoria_calcados
 * Executar: ./vitoria_calcados
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_LOJA "Vitória Calçados"
#define MAX_PRODUTOS 100


typedef struct {
    int codigo;
    char nome[50];
    int tamanho;
    float preco;
    int estoque;
} Produto;

static Produto produtos[MAX_PRODUTOS];
static int total_produtos = 0;
static int proximo_codigo = 1;
static float total_vendido = 0.0f;

static void ler_texto(char *dest, int tam) {
    if (fgets(dest, tam, stdin))
        dest[strcspn(dest, "\n")] = '\0';
}

static int ler_int(const char *msg) {
    char buf[32];
    printf("%s", msg);
    ler_texto(buf, sizeof buf);
    return atoi(buf);
}

static float ler_float(const char *msg) {
    char buf[32];
    printf("%s", msg);
    ler_texto(buf, sizeof buf);
    for (char *p = buf; *p; p++)
        if (*p == ',') *p = '.';
    return (float)atof(buf);
}

static Produto *buscar(int codigo) {
    for (int i = 0; i < total_produtos; i++)
        if (produtos[i].codigo == codigo) return &produtos[i];
    return NULL;
}

static void cadastrar(void) {
    if (total_produtos >= MAX_PRODUTOS) {
        printf("Estoque cheio!\n");
        return;
    }
    Produto *p = &produtos[total_produtos];
    p->codigo = proximo_codigo;
    printf("Nome do calçado: ");
    ler_texto(p->nome, sizeof p->nome);
    p->tamanho = ler_int("Tamanho: ");
    p->preco = ler_float("Preço (R$): ");
    p->estoque = ler_int("Quantidade em estoque: ");
    if (p->preco < 0 || p->estoque < 0 || p->nome[0] == '\0') {
        printf("Dados inválidos. Cadastro cancelado.\n");
        return;
    }
    total_produtos++;
    proximo_codigo++;
    printf("Cadastrado com o código %d.\n", p->codigo);
}

static void listar(void) {
    if (total_produtos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    printf("\n%-6s %-30s %-8s %-10s %-8s\n", "Cód", "Nome", "Tam", "Preço", "Estoque");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < total_produtos; i++) {
        Produto *p = &produtos[i];
        printf("%-6d %-30s %-8d R$%-8.2f %-8d\n",
               p->codigo, p->nome, p->tamanho, p->preco, p->estoque);
    }
}

static void vender(void) {
    int cod = ler_int("Código do produto: ");
    Produto *p = buscar(cod);
    if (!p) {
        printf("Produto não encontrado.\n");
        return;
    }
    int qtd = ler_int("Quantidade: ");
    if (qtd <= 0 || qtd > p->estoque) {
        printf("Quantidade inválida. Em estoque: %d\n", p->estoque);
        return;
    }
    float valor = qtd * p->preco;
    p->estoque -= qtd;
    total_vendido += valor;
    printf("Venda realizada: %d x %s = R$ %.2f\n", qtd, p->nome, valor);
}

static void repor(void) {
    int cod = ler_int("Código do produto: ");
    Produto *p = buscar(cod);
    if (!p) {
        printf("Produto não encontrado.\n");
        return;
    }
    int qtd = ler_int("Quantidade a adicionar: ");
    if (qtd <= 0) {
        printf("Quantidade inválida.\n");
        return;
    }
    p->estoque += qtd;
    printf("Novo estoque de %s: %d\n", p->nome, p->estoque);
}

static void relatorio(void) {
    float valor_estoque = 0.0f;
    int pares = 0;
    for (int i = 0; i < total_produtos; i++) {
        valor_estoque += produtos[i].preco * produtos[i].estoque;
        pares += produtos[i].estoque;
    }
    printf("\n=== Relatório - %s ===\n", NOME_LOJA);
    printf("Modelos cadastrados: %d\n", total_produtos);
    printf("Pares em estoque:    %d\n", pares);
    printf("Valor do estoque:    R$ %.2f\n", valor_estoque);
    printf("Total vendido:       R$ %.2f\n", total_vendido);
}

int main(void) {
    int opcao;
    do {
        printf("\n===== %s =====\n", NOME_LOJA);
        printf("1 - Cadastrar calçado\n");
        printf("2 - Listar calçados\n");
        printf("3 - Realizar venda\n");
        printf("4 - Repor estoque\n");
        printf("5 - Relatório\n");
        printf("0 - Sair\n");
        opcao = ler_int("Escolha: ");

        switch (opcao) {
            case 1: cadastrar(); break;
            case 2: listar();    break;
            case 3: vender();    break;
            case 4: repor();     break;
            case 5: relatorio(); break;
            case 0: printf("Obrigado por usar o sistema da %s!\n", NOME_LOJA); break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);
    return 0;
}
