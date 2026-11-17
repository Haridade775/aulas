/*
 * Vitória Calçados - Preço justo que cabe no bolso
 * Sistema de loja de bairro em C (terminal). Vende sapatos, tênis, chinelos e acessórios.
 * Compilar: gcc vitoria_calcados.c -o vitoria_calcados
 */
#include <stdio.h>   // entrada e saída (printf, fgets e arquivos)
#include <stdlib.h>  // malloc, realloc, free, atoi e atof
#include <string.h>  // strcmp, strstr, memmove e memcpy
#include <ctype.h>   // tolower

#define NOME_LOJA "VITÓRIA CALÇADOS - PREÇO JUSTO QUE CABE NO BOLSO"  // título mostrado no menu
#define ARQUIVO_DADOS "vitoria.txt"  // arquivo onde os produtos ficam salvos
#define ARQUIVO_CSV "vitoria.csv"    // arquivo gerado pela exportação

typedef struct {         // struct que representa um produto da loja
    int id;              // código gerado automaticamente
    char nome[50];       // nome do produto
    char categoria[30];  // categoria (Sapato, Tênis, Chinelo...)
    int estoque;         // unidades disponíveis
    int vendidos;        // total já vendido
    float preco;         // preço em reais
} Produto;

typedef struct {         // struct que guarda a lista dinâmica de produtos
    Produto *itens;      // ponteiro para o vetor de produtos
    int total;           // quantidade de produtos cadastrados
    int capacidade;      // espaço reservado na memória
} Loja;

/* ---------- Leitura e texto ---------- */

static void ler_texto(char *dest, int tam) {          // lê uma linha digitada
    if (fgets(dest, tam, stdin))                      // lê do teclado com limite de tamanho
        dest[strcspn(dest, "\n")] = '\0';             // troca o Enter por fim de texto
}

static int ler_int(const char *msg) {                 // mostra a mensagem e lê um inteiro
    char buf[32];                                     // área temporária
    printf("%s", msg);                                // exibe a pergunta
    ler_texto(buf, sizeof buf);                       // lê o texto digitado
    return atoi(buf);                                 // converte para inteiro
}

static float ler_float(const char *msg) {             // mostra a mensagem e lê um decimal
    char buf[32];                                     // área temporária
    printf("%s", msg);                                // exibe a pergunta
    ler_texto(buf, sizeof buf);                       // lê o texto digitado
    for (char *c = buf; *c; c++)                      // percorre o texto com ponteiro
        if (*c == ',') *c = '.';                      // aceita vírgula como separador decimal
    return (float)atof(buf);                          // converte para float
}

static void minusculo(const char *origem, char *destino) {  // copia o texto em letras minúsculas
    int i;                                            // posição atual
    for (i = 0; origem[i]; i++)                       // percorre cada caractere
        destino[i] = (char)tolower((unsigned char)origem[i]);  // converte para minúsculo
    destino[i] = '\0';                                // finaliza o texto
}

static int comparar(const char *a, const char *b) {   // compara textos sem diferenciar maiúsculas
    char x[50], y[50];                                // cópias em minúsculo
    minusculo(a, x);                                  // converte o primeiro texto
    minusculo(b, y);                                  // converte o segundo texto
    return strcmp(x, y);                              // devolve <0, 0 ou >0
}

static int contem(const char *texto, const char *trecho) {  // verifica se o texto contém o trecho
    char x[50], y[50];                                // cópias em minúsculo
    minusculo(texto, x);                              // converte o texto
    minusculo(trecho, y);                             // converte o trecho
    return strstr(x, y) != NULL;                      // 1 se encontrou, 0 se não
}

/* ---------- Lista de produtos ---------- */

static int adicionar(Loja *loja, Produto p) {         // coloca um produto no fim da lista
    if (loja->total == loja->capacidade) {            // lista cheia? precisa crescer
        int nova = loja->capacidade ? loja->capacidade * 2 : 8;  // dobra o espaço (ou começa com 8)
        Produto *tmp = realloc(loja->itens, nova * sizeof(Produto));  // pede mais memória
        if (!tmp) return 0;                           // falhou: avisa quem chamou
        loja->itens = tmp;                            // guarda o novo endereço
        loja->capacidade = nova;                      // guarda a nova capacidade
    }
    loja->itens[loja->total++] = p;                   // grava o produto e aumenta o total
    return 1;                                         // deu certo
}

static Produto *buscar_id(Loja *loja, int id) {       // procura um produto pelo ID
    for (int i = 0; i < loja->total; i++)             // percorre a lista
        if (loja->itens[i].id == id) return &loja->itens[i];  // devolve o endereço do produto
    return NULL;                                      // não achou
}

static int nome_existe(const Loja *loja, const char *nome, int ignorar_id) {  // evita nome repetido
    for (int i = 0; i < loja->total; i++)             // percorre a lista
        if (loja->itens[i].id != ignorar_id && comparar(loja->itens[i].nome, nome) == 0)  // mesmo nome em outro produto?
            return 1;                                 // já existe
    return 0;                                         // nome livre
}

static int proximo_id(const Loja *loja) {             // gera o ID automático
    int maior = 0;                                    // maior ID encontrado
    for (int i = 0; i < loja->total; i++)             // percorre a lista
        if (loja->itens[i].id > maior) maior = loja->itens[i].id;  // guarda o maior
    return maior + 1;                                 // próximo ID livre
}

/* ---------- Arquivos ---------- */

static void salvar(const Loja *loja) {               // grava todos os produtos em arquivo
    FILE *f = fopen(ARQUIVO_DADOS, "w");              // abre para escrita
    if (!f) { printf("Erro ao salvar os dados.\n"); return; }  // não conseguiu abrir
    for (int i = 0; i < loja->total; i++) {           // percorre a lista
        const Produto *p = &loja->itens[i];           // ponteiro para o produto atual
        fprintf(f, "%d;%s;%s;%d;%d;%.2f\n", p->id, p->nome, p->categoria, p->estoque, p->vendidos, p->preco);  // uma linha por produto
    }
    fclose(f);                                        // fecha o arquivo
}

static void dados_iniciais(Loja *loja) {              // produtos de exemplo da loja de bairro
    Produto base[] = {                                // tabela inicial
        {1, "Sapato Social Masculino", "Sapato",    12, 14,  89.90f},
        {2, "Tênis Casual",            "Tênis",     20, 22, 119.90f},
        {3, "Chinelo de Dedo",         "Chinelo",   40, 60,  24.90f},
        {4, "Cinto de Couro",          "Acessório", 25,  9,  34.90f},
        {5, "Sandália Rasteira",       "Sandália",  18, 17,  49.90f}
    };
    int qtd = sizeof base / sizeof base[0];           // quantidade de produtos da tabela
    for (int i = 0; i < qtd; i++) adicionar(loja, base[i]);  // coloca cada um na lista
}

static void carregar(Loja *loja) {                    // lê os produtos do arquivo
    FILE *f = fopen(ARQUIVO_DADOS, "r");              // abre para leitura
    Produto p;                                        // produto lido
    if (f) {                                          // arquivo existe?
        while (fscanf(f, " %d;%49[^;];%29[^;];%d;%d;%f", &p.id, p.nome, p.categoria, &p.estoque, &p.vendidos, &p.preco) == 6)  // lê uma linha
            adicionar(loja, p);                       // guarda na lista
        fclose(f);                                    // fecha o arquivo
    }
    if (loja->total == 0) {                           // sem dados? usa os iniciais
        dados_iniciais(loja);                         // carrega os exemplos
        salvar(loja);                                 // já grava no arquivo
    }
}

static void exportar_csv(const Loja *loja) {          // gera um arquivo CSV para planilhas
    FILE *f = fopen(ARQUIVO_CSV, "w");                // abre para escrita
    if (!f) { printf("Erro ao criar o CSV.\n"); return; }  // não conseguiu abrir
    fprintf(f, "id,nome,categoria,estoque,vendidos,preco\n");  // linha de cabeçalho
    for (int i = 0; i < loja->total; i++) {           // percorre a lista
        const Produto *p = &loja->itens[i];           // produto atual
        fprintf(f, "%d,%s,%s,%d,%d,%.2f\n", p->id, p->nome, p->categoria, p->estoque, p->vendidos, p->preco);  // uma linha por produto
    }
    fclose(f);                                        // fecha o arquivo
    printf("Arquivo %s criado com %d produtos.\n", ARQUIVO_CSV, loja->total);  // confirma
}

/* ---------- Exibição ---------- */

static void cabecalho_tabela(void) {                  // título da tabela
    printf("\n%-4s %-26s %-12s %-8s %-9s %s\n", "ID", "Nome", "Categoria", "Estoque", "Vendidos", "Preço");  // colunas
    printf("---------------------------------------------------------------------\n");  // linha separadora
}

static void mostrar(const Produto *p) {               // imprime uma linha da tabela
    printf("%-4d %-26s %-12s %-8d %-9d R$ %.2f\n", p->id, p->nome, p->categoria, p->estoque, p->vendidos, p->preco);  // dados do produto
}

static void listar(const Loja *loja) {                // lista todos os produtos
    if (loja->total == 0) { printf("Nenhum produto cadastrado.\n"); return; }  // lista vazia
    cabecalho_tabela();                               // mostra o cabeçalho
    for (int i = 0; i < loja->total; i++) mostrar(&loja->itens[i]);  // mostra cada produto
}

/* ---------- CRUD ---------- */

static void cadastrar(Loja *loja) {                   // Create: cadastra um novo produto
    Produto p;                                        // produto novo
    p.id = proximo_id(loja);                          // ID automático
    printf("Nome do produto: ");                      // pergunta o nome
    ler_texto(p.nome, sizeof p.nome);                 // lê o nome
    if (p.nome[0] == '\0') { printf("Nome vazio. Cadastro cancelado.\n"); return; }  // nome obrigatório
    if (nome_existe(loja, p.nome, 0)) { printf("Já existe um produto com esse nome.\n"); return; }  // bloqueia duplicado
    printf("Categoria: ");                            // pergunta a categoria
    ler_texto(p.categoria, sizeof p.categoria);       // lê a categoria
    p.estoque = ler_int("Quantidade em estoque: ");   // lê o estoque
    p.preco = ler_float("Preço (R$): ");              // lê o preço
    p.vendidos = 0;                                   // começa sem vendas
    if (p.estoque < 0 || p.preco < 0) { printf("Valores inválidos. Cadastro cancelado.\n"); return; }  // valida números
    if (!adicionar(loja, p)) { printf("Sem memória.\n"); return; }  // guarda na lista
    salvar(loja);                                     // grava no arquivo
    printf("Produto cadastrado com o ID %d.\n", p.id);  // confirma
}

static void atualizar(Loja *loja) {                   // Update: edita um produto
    Produto *p = buscar_id(loja, ler_int("ID do produto: "));  // localiza pelo ID
    char buf[50];                                     // texto digitado
    char cat[30];                                     // categoria digitada
    if (!p) { printf("Produto não encontrado.\n"); return; }  // ID inexistente
    printf("Editando %s (Enter mantém o valor atual)\n", p->nome);  // orientação
    printf("Novo nome: ");                            // pergunta o nome
    ler_texto(buf, sizeof buf);                       // lê o texto
    if (buf[0]) {                                     // digitou algo?
        if (nome_existe(loja, buf, p->id)) printf("Esse nome já existe. Nome mantido.\n");  // evita duplicado
        else snprintf(p->nome, sizeof p->nome, "%s", buf);  // troca o nome
    }
    printf("Nova categoria: ");                       // pergunta a categoria
    ler_texto(cat, sizeof cat);                       // lê o texto
    if (cat[0]) snprintf(p->categoria, sizeof p->categoria, "%s", cat);  // troca a categoria
    printf("Novo estoque: ");                         // pergunta o estoque
    ler_texto(buf, sizeof buf);                       // lê o texto
    if (buf[0] && atoi(buf) >= 0) p->estoque = atoi(buf);  // troca o estoque
    printf("Novo preço: ");                           // pergunta o preço
    ler_texto(buf, sizeof buf);                       // lê o texto
    if (buf[0] && atof(buf) >= 0) p->preco = (float)atof(buf);  // troca o preço
    salvar(loja);                                     // grava no arquivo
    printf("Produto atualizado.\n");                  // confirma
}

static void excluir(Loja *loja) {                     // Delete: remove um produto
    int id = ler_int("ID do produto a excluir: ");    // lê o ID
    int i;                                            // posição do produto
    char r[8];                                        // resposta s/n
    for (i = 0; i < loja->total; i++)                 // procura o produto
        if (loja->itens[i].id == id) break;           // achou: para o laço
    if (i == loja->total) { printf("Produto não encontrado.\n"); return; }  // não existe
    printf("Excluir %s? (s/n): ", loja->itens[i].nome);  // pede confirmação
    ler_texto(r, sizeof r);                           // lê a resposta
    if (tolower((unsigned char)r[0]) != 's') { printf("Cancelado.\n"); return; }  // desistiu
    memmove(&loja->itens[i], &loja->itens[i + 1], (loja->total - i - 1) * sizeof(Produto));  // puxa os próximos para trás
    loja->total--;                                    // diminui o total
    salvar(loja);                                     // grava no arquivo
    printf("Produto excluído.\n");                    // confirma
}

/* ---------- Busca ---------- */

static void buscar(const Loja *loja, int por_categoria) {  // procura por nome ou por categoria
    char termo[50];                                   // texto procurado
    int achou = 0;                                    // contador de resultados
    printf(por_categoria ? "Categoria: " : "Nome (ou parte dele): ");  // pergunta conforme o tipo
    ler_texto(termo, sizeof termo);                   // lê o termo
    for (int i = 0; i < loja->total; i++) {           // percorre a lista
        const Produto *p = &loja->itens[i];           // produto atual
        if (contem(por_categoria ? p->categoria : p->nome, termo)) {  // bate com o termo?
            if (!achou) cabecalho_tabela();           // cabeçalho só no primeiro resultado
            mostrar(p);                               // mostra o produto
            achou++;                                  // conta o resultado
        }
    }
    if (!achou) printf("Nada encontrado.\n");         // nenhum resultado
}

/* ---------- Ordenação (Bubble Sort) ---------- */

static int deve_trocar(const Produto *a, const Produto *b, int criterio) {  // decide se a vem depois de b
    switch (criterio) {                               // escolhe o critério
        case 1: return comparar(a->nome, b->nome) > 0;            // nome de A a Z
        case 2: return comparar(a->categoria, b->categoria) > 0;  // categoria de A a Z
        case 3: return a->preco > b->preco;           // preço do menor para o maior
        case 4: return a->estoque > b->estoque;       // estoque do menor para o maior
    }
    return 0;                                         // critério desconhecido
}

static void bubble_sort(Produto *v, int n, int criterio) {  // ordena o vetor com Bubble Sort
    for (int i = 0; i < n - 1; i++)                   // repete as passadas
        for (int j = 0; j < n - 1 - i; j++)           // compara vizinhos
            if (deve_trocar(&v[j], &v[j + 1], criterio)) {  // estão na ordem errada?
                Produto t = v[j];                     // guarda o primeiro
                v[j] = v[j + 1];                      // sobe o segundo
                v[j + 1] = t;                         // desce o primeiro
            }
}

static void ordenar(const Loja *loja) {               // mostra a lista ordenada sem mudar a original
    int op, n = loja->total;                          // critério escolhido e tamanho
    Produto *copia;                                   // cópia para ordenar
    if (n == 0) { printf("Nenhum produto cadastrado.\n"); return; }  // lista vazia
    printf("1-Nome  2-Categoria  3-Preço  4-Estoque\n");  // opções de ordenação
    op = ler_int("Ordenar por: ");                    // lê a escolha
    if (op < 1 || op > 4) { printf("Opção inválida.\n"); return; }  // valida
    copia = malloc(n * sizeof(Produto));              // reserva memória para a cópia
    if (!copia) return;                               // sem memória
    memcpy(copia, loja->itens, n * sizeof(Produto));  // copia os produtos
    bubble_sort(copia, n, op);                        // ordena a cópia
    cabecalho_tabela();                               // mostra o cabeçalho
    for (int i = 0; i < n; i++) mostrar(&copia[i]);   // mostra a lista ordenada
    free(copia);                                      // libera a cópia
}

/* ---------- Vendas e preço justo ---------- */

static void vender(Loja *loja) {                      // registra uma venda
    Produto *p = buscar_id(loja, ler_int("ID do produto: "));  // localiza pelo ID
    int qtd;                                          // quantidade vendida
    if (!p) { printf("Produto não encontrado.\n"); return; }  // ID inexistente
    qtd = ler_int("Quantidade: ");                    // lê a quantidade
    if (qtd <= 0 || qtd > p->estoque) { printf("Quantidade inválida. Estoque: %d\n", p->estoque); return; }  // valida
    p->estoque -= qtd;                                // desconta do estoque
    p->vendidos += qtd;                               // soma nas vendas
    salvar(loja);                                     // grava no arquivo
    printf("Venda feita: %d x %s = R$ %.2f\n", qtd, p->nome, qtd * p->preco);  // confirma
}

static void cabe_no_bolso(const Loja *loja) {         // lista o que o cliente consegue pagar
    float limite = ler_float("Quanto você quer gastar (R$)? ");  // lê o valor máximo
    int n = loja->total, achou = 0;                   // tamanho da lista e contador de resultados
    Produto *copia;                                   // cópia para ordenar por preço
    if (n == 0) { printf("Nenhum produto cadastrado.\n"); return; }  // lista vazia
    copia = malloc(n * sizeof(Produto));              // reserva memória
    if (!copia) return;                               // sem memória
    memcpy(copia, loja->itens, n * sizeof(Produto));  // copia os produtos
    bubble_sort(copia, n, 3);                         // ordena do mais barato ao mais caro
    for (int i = 0; i < n; i++)                       // percorre a lista ordenada
        if (copia[i].preco <= limite && copia[i].estoque > 0) {  // cabe no bolso e tem em estoque?
            if (!achou) cabecalho_tabela();           // cabeçalho só no primeiro resultado
            mostrar(&copia[i]);                       // mostra o produto
            achou++;                                  // conta o resultado
        }
    if (!achou) printf("Nenhum produto até R$ %.2f.\n", limite);  // nada coube
    free(copia);                                      // libera a cópia
}

/* ---------- Programa principal ---------- */

int main(void) {
    Loja loja = {NULL, 0, 0};                         // lista começa vazia
    int opcao;                                        // opção do menu
    carregar(&loja);                                  // lê o arquivo (ou usa os dados iniciais)
    do {                                              // repete até sair
        printf("\n===== %s =====\n", NOME_LOJA);      // título
        printf(" 1 - Cadastrar produto\n 2 - Listar produtos\n 3 - Atualizar produto\n 4 - Excluir produto\n");  // CRUD
        printf(" 5 - Buscar por nome\n 6 - Buscar por categoria\n 7 - Ordenar listagem\n");  // busca e ordenação
        printf(" 8 - Registrar venda\n 9 - Cabe no bolso (até quanto quer gastar)\n10 - Exportar CSV\n 0 - Sair\n");  // vendas e extras
        opcao = ler_int("Escolha: ");                 // lê a opção
        switch (opcao) {                              // chama a função escolhida
            case 1: cadastrar(&loja);   break;        // cadastrar
            case 2: listar(&loja);      break;        // listar
            case 3: atualizar(&loja);   break;        // atualizar
            case 4: excluir(&loja);     break;        // excluir
            case 5: buscar(&loja, 0);   break;        // busca por nome
            case 6: buscar(&loja, 1);   break;        // busca por categoria
            case 7: ordenar(&loja);     break;        // ordenar
            case 8: vender(&loja);      break;        // vender
            case 9: cabe_no_bolso(&loja); break;      // produtos dentro do orçamento
            case 10: exportar_csv(&loja); break;      // exportar
            case 0: printf("Obrigado pela preferência! Volte sempre.\n"); break;  // despedida
            default: printf("Opção inválida.\n");     // opção fora do menu
        }
    } while (opcao != 0);                             // sai com 0
    free(loja.itens);                                 // libera a memória
    return 0;                                         // fim do programa
}
