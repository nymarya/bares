# BARES

## Sobre o projeto

O programa BARES (Basic ARithmetic Expression Evaluator based on Stacks) consiste em um avaliador aritmético capaz
de receber expressões aritméticas simples, formadas por constantes numéricas inteiras entre -32.768 e 32.767, operadores(+, -, /, *, %, ^) e parênteses.

## Como executar

|  Comando           | Descrição  |
| :-----| :-------------|
| ```$ make``` | Compilar | 

Existem 3 maneiras de executar o BARES:

- Expressões passadas no terminal

|  Comando           | Descrição  |
| :-----| :-------------|
| ```$ ./parser ```       | Executar  | 

Com esse comando, o usuário deve digitar cada expressão no terminal. Cada linha representará uma expressão e, ao final, basta pressionar `CRTL+D` para que as entradas sejam avaliadas e os resultados sejam exibidos no terminal.

- Utilizando expressões salvas em arquivo

|  Comando           | Descrição  |
| :-----| :-------------|
| ```$ ./parser <  arquivo_entrada```       | Executar  | 

Com esse comando, os resultados das avaliações das expressões serão exibidos no prório terminal.

- Utilizando expressões escritas em arquivo e salvando

|  Comando           | Descrição  |
| :-----| :-------------|
| ```$ ./parser <  arquivo_entrada > arquivo_saida```       | Executar  |

Neste caso, os resultados das avaliações das expressões serão escritos no arquivo especificado com sendo o de saída.


#### Exemplo de entradas válidas
```
25 / 5 + 4 * 8
(2+3)*8
5 % 2 ^4
(5 % 3) ^4
-----3
```

## Possíveis erros



## Autoria

Desenvolvido por Gabriel Araújo de Souza (<gabriel_feg@hotmail.com >) e Mayra Dantas de Azevedo (<mayradazevedo@gmail.com >)

2017.1

&copy; IMD/UFRN 2017.
