/*
 * VITORIA CALCADOS - Cadastro de produtos em C (Atividade Avaliativa 04)
 * Usa: struct, ponteiros, arquivos texto, Bubble Sort, arquivo temporario.
 * Roda no terminal (nao abre nenhum site).
 *
 * Integrantes:
 *   Murillo Kaua Belchior Sanches - RGM 48257460
 *   Leonel Achu Quispe            - RGM 48959219
 *   Lucas Brito Alves             - RGM 48098345
 *   Ronald Hamachi Vargas         - RGM 48909629
 *
 * Compilar:  gcc vitoria_calcados.c -o vitoria
 * Executar:  ./vitoria
 */
#include <stdio.h>    // printf, fopen, fgets, fprintf...
#include <stdlib.h>   // remove, rename (arquivos) e strtod
#include <string.h>   // strlen, strcpy, strchr...
#include <ctype.h>    // tolower

#define MAX 500               // maximo de produtos na memoria
#define ARQUIVO "produtos.txt" // arquivo principal (tabela de dados)
#define TEMP "temp.txt"        // arquivo temporario (alterar/excluir)
#define CSV "produtos.csv"     // arquivo exportado

// Estrutura de um produto (campos com tamanho limitado: aumente se precisar)
typedef struct {
    int id;
    char produto[50];
    char categoria[30];
    float preco;
    int quantidade;
    char fornecedor[40];
} Produto;

/* ---------------------- FUNCOES AUXILIARES ---------------------- */

// Le uma linha de texto do teclado (aceita espacos) e remove o '\n'
void lerTexto(const char *msg, char *dest, int tam) {
    printf("%s", msg);                       // mostra a pergunta
    if (fgets(dest, tam, stdin) == NULL) {   // le a linha inteira
        dest[0] = '\0';
        return;
    }
    dest[strcspn(dest, "\n")] = '\0';        // troca o '\n' por fim de string
}

// Le um numero inteiro com seguranca
int lerInt(const char *msg) {
    char buf[50];
    lerTexto(msg, buf, sizeof(buf));
    return atoi(buf);                        // converte texto em int
}

// Le um numero real; aceita virgula ou ponto (ex.: 49,90 ou 49.90)
float lerFloat(const char *msg) {
    char buf[50];
    lerTexto(msg, buf, sizeof(buf));
    for (char *p = buf; *p; p++)             // ponteiro percorre a string
        if (*p == ',') *p = '.';             // troca virgula por ponto
    return (float) strtod(buf, NULL);        // converte texto em float
}

// Compara dois textos ignorando maiusculas/minusculas (case-insensitive)
int compararTexto(const char *a, const char *b) {
    while (*a && *b) {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;        // diferente: retorna a diferenca
        a++; b++;                            // avanca os ponteiros
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

// Verifica se 'texto' contem 'busca' (ignorando maiusculas/minusculas)
int contem(const char *texto, const char *busca) {
    char t[100], b[100];
    int i;
    for (i = 0; texto[i] && i < 99; i++) t[i] = (char) tolower((unsigned char)texto[i]);
    t[i] = '\0';
    for (i = 0; busca[i] && i < 99; i++) b[i] = (char) tolower((unsigned char)busca[i]);
    b[i] = '\0';
    return strstr(t, b) != NULL;             // strstr procura b dentro de t
}

// Troca dois produtos usando ponteiros
void trocar(Produto *a, Produto *b) {
    Produto aux = *a;
    *a = *b;
    *b = aux;
}

/* ---------------------- ARQUIVO ---------------------- */

// CRIAR: cria o arquivo com os 4 produtos iniciais, se ele ainda nao existir
void criarArquivo(void) {
    FILE *fp = fopen(ARQUIVO, "r");          // tenta abrir para leitura
    if (fp != NULL) {                        // ja existe: nada a fazer
        fclose(fp);
        return;
    }
    fp = fopen(ARQUIVO, "w");                // cria o arquivo (modo escrita)
    if (fp == NULL) {
        printf("Erro ao criar o arquivo!\n");
        return;
    }
    // formato de cada linha: id|produto|categoria|preco|quantidade|fornecedor
    fprintf(fp, "1|Camiseta Basica|Vestuario|49.90|30|Moda Brasil\n");
    fprintf(fp, "2|Mouse Optico USB|Informatica|59.99|15|TecnoShop\n");
    fprintf(fp, "3|Shampoo 300ml|Higiene Pessoal|22.50|40|Beleza Pura\n");
    fprintf(fp, "4|Cafe 500g|Alimentos|18.00|25|Graos do Sul\n");
    fclose(fp);
    printf("Arquivo '%s' criado com 4 produtos iniciais.\n", ARQUIVO);
}

// Carrega o arquivo para o vetor; devolve quantos produtos foram lidos
int carregar(Produto *lista) {
    FILE *fp = fopen(ARQUIVO, "r");
    if (fp == NULL) return 0;
    int n = 0;
    char linha[300];
    while (n < MAX && fgets(linha, sizeof(linha), fp) != NULL) {
        Produto *p = &lista[n];              // ponteiro para a posicao atual
        if (sscanf(linha, "%d|%49[^|]|%29[^|]|%f|%d|%39[^\n]",
                   &p->id, p->produto, p->categoria,
                   &p->preco, &p->quantidade, p->fornecedor) == 6)
            n++;                             // linha valida: conta o produto
    }
    fclose(fp);
    return n;
}

// Grava o vetor inteiro usando arquivo TEMPORARIO e depois substitui o original
// (tecnica para evitar corrupcao de dados se o programa for interrompido)
void salvarTudo(Produto *lista, int n) {
    FILE *fp = fopen(TEMP, "w");
    if (fp == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        Produto *p = &lista[i];
        fprintf(fp, "%d|%s|%s|%.2f|%d|%s\n", p->id, p->produto,
                p->categoria, p->preco, p->quantidade, p->fornecedor);
    }
    fclose(fp);
    remove(ARQUIVO);                         // apaga o original
    rename(TEMP, ARQUIVO);                   // temporario vira o original
}

// ID automatico: maior ID existente + 1 (o usuario nao digita)
int gerarId(Produto *lista, int n) {
    int maior = 0;
    for (int i = 0; i < n; i++)
        if (lista[i].id > maior) maior = lista[i].id;
    return maior + 1;
}

// Procura produto pelo nome exato (sem diferenciar caixa); devolve indice ou -1
int existeNome(Produto *lista, int n, const char *nome) {
    for (int i = 0; i < n; i++)
        if (compararTexto(lista[i].produto, nome) == 0) return i;
    return -1;
}

// Procura produto pelo ID; devolve indice ou -1
int buscarPorId(Produto *lista, int n, int id) {
    for (int i = 0; i < n; i++)
        if (lista[i].id == id) return i;
    return -1;
}

/* ---------------------- EXIBICAO ---------------------- */

// Cabecalho da tabela
void cabecalho(void) {
    printf("\n-------------------------------------------------------------------------------------\n");
    printf("| %-3s | %-20s | %-16s | %10s | %5s | %-14s |\n",
           "ID", "Produto", "Categoria", "Preco (R$)", "Qtd", "Fornecedor");
    printf("-------------------------------------------------------------------------------------\n");
}

// Uma linha da tabela
void linhaProduto(const Produto *p) {
    printf("| %-3d | %-20.20s | %-16.16s | %10.2f | %5d | %-14.14s |\n",
           p->id, p->produto, p->categoria, p->preco, p->quantidade, p->fornecedor);
}

// LISTAR: mostra todos os produtos em formato de tabela
void listar(Produto *lista, int n) {
    if (n == 0) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }
    cabecalho();
    for (int i = 0; i < n; i++) linhaProduto(&lista[i]);
    printf("-------------------------------------------------------------------------------------\n");
    printf("Total: %d produto(s)\n", n);
}

/* ---------------------- OPERACOES ---------------------- */

// INSERIR: cadastra um novo produto (ID automatico e sem duplicatas)
void inserir(Produto *lista, int *n) {
    if (*n >= MAX) {
        printf("Limite de produtos atingido!\n");
        return;
    }
    Produto novo;
    lerTexto("Nome do produto: ", novo.produto, sizeof(novo.produto));
    if (strlen(novo.produto) == 0) {
        printf("Nome vazio. Cancelado.\n");
        return;
    }
    if (existeNome(lista, *n, novo.produto) != -1) {   // verifica duplicata
        printf("Ja existe um produto com esse nome. Cadastro cancelado.\n");
        return;
    }
    lerTexto("Categoria: ", novo.categoria, sizeof(novo.categoria));
    novo.preco = lerFloat("Preco (R$): ");
    novo.quantidade = lerInt("Quantidade: ");
    lerTexto("Fornecedor: ", novo.fornecedor, sizeof(novo.fornecedor));
    novo.id = gerarId(lista, *n);                      // ID incremental

    lista[*n] = novo;                                  // guarda no vetor
    (*n)++;

    FILE *fp = fopen(ARQUIVO, "a");                    // modo append: mantem os dados antigos
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    fprintf(fp, "%d|%s|%s|%.2f|%d|%s\n", novo.id, novo.produto,
            novo.categoria, novo.preco, novo.quantidade, novo.fornecedor);
    fclose(fp);
    printf("Produto cadastrado com ID %d!\n", novo.id);
}

// ALTERAR: edita um produto pelo ID (Enter mantem o valor atual)
void alterar(Produto *lista, int n) {
    int id = lerInt("ID do produto a alterar: ");
    int pos = buscarPorId(lista, n, id);
    if (pos == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }
    Produto *p = &lista[pos];                          // ponteiro para o produto
    char buf[100];
    printf("(Deixe vazio e tecle Enter para manter o valor atual)\n");

    printf("Nome atual: %s\n", p->produto);
    lerTexto("Novo nome: ", buf, sizeof(buf));
    if (strlen(buf) > 0) {
        int outro = existeNome(lista, n, buf);         // evita duplicata ao renomear
        if (outro != -1 && outro != pos) {
            printf("Ja existe outro produto com esse nome. Nome mantido.\n");
        } else {
            strncpy(p->produto, buf, sizeof(p->produto) - 1);
            p->produto[sizeof(p->produto) - 1] = '\0';
        }
    }

    printf("Categoria atual: %s\n", p->categoria);
    lerTexto("Nova categoria: ", buf, sizeof(buf));
    if (strlen(buf) > 0) {
        strncpy(p->categoria, buf, sizeof(p->categoria) - 1);
        p->categoria[sizeof(p->categoria) - 1] = '\0';
    }

    printf("Preco atual: %.2f\n", p->preco);
    lerTexto("Novo preco: ", buf, sizeof(buf));
    if (strlen(buf) > 0) {
        for (char *q = buf; *q; q++) if (*q == ',') *q = '.';
        p->preco = (float) strtod(buf, NULL);
    }

    printf("Quantidade atual: %d\n", p->quantidade);
    lerTexto("Nova quantidade: ", buf, sizeof(buf));
    if (strlen(buf) > 0) p->quantidade = atoi(buf);

    printf("Fornecedor atual: %s\n", p->fornecedor);
    lerTexto("Novo fornecedor: ", buf, sizeof(buf));
    if (strlen(buf) > 0) {
        strncpy(p->fornecedor, buf, sizeof(p->fornecedor) - 1);
        p->fornecedor[sizeof(p->fornecedor) - 1] = '\0';
    }

    salvarTudo(lista, n);                              // regrava via arquivo temporario
    printf("Produto alterado com sucesso!\n");
}

// EXCLUIR: remove um produto pelo ID
void excluir(Produto *lista, int *n) {
    int id = lerInt("ID do produto a excluir: ");
    int pos = buscarPorId(lista, *n, id);
    if (pos == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }
    for (int i = pos; i < *n - 1; i++)                 // puxa os seguintes uma posicao
        lista[i] = lista[i + 1];
    (*n)--;
    salvarTudo(lista, *n);                             // regrava via arquivo temporario
    printf("Produto excluido com sucesso!\n");
}

// BUSCAR POR NOME (parte do nome, sem diferenciar caixa)
void buscarNome(Produto *lista, int n) {
    char termo[50];
    lerTexto("Nome (ou parte): ", termo, sizeof(termo));
    int achou = 0;
    for (int i = 0; i < n; i++) {
        if (contem(lista[i].produto, termo)) {
            if (!achou) cabecalho();
            linhaProduto(&lista[i]);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhum produto encontrado.\n");
}

// BUSCAR POR CATEGORIA (parte da categoria, sem diferenciar caixa)
void buscarCategoria(Produto *lista, int n) {
    char termo[30];
    lerTexto("Categoria (ou parte): ", termo, sizeof(termo));
    int achou = 0;
    for (int i = 0; i < n; i++) {
        if (contem(lista[i].categoria, termo)) {
            if (!achou) cabecalho();
            linhaProduto(&lista[i]);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhuma categoria encontrada.\n");
}

/* ---------------------- ORDENACAO (Bubble Sort) ---------------------- */

// modo: 1 = nome A-Z | 2 = nome Z-A | 3 = preco menor->maior | 4 = preco maior->menor
// Trabalha numa COPIA do vetor, entao o arquivo original nao muda.
void ordenar(Produto *lista, int n, int modo) {
    if (n == 0) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }
    Produto copia[MAX];
    memcpy(copia, lista, n * sizeof(Produto));         // copia o vetor

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            Produto *a = &copia[j];
            Produto *b = &copia[j + 1];
            int trocarAgora = 0;
            switch (modo) {
                case 1: trocarAgora = compararTexto(a->produto, b->produto) > 0; break;
                case 2: trocarAgora = compararTexto(a->produto, b->produto) < 0; break;
                case 3: trocarAgora = a->preco > b->preco; break;
                case 4: trocarAgora = a->preco < b->preco; break;
            }
            if (trocarAgora) trocar(a, b);             // troca via ponteiros
        }
    }
    cabecalho();
    for (int i = 0; i < n; i++) linhaProduto(&copia[i]);
    printf("-------------------------------------------------------------------------------------\n");
}

/* ---------------------- EXPORTAR CSV ---------------------- */

// Exporta os dados para CSV separado por virgula
void exportarCSV(Produto *lista, int n) {
    FILE *fp = fopen(CSV, "w");
    if (fp == NULL) {
        printf("Erro ao criar o CSV!\n");
        return;
    }
    fprintf(fp, "ID,Produto,Categoria,Preco,Quantidade,Fornecedor\n");
    for (int i = 0; i < n; i++) {
        Produto *p = &lista[i];
        fprintf(fp, "%d,\"%s\",\"%s\",%.2f,%d,\"%s\"\n", p->id, p->produto,
                p->categoria, p->preco, p->quantidade, p->fornecedor);
    }
    fclose(fp);
    printf("Dados exportados para '%s'.\n", CSV);
}

/* ---------------------- PROGRAMA PRINCIPAL ---------------------- */

int main(void) {
    Produto lista[MAX];                                // vetor de estruturas
    int n, opcao;

    criarArquivo();                                    // cria o arquivo se nao existir
    n = carregar(lista);                               // carrega para a memoria

    do {
        printf("\n===== VITORIA CALCADOS =====\n");
        printf(" 1 - Listar produtos\n");
        printf(" 2 - Inserir produto\n");
        printf(" 3 - Alterar produto\n");
        printf(" 4 - Excluir produto\n");
        printf(" 5 - Buscar por nome\n");
        printf(" 6 - Buscar por categoria\n");
        printf(" 7 - Ordenar por nome (A-Z)\n");
        printf(" 8 - Ordenar por nome (Z-A)\n");
        printf(" 9 - Ordenar por preco (menor -> maior)\n");
        printf("10 - Ordenar por preco (maior -> menor)\n");
        printf("11 - Exportar para CSV\n");
        printf(" 0 - Sair\n");
        opcao = lerInt("Escolha: ");

        switch (opcao) {
            case 1:  listar(lista, n); break;
            case 2:  inserir(lista, &n); break;
            case 3:  alterar(lista, n); break;
            case 4:  excluir(lista, &n); break;
            case 5:  buscarNome(lista, n); break;
            case 6:  buscarCategoria(lista, n); break;
            case 7:  ordenar(lista, n, 1); break;
            case 8:  ordenar(lista, n, 2); break;
            case 9:  ordenar(lista, n, 3); break;
            case 10: ordenar(lista, n, 4); break;
            case 11: exportarCSV(lista, n); break;
            case 0:  printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
