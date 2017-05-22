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
            std::cout << ">>> Division by zero!\n";
            break;
        case Bares::Result::NUMERIC_OVERFLOW:
            std::cout << ">>> Numeric overflow error!\n";
            break;
        default:
            std::cout << ">>> Unhandled error found!\n";
            break;
    }
}

/**
 * @brief      Imprime menssagens de erro do Tokenizer
 *
 * @param[in]  result  The resultado do Tokenizer
 * @param[in]  str     A string da posição do erro
 */
void print_msg( const Tokenizer::Result & result, std::string str )
{
    std::string error_indicator( str.size(), ' ');

    // Have we got a parsing error?
    error_indicator[result.at_col -1] = '^';
    switch ( result.type )
    {
        case Tokenizer::Result::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << ">>> Unexpected end of input at column (" << result.at_col << ")!\n";
            break;
        case Tokenizer::Result::ILL_FORMED_INTEGER:
            std::cout << ">>> Ill formed integer at column (" << result.at_col << ")!\n";
            break;
        case Tokenizer::Result::MISSING_TERM:
            std::cout << ">>> Missing <term> at column (" << result.at_col << ")!\n";
            break;
        case Tokenizer::Result::EXTRANEOUS_SYMBOL:
            std::cout << ">>> Extraneous symbol after valid expression found at column (" << result.at_col << ")!\n";
            break;
        case Tokenizer::Result::MISSING_CLOSING_PARENTHESIS:
            std::cout << ">>> Missing closing \")\" at column (" << result.at_col << ")!\n";
            break;
        case Tokenizer::Result::INTEGER_OUT_OF_RANGE:
            std::cout << ">>> Integer constant out of range beginning at column (" << result.at_col << ")!\n";
            break;
        default:
            std::cout << ">>> Unhandled error found!\n";
            break;
    }

    std::cout << "\"" << str << "\"\n";
    std::cout << " " << error_indicator << std::endl;
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

    std::cout << "expressões lidas:\n ";
    for ( auto i( expressions.begin() ); i != expressions.end(); i++)
    {
        std::cout << "' " << *i << " '" << std::endl;
    }

    Tokenizer my_parser; // Instancia um parser.
    // Tentar analisar cada expressão da lista.
    for( const auto & expr : expressions )
    {
        // Fazer o parsing desta expressão.
        auto result = my_parser.parse( expr );
        // Preparar cabeçalho da saida.
        std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";
        // Se deu pau, imprimir a mensagem adequada.
        if ( result.type != Tokenizer::Result::OK )
            print_msg( result, expr );
        else{
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";
        
            // Recuperar a lista de tokens.
            auto lista = my_parser.get_tokens();

            //Avaliar expressão
            Bares bares;
            auto result_ = bares.evaluate( lista );

            //Imprimir mensagem de erro
            if ( result_.type_b != Bares::Result::OK )
                print_msg_bares( result_ );
            else
                std::cout << ">>> Result is: " << result_.value_b << std::endl;
        }

    }

    std::cout << "\n>>> Normal parsing...\n";

    return EXIT_SUCCESS;
}
