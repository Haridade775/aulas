# Vitória Calçados

Sistema simples de loja de calçados feito em **C**, para rodar no terminal.
Permite cadastrar calçados, controlar o estoque, registrar vendas e ver um relatório.

## Funcionalidades

- Cadastrar calçado (nome, tamanho, preço e quantidade em estoque)
- Listar todos os calçados cadastrados
- Realizar venda, descontando do estoque
- Repor estoque
- Relatório com modelos cadastrados, pares em estoque, valor do estoque e total vendido

## Requisitos

- Compilador C, como o `gcc`
- Terminal com suporte a UTF-8 (para mostrar acentos corretamente)

## Como compilar e executar

```bash
gcc vitoria_calcados.c -o vitoria_calcados
./vitoria_calcados
```

No Windows (com MinGW), execute `vitoria_calcados.exe`.

## Menu

```
===== Vitória Calçados =====
1 - Cadastrar calçado
2 - Listar calçados
3 - Realizar venda
4 - Repor estoque
5 - Relatório
0 - Sair
```

## Funções do código

| Função | Para que serve |
| --- | --- |
| `ler_texto` | Lê uma linha de texto e remove o "Enter" do final |
| `ler_int` | Lê um número inteiro digitado pelo usuário |
| `ler_float` | Lê um número decimal (aceita vírgula ou ponto) |
| `buscar` | Procura um produto pelo código |
| `cadastrar` | Cadastra um novo calçado |
| `listar` | Mostra a tabela de calçados |
| `vender` | Registra uma venda e atualiza o estoque |
| `repor` | Adiciona quantidade ao estoque de um produto |
| `relatorio` | Mostra o resumo da loja |
| `main` | Exibe o menu e chama a função escolhida |

## Limitações

- Os dados ficam apenas na memória: ao fechar o programa, tudo é apagado.
- Máximo de 100 produtos cadastrados (`MAX_PRODUTOS`).

## Ideias para evoluir

- Salvar e carregar os produtos em arquivo
- Editar e excluir produtos
- Login de vendedor e descontos
