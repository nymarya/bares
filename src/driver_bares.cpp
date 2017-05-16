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
#include <iomanip>
#include <cassert>   // assert
#include <cmath>     // pow
#include "parser.h"

using value_type = long int;

bool is_operator(char c);

bool is_operand( char c);

bool is_opening_scope( char c);

bool is_closing_scope(char c);

bool has_higher_precedence( char op1, char op2);

/// Converts a expression in infix notation to a corresponding profix representation.
std::string infix_to_postfix( std::string );

int char2int( char ch){
    return ch-'0';
}

int execute( value_type n1, value_type n2, char opr){

    value_type result(0);
    switch ( opr )
    {
        case '^' : result = static_cast<value_type>( pow( n1, n2 ) );
                   break;
        case '*' : result =  n1 * n2;
                   break;
        case '/' : if ( n2 == 0 )
                       throw std::runtime_error( "Division by zero" );
                   result = n1/n2;
                   break;
        case '%' : if ( n2 == 0 )
                       throw std::runtime_error( "Division by zero" );
                   result = n1%n2;
                   break;
        case '+' : result = n1 + n2;
                   break;
        case '-' : result =  n1 - n2;
                   break;
        default: assert(false);
    }

    return result;

}

int evaluate_postfix( std::string postfix){

    std::stack< int> s;

    for( auto ch: postfix){
        if( is_operand(ch)) s.push(   char2int(ch)   );

        else if( is_operator(ch) ){
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();

            auto result = execute(op1, op2, ch);
            s.push( result);
        }
        else {
            assert(false);
        }
    }

    return s.top(); 
}

void print_msg( const Parser::ParserResult & result, std::string str )
{
    std::string error_indicator( str.size()+1, ' ');

    // Have we got a parsing error?
    error_indicator[result.at_col] = '^';
    switch ( result.type )
    {
        case Parser::ParserResult::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << ">>> Unexpected end of input at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::ILL_FORMED_INTEGER:
            std::cout << ">>> Ill formed integer at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::MISSING_TERM:
            std::cout << ">>> Missing <term> at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::EXTRANEOUS_SYMBOL:
            std::cout << ">>> Extraneous symbol after valid expression found at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::MISSING_CLOSING_PARENTHESIS:
            std::cout << ">>> Missing closing \")\" at column (" << result.at_col << ")!\n";
            break;
        case Parser::ParserResult::INTEGER_OUT_OF_RANGE:
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

    Parser my_parser; // Instancia um parser.
    // Tentar analisar cada expressão da lista.
    for( const auto & expr : expressions )
    {
        // Fazer o parsing desta expressão.
        auto result = my_parser.parse( expr );
        // Preparar cabeçalho da saida.
        std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";
        // Se deu pau, imprimir a mensagem adequada.
        if ( result.type != Parser::ParserResult::PARSER_OK )
            print_msg( result, expr );
        else
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";

        // Recuperar a lista de tokens.
        auto lista = my_parser.get_tokens();
        std::cout << ">>> Tokens: { ";
        std::copy( lista.begin(), lista.end(),
                std::ostream_iterator< Token >(std::cout, " ") );
        std::cout << "}\n";
    }

    std::cout << "\n>>> Normal parsing...\n";

    std::string expression = "4/(5^2)+(6^2^3)";
    std::string expression2 = "1+ 3 * ( 4 + 8 * 3 ^7)";
    //std::string expression = "A+(B*C-(D/E^F)+G)*H";
    //
    auto postfix = infix_to_postfix(expression2);
    std::cout << ">>> Input (infix)    = " << expression2 << "\n";
    std::cout << ">>> Output (postfix) = " << postfix << "\n";
    auto result = evaluate_postfix( postfix );
    std::cout << ">>> Result is: " << result << std::endl;

    postfix = infix_to_postfix(expression);
    std::cout << ">>> Input (infix)    = " << expression << "\n";
    std::cout << ">>> Output (postfix) = " << postfix << "\n";

    result = evaluate_postfix( postfix );
    std::cout << ">>> Result is: " << result << std::endl;
    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}

std::string infix_to_postfix( std::string infix_ ){

    //Stores the postfix expression
    std::string postfix = "";

    //Stack to help us convert the exp
    std::stack< char > s;

    //Tranverse the expression
    for (char  ch : infix_){

        if( is_operand(ch))
            postfix += ch;
        else if ( is_operator(ch) ){

            //Pops out all the element with higher priority
            while( not s.empty() and has_higher_precedence(s.top(), ch) ){
                postfix += s.top();
                s.pop();
            }

            //the incoming operator always goes into the stack
            s.push(ch);
        }
        else if ( is_opening_scope(ch) ){
            s.push(ch);
        }
        else if ( is_closing_scope(ch) ){
            //pop out all the elemens that are not '('
            while( not is_opening_scope(s.top()) and not s.empty() ){
                postfix += s.top(); //goes to the output
                s.pop();    //pops out the element
            }
            s.pop(); //remove the '(' from the stack
        }
        else {

        }
    }

    while (not s.empty()){
        postfix += s.top();
        s.pop();
    }

    return postfix;
}

bool is_operator(char c){
    std::string operators = "+-/^*%";

    return (operators.find(c) != std::string::npos);
}

bool is_operand( char c){
    return ( c >= 48 and c <= 57);
}

bool is_opening_scope( char c){
    return (c == '(');
}

bool is_closing_scope(char c){
    return (c== ')');
}

bool is_right_association(char c){
    return c == '^';
}

int get_precedence( char c){
    int weigth = 0;
    switch( c ){
        case '^':
            weigth = 3;
            break;
        case '*':
            weigth = 2;
            break;
        case '/':
            weigth = 2;
            break;
        case '%':
            weigth = 2;
            break;
        case '+':
            weigth = 1;
            break;
        case '-':
            weigth = 1;
            break;
        case '(':
            weigth = 0;
            break;
        default:
            assert(false);
    }

    return weigth;
}

bool has_higher_precedence( char op1, char op2){

    auto p1 = get_precedence( op1 ); //Top
    auto p2 = get_precedence( op2 ); //New operator

    //special case:
    //Has the same precedence and is right association
    if(p1 == p2 and is_right_association( op1 )) 
        return false;
    

    return p1 >= p2;
}