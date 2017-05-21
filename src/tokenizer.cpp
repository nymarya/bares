/**
 * @file tokenizer.cpp
 * @authors Gabriel Araújo de Souza e Mayra Dantas de Azevedo
 * @date 21 Maio 2017
 * @brief Arquivo contendo as implementações da classe Tokenizer.
 */

#include "../include/tokenizer.h"

/// Converte um caractere válido para seu correspondente  em terminal symbol.
Tokenizer::terminal_symbol_t  Tokenizer::lexer( char c_ ) const
{
    switch( c_ )
    {
        case '+':  return terminal_symbol_t::TS_PLUS;
        case '-':  return terminal_symbol_t::TS_MINUS;
        case '%':  return terminal_symbol_t::TS_MOD;
        case '/':  return terminal_symbol_t::TS_SLASH;
        case '*':  return terminal_symbol_t::TS_ASTERISK;
        case '^':  return terminal_symbol_t::TS_CARRET;
        case ')':  return terminal_symbol_t::TS_CLOSING_SCOPE;
        case '(':  return terminal_symbol_t::TS_OPENING_SCOPE;
        case ' ':  return terminal_symbol_t::TS_WS;
        case   9:  return terminal_symbol_t::TS_TAB;
        case '0':  return terminal_symbol_t::TS_ZERO;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':  return terminal_symbol_t::TS_NON_ZERO_DIGIT;
        case '\0': return terminal_symbol_t::TS_EOS; // end of string: the $ terminal symbol
    }
    return terminal_symbol_t::TS_INVALID;
}


/// Converte um terminal symbol para seu correspondente em string.
std::string Tokenizer::token_str( terminal_symbol_t s_ ) const
{
    switch( s_ )
    {
        case terminal_symbol_t::TS_PLUS      : return "+";
        case terminal_symbol_t::TS_MINUS     : return "-";
        case terminal_symbol_t::TS_MOD       : return "%";
        case terminal_symbol_t::TS_SLASH     : return "/";
        case terminal_symbol_t::TS_ASTERISK  : return "*";
        case terminal_symbol_t::TS_CARRET    : return "^";
        case terminal_symbol_t::TS_CLOSING_SCOPE 
                                             : return ")";
        case terminal_symbol_t::TS_OPENING_SCOPE 
                                             : return "(";
        case terminal_symbol_t::TS_WS        : return " ";
        case terminal_symbol_t::TS_ZERO      : return "0";
        default                              : return "X";
    }
}

//<! Consome um caractere válido para a expressão a ser parsed.
void Tokenizer::next_symbol( void )
{
    //<! Pega um símbolo válido para processar
    std::advance( it_curr_symb, 1 );
}

//<! Verifica se o caractere atual é igual ao simbolo solicitado
bool Tokenizer::peek( terminal_symbol_t c_ ) const
{
    return ( not end_input() ) and lexer( *it_curr_symb ) == c_;
}

//<!  Tenta aceitar o símbolo solicitado
bool Tokenizer::accept( terminal_symbol_t c_ )
{
    if ( peek(c_))
    {
        next_symbol();
        return true;
    }

    return false;
}

//<! Ignora qualquer espaço/Tab e tenta aceitar o símbolo solicitado.
// !  se for aceito, avança para o próximo caractere
bool Tokenizer::expect( terminal_symbol_t c_ )
{
    skip_ws();
    return accept(c_); // Stub
}

//<! Ignora qualquer espaço/Tab e para no próximo caractere
void Tokenizer::skip_ws( void )
{
    //<! Avança enquanto houver espaço ou tab
    while ( (not end_input()) and 
            ((lexer( *it_curr_symb ) == Tokenizer::terminal_symbol_t::TS_WS) or
             (lexer( *it_curr_symb ) == Tokenizer::terminal_symbol_t::TS_TAB )) )
    {
        next_symbol();
    }
}

//<! Verifica se chegamos ao final da seqüência de expressão
bool Tokenizer::end_input( void ) const
{
    return it_curr_symb == expr.end(); // Stub
}

/// Converte de String para inteiro
Tokenizer::input_int_type str_to_int( std::string input_str_ )
{
    // Creaando uma string de entrada
    std::istringstream iss( input_str_ );

    // Valor resultante.
    Tokenizer::input_int_type value;
    iss >> value; // Ignore os espaços em branco

    // Check erro durante a converção
    if ( iss.fail() )
        throw std::runtime_error( "str_to_int(): Erro, illegal integer format." );

    return value;
}

//=== NTS methods.

//<! <expr> := <term>,{ ("+"|"-"|"*"|"/"|"%"|"^"),<term> }
//<! Resolve a expressão
Tokenizer::Result Tokenizer::expression()
{
    //ignora espaço em branco
    skip_ws();
    //validar um termo
    auto result = term();
    //resultado ok, pode vir +/- <term>
    while ( result.type == Result::OK)
    {

        //pode vir o '+'
        if (expect(terminal_symbol_t::TS_PLUS))
        {
            // Token "+", Operator
            token_list.push_back( 
                Token(token_str(terminal_symbol_t::TS_PLUS), Token::token_t::OPERATOR));

        }
        else if ( expect(terminal_symbol_t::TS_MINUS))
        {
            // Token "-", Operator
            token_list.push_back( 
                Token(token_str(terminal_symbol_t::TS_MINUS), Token::token_t::OPERATOR));   
        }
        else if ( expect(terminal_symbol_t::TS_ASTERISK))
        {
            // Token "*", Operator
            token_list.push_back( 
                Token(token_str(terminal_symbol_t::TS_ASTERISK), Token::token_t::OPERATOR));
        }
        else if ( expect(terminal_symbol_t::TS_SLASH))
        {
            // Token "/", Operator
            token_list.push_back( 
                Token(token_str(terminal_symbol_t::TS_SLASH), Token::token_t::OPERATOR));
        }
         else if ( expect(terminal_symbol_t::TS_MOD))
        {
            // Token "%", Operator
            token_list.push_back( 
                Token(token_str(terminal_symbol_t::TS_MOD), Token::token_t::OPERATOR));
        }
        else if ( expect(terminal_symbol_t::TS_CARRET))
        {
            // Token "^", Operator
            token_list.push_back( 
                Token(token_str(terminal_symbol_t::TS_CARRET), Token::token_t::OPERATOR));
        }
        else 
        {
            return result;
        }

        result = term();
        if ( result.type != Result::OK and result.type != Result::INTEGER_OUT_OF_RANGE and end_input())
        {
            result.type = Result::MISSING_TERM;
            return result;
        }
    }

    return result;
}

//<! <term> := "(",<expr>,")" | <integer>
//<! Verifica se é termo
Tokenizer::Result Tokenizer::term()
{
    skip_ws();
    std::string::iterator it_begin =  it_curr_symb;

    Result result = Result( Result::MISSING_TERM, std::distance( expr.begin(), it_curr_symb) +1 );
    //Pode vir um "("
    if( expect(terminal_symbol_t::TS_OPENING_SCOPE)){
        token_list.push_back( 
                           Token( token_str(terminal_symbol_t::TS_OPENING_SCOPE), Token::token_t::OPENING_SCOPE));
        result = expression();
        //result = Result( Result::OK, std::distance( expr.begin(), it_curr_symb) );
        if(result.type == Result::OK){
            if( not expect(terminal_symbol_t::TS_CLOSING_SCOPE))
                return Result( Result::MISSING_CLOSING_PARENTHESIS, std::distance( expr.begin(), it_curr_symb) );
            //Se for ")", adiciona à lista de tokens
            token_list.push_back( 
                           Token( token_str(terminal_symbol_t::TS_CLOSING_SCOPE), Token::token_t::CLOSING_SCOPE));
        }
    } else{
        result =  integer();

        if( result.type == Result::OK ){
            //Salva número recuperado com string
            std::string num;
            num.insert(num.begin(), it_begin+result.at_col, it_curr_symb);

            //Testa se num está no limite de required_int_type
            input_int_type value = std::stoll(num);
            if( value <= std::numeric_limits< Tokenizer::required_int_type >::max() 
                and value >= std::numeric_limits< Tokenizer::required_int_type >::min()){

                token_list.push_back( 
                           Token( num, Token::token_t::OPERAND));
                
            } else{
                result.type = Result::INTEGER_OUT_OF_RANGE;
                result.at_col = std::distance( expr.begin(), it_begin) + 1;
            }
        }
    }
  
    return result;
}

//<! <integer> := 0 | ["-"],<natural_number>;
//<! verifica se é inteiro
Tokenizer::Result Tokenizer::integer()
{
    if ( accept(terminal_symbol_t::TS_ZERO) )
    {
        return Result( Result::OK );
    }
    
    //Pode vir vários "-"
    auto cont(0);
    while( expect(terminal_symbol_t::TS_MINUS) ){
        cont++;
    }
    //Se o número de "-" for par, o número será positivo
    //Ou seja, todos os "-" serão ignorados
    //Se for ímpar, o número será negativo
    //Ou seja, apenas o último "-" vai ser considerado
    if(cont % 2 == 1)
        cont-=1;

    auto result =  natural_number();

    // Se o resultado for ok, conta quantos "-" vai ter que pular
    if(result.type == Result::OK)
        result.at_col = cont;

    return result; 

}

//<natural_number> := <digit_excl_zero>,{<digit>}
//<! verifica se é número natural
Tokenizer::Result Tokenizer::natural_number()
{
    auto result = digit_excl_zero();
    if( result) {
    
        while( result){
            result = digit();
        }

        return Result( Result::OK );
    }

    return Result( Result::ILL_FORMED_INTEGER, std::distance( expr.begin(), it_curr_symb) +1);
}

//TS methods

//<! <digit_excl_zero> := "1"|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9"
//<! Verifica se é dígito exceto zero
bool Tokenizer::digit_excl_zero()
{  
    return accept( terminal_symbol_t::TS_NON_ZERO_DIGIT );
}

//<! <digit> := "0" | <digit_excl_zero>
//<! Verifica se é dígito
bool Tokenizer::digit()
{ 
    return (accept( terminal_symbol_t::TS_ZERO ) or digit_excl_zero() );
}

//<! Recebe uma expressão, realiza o parsing e retorna o resultado.
Tokenizer::Result
Tokenizer::parse( std::string e_ )
{
    // We reset the parsing process each new expression.
    expr = e_;  // The expression in a string.
    it_curr_symb = expr.begin(); // Iterator to the 1st character in the expression.
    token_list.clear(); // Clear the list of tokens.

    // Default result.
    Result result( Result::OK );

    skip_ws();
    if ( end_input() )
    {
        return Result ( Result::UNEXPECTED_END_OF_EXPRESSION, 
                              std::distance( expr.begin(), it_curr_symb ) );
    }

    // tentar validar a expressão
    result = expression();

    if( result.type == Result::OK){

        skip_ws();
        if( not end_input()){
            token_list.clear();
            return Result( Result::EXTRANEOUS_SYMBOL, std::distance(expr.begin(), it_curr_symb) + 1);
        }

    }
    return result;


}

//<! Pega a lista de Tokens
std::vector< Token >
Tokenizer::get_tokens( void ) const
{
    return token_list;
}



//==========================[ Fim do parse.cpp ]==========================//
