/**
 * @file driver_bares.cpp
 * @authors Gabriel Araújo de Souza e Mayra Dantas de Azevedo
 * @date 21 Maio 2017
 * @brief Arquivo que utiliza dos métodos implementados para resolver 
 *        uma expressão enviada pelo usuário.
 */

/*!
 * Infix to postfix conversion in C++
 * Input Postfix expression must be in a desired format.
 * Operands and operator, **both must be single character**.
 * Only '+', '-', '*', '%', '/', and '^' (for exponentiation) operators are expected;
 * Any other character is just ignored.
 */

#include <iostream>  // cout, endl
#include <sstream>   // getline
#include <stack>     // stack
#include <string>    // string
#include <iomanip>   //setfill, setw

#include "tokenizer.h"
#include "bares.h"

using value_type = long int;

/**
 * @brief      Imprime menssagens de erro do Bares
 *
 * @param[in]  result  O Resultado do bares
 */
void print_msg_bares ( const Bares::Result & result)
{
    switch ( result.type_b )
    {
        case Bares::Result::DIVISION_BY_ZERO:
            std::cout << "Divisão por zero!\n";
            break;
        case Bares::Result::NUMERIC_OVERFLOW:
            std::cout << "Erro de sobrecarga numérica!\n";
            break;
        default:
            std::cout << "Erro sem tratamento!\n";
            break;
    }
}

/**
 * @brief      Imprime menssagens de erro do Tokenizer
 *
 * @param[in]  result  The resultado do Tokenizer
 * @param[in]  str     A string da posição do erro
 */
void print_msg( const Tokenizer::Result & result)
{
    //std::string error_indicator( str.size(), ' ');

    // Have we got a parsing error?
    //error_indicator[result.at_col -1] = '^';
    switch ( result.type )
    {
        case Tokenizer::Result::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << "Final inesperado de expressão na coluna (" << result.at_col << ")!\n";
            break;
        case Tokenizer::Result::ILL_FORMED_INTEGER:
            std::cout << "Inteiro mal formado na coluna (" << result.at_col << ")!\n";
            break;
        case Tokenizer::Result::MISSING_TERM:
            std::cout << "Faltando <termo> na coluna (" << result.at_col << ")!\n";
            break;
        case Tokenizer::Result::EXTRANEOUS_SYMBOL:
            std::cout << "Símbolo inesperado após expressão válida encontrado na coluna (" << result.at_col << ")!\n";
            break;
        case Tokenizer::Result::MISSING_CLOSING_PARENTHESIS:
            std::cout << "Faltando símbolo \")\" na coluna (" << result.at_col << ")!\n";
            break;
        case Tokenizer::Result::INTEGER_OUT_OF_RANGE:
            std::cout << "Constante inteira fora do intervalo começando na coluna (" << result.at_col << ")!\n";
            break;
        default:
            std::cout << "Erro sem tratamento!\n";
            break;
    }
}

/**
 * @brief      Programa principal
 *
 * @return     Execução terminada
 */
int main()
{
    //lista com as expressões
    std::vector<std::string> expressions;
    std::string aux;

    while ( std::getline(std::cin, aux) )
    {
        expressions.push_back( aux );
    }

    Tokenizer my_parser; // Instancia um parser.
    // Tentar analisar cada expressão da lista.
    for( const auto & expr : expressions )
    {
        // Fazer o parsing desta expressão.
        auto result = my_parser.parse( expr );
        // Se houver erro, imprimir a mensagem adequada.
        if ( result.type != Tokenizer::Result::OK )
            print_msg( result);
        else{
        
            // Recuperar a lista de tokens.
            auto lista = my_parser.get_tokens();

            //Avaliar expressão
            Bares bares;
            auto result_ = bares.evaluate( lista );

            //Imprimir mensagem de erro
            if ( result_.type_b != Bares::Result::OK )
                print_msg_bares( result_ );
            else
                std::cout << result_.value_b << std::endl;
        }

    }

    return EXIT_SUCCESS;
}
