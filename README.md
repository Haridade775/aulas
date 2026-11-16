# Vitória Calçados — Cadastro de Produtos em C

Programa de terminal em linguagem C para gerenciar o cadastro de produtos de uma loja.
Desenvolvido como **Atividade Avaliativa 04**, unindo algoritmos de ordenação, manipulação de arquivos, estruturas de dados (`struct`) e ponteiros.

## Integrantes

| Nome                            | RGM      |
|---------------------------------|----------|
| Murillo Kauã Belchior Sanches   | 48257460 |
| Leonel Achu Quispe              | 48959219 |
| Lucas Brito Alves               | 48098345 |
| Ronald Hamachi Vargas           | 48909629 |

## Funcionalidades

- **Criar** o arquivo de dados automaticamente, com 4 produtos iniciais
- **Listar** todos os produtos em formato de tabela
- **Inserir** produto com **ID automático** (incremental, o usuário não digita)
- **Verificação de duplicatas**: não permite dois produtos com o mesmo nome
- **Alterar** produto pelo ID (Enter mantém o valor atual)
- **Excluir** produto pelo ID
- **Buscar** por nome e por categoria (por parte do texto)
- **Ordenar** (sem alterar o arquivo):
  - Nome A–Z
  - Nome Z–A
  - Preço menor → maior
  - Preço maior → menor
- **Exportar** os dados para CSV (separado por vírgula)

> Comparações e buscas são *case-insensitive* (não diferenciam maiúsculas de minúsculas).

## Como compilar e executar

Requisito: compilador C (GCC, por exemplo).

```bash
gcc vitoria_calcados.c -o vitoria
./vitoria
```

No Windows (MinGW):

```bash
gcc vitoria_calcados.c -o vitoria.exe
vitoria.exe
```

## Menu do programa

```
===== VITORIA CALCADOS =====
 1 - Listar produtos
 2 - Inserir produto
 3 - Alterar produto
 4 - Excluir produto
 5 - Buscar por nome
 6 - Buscar por categoria
 7 - Ordenar por nome (A-Z)
 8 - Ordenar por nome (Z-A)
 9 - Ordenar por preco (menor -> maior)
10 - Ordenar por preco (maior -> menor)
11 - Exportar para CSV
 0 - Sair
```

## Dados iniciais

| ID | Produto          | Categoria       | Preço (R$) | Quantidade | Fornecedor   |
|----|------------------|-----------------|------------|------------|--------------|
| 1  | Camiseta Básica  | Vestuário       | 49.90      | 30         | Moda Brasil  |
| 2  | Mouse Óptico USB | Informática     | 59.99      | 15         | TecnoShop    |
| 3  | Shampoo 300ml    | Higiene Pessoal | 22.50      | 40         | Beleza Pura  |
| 4  | Café 500g        | Alimentos       | 18.00      | 25         | Grãos do Sul |

## Arquivos gerados

| Arquivo        | Descrição                                                        |
|----------------|------------------------------------------------------------------|
| `produtos.txt` | Banco de dados em texto (uma linha por produto)                  |
| `temp.txt`     | Arquivo temporário usado ao alterar/excluir (some após o uso)    |
| `produtos.csv` | Exportação dos dados (gerado na opção 11)                        |

Formato de cada linha do `produtos.txt`:

```
id|produto|categoria|preco|quantidade|fornecedor
```

## Como funciona

- **Estrutura:** `struct Produto` com `id`, `produto[50]`, `categoria[30]`, `preco`, `quantidade` e `fornecedor[40]`. Os dados ficam num vetor `Produto lista[]` na memória.
- **ID automático:** `gerarId()` retorna o maior ID existente + 1.
- **Duplicatas:** `existeNome()` compara o nome ignorando maiúsculas/minúsculas, ao inserir e ao renomear.
- **Inserir:** abre o arquivo em modo **append** (`"a"`), mantendo os dados anteriores.
- **Alterar/Excluir:** o vetor é regravado em um **arquivo temporário** que depois substitui o original (`remove` + `rename`), técnica para evitar corrupção de dados.
- **Ponteiros:** usados na troca de produtos (`trocar`), na leitura de texto e no acesso aos elementos do vetor (`Produto *p = &lista[i]`).
- **Ordenação:** **Bubble Sort** aplicado a uma cópia do vetor, exibindo o resultado em tabela sem modificar o arquivo.

## Limitações

- Campos de texto têm tamanho limitado (ex.: `produto[50]`); aumente os valores na `struct` se precisar de textos maiores.
- Capacidade máxima de 500 produtos (constante `MAX`).
- Os nomes são salvos sem acentos nos dados iniciais para evitar problemas de codificação no terminal.
- O caractere `|` não deve ser usado nos campos, pois é o separador do arquivo.

## Conceitos praticados

`struct` • ponteiros • arquivos texto (`fopen`, `fgets`, `fprintf`, `sscanf`, `remove`, `rename`) • Bubble Sort • `switch/case` • funções • comparação de strings *case-insensitive*
