#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream> // cout, cin
#include <iterator> // std::distance()
#include <vector>   // std::vector
#include <sstream>  // std::istringstream
#include <stdexcept>
#include <string>

#include "token.h"  // struct Token.

/*!
 * Implements a recursive descendent parser for a EBNF grammar.
 *
 *   <expr>            := <term>,{ ("+"|"-"),<term> };
 *   <term>            := <integer>;
 *   <integer>         := 0 | ["-"],<natural_number>;
 *   <natural_number>  := <digit_excl_zero>,{<digit>};
 *   <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 *   <digit>           := "0"| <digit_excl_zero>;
 */
class Parser
{
    public:
        /// This class represents the result of the parsing operation.
        struct ParserResult
        {
            //=== Alias
            typedef size_t size_type; //<! Used for column location.

            // List of possible syntax errors.
            enum code_t {
                    PARSER_OK = 0,
                    UNEXPECTED_END_OF_EXPRESSION,
                    ILL_FORMED_INTEGER,
                    MISSING_TERM,
                    EXTRANEOUS_SYMBOL,
                    MISSING_CLOSING_PARENTHESIS,
                    INTEGER_OUT_OF_RANGE
            };

            //=== Members (public).
            code_t type;      //<! Error code.
            size_type at_col; //<! Guarda a coluna do erro.

            // Por padrão, o resultado é positivo.
            explicit ParserResult( code_t type_=PARSER_OK , size_type col_=0u )
                    : type{ type_ }
                    , at_col{ col_ }
            { /* empty */ }
        };

        //==== Aliases
        typedef short int required_int_type;
        typedef long long int input_int_type;

        //==== Public interface
        /// Recebe uma expressão, realiza o parsing e retorna o resultado.
        ParserResult parse( std::string e_ );
        /// Retorna a lista de tokens.
        std::vector< Token > get_tokens( void ) const;

        //==== Special methods
        /// Constutor default.
        Parser() = default;
        ~Parser() = default;
        /// Desligar cópia e atribuição.
        Parser( const Parser & ) = delete;  // Construtor cópia.
        Parser & operator=( const Parser & ) = delete; // Atribuição.

    private:
        //=== Aliases
        static constexpr bool SUCCESS{ true };
        static constexpr bool FAILURE{ false };

        // Terminal symbols table
        enum class terminal_symbol_t{  // The symbols:-
            TS_PLUS,	        //<! "+"
            TS_MINUS,	        //<! "-"
            TS_ZERO,            //<! "0"
            TS_NON_ZERO_DIGIT,  //<! "1"->"9"
            TS_WS,              //<! white-space
            TS_TAB,             //<! tab
            TS_EOS,             //<! End Of String
            TS_INVALID	        //<! invalid token
        };

        //==== Private members.
        std::string expr;                //<! The expression to be parsed
        std::string::iterator it_curr_symb; //<! Pointer to the current char inside the expression.
        std::vector< Token > token_list; //<! Resulting list of tokens extracted from the expression.

        /// Converte de caractere para código do símbolo terminal.
        terminal_symbol_t lexer( char ) const;
        std::string token_str( terminal_symbol_t s_ ) const;

        //=== Support methods.
        void next_symbol( void );                // Advances iterator to the next char in the expression.
        bool peek( terminal_symbol_t s_ ) const; // Peeks the current character.
        bool accept( terminal_symbol_t s_ );     // Tries to accept the requested symbol.
        bool expect( terminal_symbol_t );        // Skips any WS/Tab and tries to accept the requested symbol.
        void skip_ws( void );                    // Skips any WS/Tab ans stops at the next character.
        bool end_input( void ) const;            // Checks whether we reached the end of the expression string.
        input_int_type str_to_int( std::string );// Converts a string into an integer.

        //=== NTS methods.
        ParserResult expression();
        ParserResult term();
        ParserResult integer();
        ParserResult natural_number();
        ParserResult digit_excl_zero();
        ParserResult digit();
};

#endif
