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
#include <typeinfo>

using value_type = long int;



void print_msg( const Tokenizer::Result & result, std::string str )
{
    std::string error_indicator( str.size()+1, ' ');

    // Have we got a parsing error?
    error_indicator[result.at_col] = '^';
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
            std::cout << ">>> Tokens: { ";
            std::copy( lista.begin(), lista.end(),
                    std::ostream_iterator< Token >(std::cout, " ") );
            std::cout << "}\n";

            //Aqui converteria a lista para <string>. Ou não.....
            //Aqui usaria o bares
            Bares bares;
            //auto result = bares.evaluate( lista );
            //std::cout << ">>> Result is: " << result << std::endl;
        }

    }

    std::cout << "\n>>> Normal parsing...\n";

    return EXIT_SUCCESS;
}
