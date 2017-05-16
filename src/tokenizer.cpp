#include "../include/tokenizer.h"
#include <limits> //numeric_limits

/// Converts a valid character to the corresponding terminal symbol.
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


/// Convert a terminal symbol into its corresponding string representation.
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

/// Consumes a valid character from the expression being parsed.
void Tokenizer::next_symbol( void )
{
    // Get a valid symbol for processing
    std::advance( it_curr_symb, 1 );
}

/// Verifies whether the current symbol is equal to the terminal symbol requested.
bool Tokenizer::peek( terminal_symbol_t c_ ) const
{
    return ( not end_input() ) and lexer( *it_curr_symb ) == c_;
}

/// Tries to match the current character to a symbol passed as argument.
bool Tokenizer::accept( terminal_symbol_t c_ )
{
    if ( peek(c_))
    {
        next_symbol();
        return true;
    }

    return false;
}

/// Verify whether the next valid symbol is the one expected; if it is so, the method accepts it.
bool Tokenizer::expect( terminal_symbol_t c_ )
{
    // TODO
    skip_ws();
    return accept(c_); // Stub
}


/// Ignores any white space or tabs in the expression until reach a valid symbol or end of input.
void Tokenizer::skip_ws( void )
{
    while ( (not end_input()) and 
            ((lexer( *it_curr_symb ) == Tokenizer::terminal_symbol_t::TS_WS) or
             (lexer( *it_curr_symb ) == Tokenizer::terminal_symbol_t::TS_TAB )) )
    {
        next_symbol();
    }
}

/// Checks whether we reached the end of the expression string.
bool Tokenizer::end_input( void ) const
{
    return it_curr_symb == expr.end(); // Stub
}

/// Converts from string to integer.
Tokenizer::input_int_type str_to_int( std::string input_str_ )
{
    // Creating input stream.
    std::istringstream iss( input_str_ );

    // Resulting value.
    Tokenizer::input_int_type value;
    iss >> value; // Ignore trailling white space.

    // Check for error during convertion.
    if ( iss.fail() )
        throw std::runtime_error( "str_to_int(): Erro, illegal integer format." );

    return value;
}

//=== NTS methods.
Tokenizer::Result Tokenizer::expression()
{
    //ignora espaço em branco
    skip_ws();

    if ( expect(terminal_symbol_t::TS_OPENING_SCOPE))
    {
        // Token "^", Operator
        token_list.push_back( 
            Token(token_str(terminal_symbol_t::TS_OPENING_SCOPE), Token::token_t::OPENING_SCOPE));
    }
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
        else if ( expect(terminal_symbol_t::TS_CARRET))
        {
            // Token "^", Operator
            token_list.push_back( 
                Token(token_str(terminal_symbol_t::TS_CARRET), Token::token_t::OPERATOR));
        }
        else if ( expect(terminal_symbol_t::TS_CLOSING_SCOPE))
        {
            // Token "^", Operator
            token_list.push_back( 
                Token(token_str(terminal_symbol_t::TS_CLOSING_SCOPE), Token::token_t::CLOSING_SCOPE));
        }
        else 
        {
            return result;
        }

        if ( expect(terminal_symbol_t::TS_OPENING_SCOPE))
        {
            // Token "^", Operator
            token_list.push_back( 
                Token(token_str(terminal_symbol_t::TS_OPENING_SCOPE), Token::token_t::OPENING_SCOPE));
        }
        result = term();
        if ( result.type != Result::OK and result.type != Result::INTEGER_OUT_OF_RANGE)
        {
            //result.type = Result::MISSING_TERM;
            return result;
        }
    }

    return result;
}

Tokenizer::Result Tokenizer::term()
{
    skip_ws();
    std::string::iterator it_begin =  it_curr_symb;
    auto result =  integer();

    std::string num;
    num.insert(num.begin(), it_begin, it_curr_symb);

    if( result.type == Result::OK ){
        std::string num;
        num.insert(num.begin(), it_begin, it_curr_symb);

        //Testa se num está no limite de required_int_type
        input_int_type value = std::stoll(num);
        if( value <= std::numeric_limits< Tokenizer::required_int_type >::max() 
            and value >= std::numeric_limits< Tokenizer::required_int_type >::min()){

            token_list.push_back( 
                       Token( num, Token::token_t::OPERAND));
            
        } else{
            result.type = Result::INTEGER_OUT_OF_RANGE;
            result.at_col = std::distance( expr.begin(), it_begin);
        }
    }

    return result;
}

Tokenizer::Result Tokenizer::integer()
{
    // TODO
    std::string num;
    if ( accept(terminal_symbol_t::TS_ZERO) )
    {
    
        return Result( Result::OK );
    }    


    accept(terminal_symbol_t::TS_MINUS);

    return natural_number();
}

Tokenizer::Result Tokenizer::natural_number()
{

    auto result = digit_excl_zero();
    if( result) {
    
        while( result){
            result = digit();
        }

        return Result( Result::OK );
    }

    return Result( Result::ILL_FORMED_INTEGER, std::distance( expr.begin(), it_curr_symb) );
}

//TS methods
bool Tokenizer::digit_excl_zero()
{
   
    return accept( terminal_symbol_t::TS_NON_ZERO_DIGIT );
}

bool Tokenizer::digit()
{
    
    return (accept( terminal_symbol_t::TS_ZERO ) or digit_excl_zero() );
}

/*!
 * This is the Tokenizer's entry point.
 * This method tries to (recursivelly) validate the expression.
 * During this process, we also stored the tokens into a container.
 *
 * \param e_ The string with the expression to parse.
 * \return The parsing result.
 */
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
            return Result( Result::EXTRANEOUS_SYMBOL, std::distance(expr.begin(), it_curr_symb));
        }

    }
    return result;


}
/// Return the list of tokens, which is the by-product created during the syntax analysis.
std::vector< Token >
Tokenizer::get_tokens( void ) const
{
    return token_list;
}



//==========================[ End of parse.cpp ]==========================//
