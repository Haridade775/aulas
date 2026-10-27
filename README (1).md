# Cadastro de Loja em C — Atividade Avaliativa 04

Programa em C que une **algoritmos de ordenação** (Bubble Sort) com **manipulação de arquivos**, usando **estruturas (`struct`)** e **ponteiros**. Ele gerencia o cadastro genérico de uma loja, com os dados guardados em um arquivo de texto.

## Arquivos

| Arquivo | Descrição |
|---|---|
| `cadastro_loja.c` | Código-fonte completo, com comentários explicativos |
| `produtos.txt` | Base de dados em texto (criada pela opção 1 do menu) |
| `produtos.tmp` | Arquivo temporário usado em alterar/excluir (some após a operação) |
| `produtos.csv` | Exportação em CSV (criada pela opção 12) |

## Como compilar e executar

```bash
gcc cadastro_loja.c -o cadastro_loja -Wall
./cadastro_loja        # no Windows: cadastro_loja.exe
```

Na primeira execução, escolha a opção **1** para criar o `produtos.txt` com os 4 produtos iniciais.

## Menu

| Opção | Função |
|---|---|
| 1 | Criar arquivo com os dados iniciais |
| 2 | Listar produtos |
| 3 | Inserir produto |
| 4 | Alterar produto (Enter mantém o valor atual) |
| 5 | Excluir produto (pede confirmação) |
| 6 | Buscar por nome |
| 7 | Buscar por categoria |
| 8 | Ordenar por nome (A–Z) |
| 9 | Ordenar por nome (Z–A) |
| 10 | Ordenar por preço (menor → maior) |
| 11 | Ordenar por preço (maior → menor) |
| 12 | Exportar para CSV |
| 0 | Sair |

## Dados iniciais

```
---------------------------------------------------------------------------
| ID | Produto          | Categoria       | Preço (R$) | Quantidade | Fornecedor     |
---------------------------------------------------------------------------
| 1  | Camiseta Básica  | Vestuário       | 49.90      | 30         | Moda Brasil    |
| 2  | Mouse Óptico USB | Informática     | 59.99      | 15         | TecnoShop      |
| 3  | Shampoo 300ml    | Higiene Pessoal | 22.50      | 40         | Beleza Pura    |
| 4  | Café 500g        | Alimentos       | 18.00      | 25         | Grãos do Sul   |
---------------------------------------------------------------------------
```

No programa, os textos são gravados sem acentos para evitar problemas de codificação no terminal.

## Como funciona

- **ID automático:** o maior ID existente + 1. Na inserção também há uma checagem extra de ID duplicado.
- **Duplicatas:** não é possível cadastrar (nem renomear para) um nome já existente. A comparação ignora maiúsculas e minúsculas.
- **Buscas:** parciais e *case-insensitive* (ex.: `CAFE` encontra "Café 500g").
- **Alterar/excluir:** os dados são gravados em `produtos.tmp`, o original é apagado e o temporário é renomeado. Isso evita corromper o arquivo se algo falhar no meio.
- **Ordenação:** Bubble Sort genérico, que recebe a função de comparação por **ponteiro para função**. Ordena uma **cópia** do vetor e mostra o resultado em tabela, **sem alterar o arquivo**.
- **Memória:** os produtos são carregados em um vetor de `Produto` alocado com `malloc`/`realloc` e liberado com `free`.
- **CSV:** separado por vírgula, com textos entre aspas.

## Formato do `produtos.txt`

Um produto por linha, campos separados por `;`:

```
id;nome;categoria;preco;quantidade;fornecedor
1;Camiseta Basica;Vestuario;49.90;30;Moda Brasil
```

O programa troca automaticamente um `;` digitado pelo usuário por `,`, para não quebrar o formato.

## Limites dos campos

Definidos por constantes no início do código: `TAM_NOME` (50), `TAM_CATEGORIA` (30) e `TAM_FORNECEDOR` (50). Se aumentar esses valores, ajuste também os limites do `sscanf` na função `carregar()` (`%49[^;]`, `%29[^;]`, `%49[^\n]`).
