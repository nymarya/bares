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

- Durante a validação da expressõe, os seguintes erros podem ocorrer:


`Constante inteira fora do intervalo começando na coluna(n)!`

O operando que se inicia na coluna *n* está fora da faixa permitida.<br/>
Ex.: 1000000 − 2, coluna 1.


`Faltando <termo> na coluna(n)!` 

A partir da coluna *n* está faltando o resto da expressão .<br/>
Ex.: 2 +, coluna 4.


`Símbolo inesperado após expressão válida encontrado na coluna(n)!`

Existe um sı́mbolo qualquer (válido ou não) que foi encontrado depois que uma expressão completa foi validada, na coluna *n*.<br/>
Ex.: 2 = 3, coluna 3; ou 2 + 3 4, coluna 7.


`Inteiro mal formado na coluna (n)!` 

Expressão contém constante inteira iniciada na coluna *n* que possui sı́mbolo inválido em sua composição.<br/>
Ex.: − 5, coluna 1; ou ! 3, coluna 1; ou ) 2 + 4, coluna 1.

`Faltando símbolo ")" na coluna(n)!` 

Está faltando um parêntese de fechamento ‘)’ para um parêntese de abertura ‘(’ correspondente, na coluna *n*.<br/>
Ex.: ((2 % 3) ∗ 8, coluna 13.

`Final inesperado de expressão na coluna(n)!` 

Expressão corresponde a uma linha contendo apenas espaços, cujo final é encontrado na coluna *n*.<br/>
Ex.: "    ", coluna 4 ou "   (", coluna 4.

- Os seguintes erros podem ocorrer durante o cálculo da expressão

`Divisão por zero!` 

Durante a avaliação, o quociente de alguma divisão é zero.<br/>
Ex.: 3/(1 − 1); ou 10/(3 ∗ 3ˆ−2).

`Erro de sobrecarga numérica!` 

Acontece quando uma operação dentro da expressão ou o resultado da expressão inteira estoura o limite das constantes numéricas definidos na descrição do programa.<br/>
Ex.: 20 ∗ 20000.


## Autoria

Desenvolvido por Gabriel Araújo de Souza (<gabriel_feg@hotmail.com >) e Mayra Dantas de Azevedo (<mayradazevedo@gmail.com >)

2017.1

&copy; IMD/UFRN 2017.
