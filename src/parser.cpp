#include "../include/parser.h"

/// Converts a valid character to the corresponding terminal symbol.
Parser::terminal_symbol_t  Parser::lexer( char c_ ) const
{
    switch( c_ )
    {
        case '+':  return terminal_symbol_t::TS_PLUS;
        case '-':  return terminal_symbol_t::TS_MINUS;
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
std::string Parser::token_str( terminal_symbol_t s_ ) const
{
    switch( s_ )
    {
        case terminal_symbol_t::TS_PLUS   : return "+";
        case terminal_symbol_t::TS_MINUS  : return "-";
        case terminal_symbol_t::TS_WS     : return " ";
        case terminal_symbol_t::TS_ZERO   : return "0";
        default                           : return "X";
    }
}

/// Consumes a valid character from the expression being parsed.
void Parser::next_symbol( void )
{
    // Get a valid symbol for processing
    std::advance( it_curr_symb, 1 );
}

/// Verifies whether the current symbol is equal to the terminal symbol requested.
bool Parser::peek( terminal_symbol_t c_ ) const
{
    // 
    return ( not end_input() ) and lexer( *it_curr_symb ) == c_;
}

/// Tries to match the current character to a symbol passed as argument.
bool Parser::accept( terminal_symbol_t c_ )
{
    // TODO
    if ( peek(c_))
    {
        next_symbol();
        return true;
    }

    return false;
}

/// Verify whether the next valid symbol is the one expected; if it is so, the method accepts it.
bool Parser::expect( terminal_symbol_t c_ )
{
    // TODO
    skip_ws();
    return accept(c_); // Stub
}


/// Ignores any white space or tabs in the expression until reach a valid symbol or end of input.
void Parser::skip_ws( void )
{
    // TODO
    while ( (not end_input()) and 
            ((lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_WS) or
             (lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_TAB )) )
    {
        next_symbol();
    }
}

/// Checks whether we reached the end of the expression string.
bool Parser::end_input( void ) const
{
    // TODO
    return it_curr_symb == expr.end(); // Stub
}

/// Converts from string to integer.
Parser::input_int_type str_to_int( std::string input_str_ )
{
    // Creating input stream.
    std::istringstream iss( input_str_ );

    // Resulting value.
    Parser::input_int_type value;
    iss >> value; // Ignore trailling white space.

    // Check for error during convertion.
    if ( iss.fail() )
        throw std::runtime_error( "str_to_int(): Erro, illegal integer format." );

    return value;
}

//=== NTS methods.
Parser::ParserResult Parser::expression()
{
    // TODO
    skip_ws();
    //validar um termo
    auto result = term();
    //resultado ok, pode vir +/- <term>
    while ( result.type == ParserResult::PARSER_OK)
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
        }else
        {
            return result;
        }


        result = term();
        if ( result.type != ParserResult::PARSER_OK)
        {
            result.type = ParserResult::MISSING_TERM;
            return result;
        }
    }

    return result;
}

Parser::ParserResult Parser::term()
{
    skip_ws();
    std::string::iterator it_begin =  it_curr_symb;
    auto result =  integer();

    std::string num;
    num.insert(num.begin(), it_begin, it_curr_symb);

    if( not num.empty() ){
        token_list.push_back( 
                   Token( num, Token::token_t::OPERATOR));
    }

    return result;
}

Parser::ParserResult Parser::integer()
{
    // TODO
    std::string num;
    if ( accept(terminal_symbol_t::TS_ZERO) )
    {
        token_list.push_back( Token(num, Token::token_t::OPERAND) );
        return ParserResult( ParserResult::PARSER_OK );
    }    


    accept(terminal_symbol_t::TS_MINUS);

    return natural_number();
}

Parser::ParserResult Parser::natural_number()
{

    auto result = digit_excl_zero();
    if( result) {
    
        while( result){
            result = digit();
        }

        return ParserResult( ParserResult::PARSER_OK );
    }

    return ParserResult( ParserResult::ILL_FORMED_INTEGER, std::distance( expr.begin(), it_curr_symb) );
}

//TS methods
bool Parser::digit_excl_zero()
{
   
    return accept( terminal_symbol_t::TS_NON_ZERO_DIGIT );
}

bool Parser::digit()
{
    
    return (accept( terminal_symbol_t::TS_ZERO ) or digit_excl_zero() );
}

/*!
 * This is the parser's entry point.
 * This method tries to (recursivelly) validate the expression.
 * During this process, we also stored the tokens into a container.
 *
 * \param e_ The string with the expression to parse.
 * \return The parsing result.
 */
Parser::ParserResult
Parser::parse( std::string e_ )
{
    // We reset the parsing process each new expression.
    expr = e_;  // The expression in a string.
    it_curr_symb = expr.begin(); // Iterator to the 1st character in the expression.
    token_list.clear(); // Clear the list of tokens.

    // Default result.
    ParserResult result( ParserResult::PARSER_OK );

    skip_ws();
    if ( end_input() )
    {
        return ParserResult ( ParserResult::UNEXPECTED_END_OF_EXPRESSION, 
                              std::distance( expr.begin(), it_curr_symb ) );
    }

    // tentar validar a expressão
    result = expression();

    if( result.type == ParserResult::PARSER_OK){

        skip_ws();
        if( not end_input()){
            return ParserResult( ParserResult::EXTRANEOUS_SYMBOL, std::distance(expr.begin(), it_curr_symb));
        }

    }
    return result;


}
/// Return the list of tokens, which is the by-product created during the syntax analysis.
std::vector< Token >
Parser::get_tokens( void ) const
{
    return token_list;
}



//==========================[ End of parse.cpp ]==========================//
