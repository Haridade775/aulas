# Vitória Calçados - Preço justo que cabe no bolso

Sistema de loja de bairro feito em **C**, para rodar no terminal.
A Vitória Calçados vende sapatos, tênis, sandálias, chinelos e acessórios com **preço justo, que cabe no bolso**. Por isso o sistema tem a opção **"Cabe no bolso"**: o cliente diz quanto quer gastar e o programa mostra o que dá para levar, do mais barato ao mais caro.

## Funcionalidades

- **CRUD**: cadastrar, listar, atualizar e excluir produtos
- **ID automático** e **nome único** (não aceita dois produtos com o mesmo nome)
- **Busca** por nome (aceita parte do nome) e por categoria
- **4 ordenações com Bubble Sort**: nome, categoria, preço e estoque
- **Vendas**: desconta do estoque e soma nas vendas do produto
- **Cabe no bolso**: lista os produtos até o valor que o cliente quer gastar
- **Arquivos**: os dados são salvos em `vitoria.txt` e carregados ao abrir o programa
- **Exportar CSV**: gera `vitoria.csv` para abrir em planilhas

## Como compilar e executar

```bash
gcc vitoria_calcados.c -o vitoria_calcados
./vitoria_calcados
```

No Windows (MinGW), execute `vitoria_calcados.exe`. Use um terminal com UTF-8 para os acentos aparecerem corretamente.

## Menu

```
===== VITÓRIA CALÇADOS - PREÇO JUSTO QUE CABE NO BOLSO =====
 1 - Cadastrar produto
 2 - Listar produtos
 3 - Atualizar produto
 4 - Excluir produto
 5 - Buscar por nome
 6 - Buscar por categoria
 7 - Ordenar listagem
 8 - Registrar venda
 9 - Cabe no bolso (até quanto quer gastar)
10 - Exportar CSV
 0 - Sair
```

## Estrutura do código

- `struct Produto`: id, nome, categoria, estoque, vendidos e preço
- `struct Loja`: lista dinâmica de produtos (ponteiro, total e capacidade)

## Funções

| Função | Para que serve |
| --- | --- |
| `ler_texto`, `ler_int`, `ler_float` | Leem o que o usuário digita (texto, inteiro e decimal) |
| `minusculo`, `comparar`, `contem` | Comparam e procuram textos sem diferenciar maiúsculas de minúsculas |
| `adicionar` | Coloca um produto na lista, aumentando a memória quando precisa |
| `buscar_id` | Encontra um produto pelo ID |
| `nome_existe` | Verifica se o nome já está em uso |
| `proximo_id` | Gera o ID automático |
| `salvar`, `carregar` | Gravam e leem os produtos no arquivo `vitoria.txt` |
| `dados_iniciais` | Cria os produtos de exemplo na primeira execução |
| `exportar_csv` | Gera o arquivo `vitoria.csv` |
| `cabecalho_tabela`, `mostrar`, `listar` | Mostram os produtos em forma de tabela |
| `cadastrar`, `atualizar`, `excluir` | Create, Update e Delete do CRUD |
| `buscar` | Busca por nome ou por categoria |
| `deve_trocar`, `bubble_sort`, `ordenar` | Ordenam a listagem usando Bubble Sort |
| `vender` | Registra uma venda |
| `cabe_no_bolso` | Mostra os produtos que cabem no valor que o cliente quer gastar |
| `main` | Mostra o menu e chama a função escolhida |

## Produtos iniciais

Sapato Social Masculino, Tênis Casual, Chinelo de Dedo, Cinto de Couro e Sandália Rasteira.
