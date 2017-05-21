/**
 * @file tokenizer.h
 * @authors Gabriel Araújo de Souza e Mayra Dantas de Azevedo
 * @date 21 Maio 2017
 * @brief Arquivo contendo as definições da classe Tokenizer.
 */

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <iostream> // cout, cin
#include <iterator> // std::distance()
#include <vector>   // std::vector
#include <sstream>  // std::istringstream
#include <stdexcept> //throw
#include <string>   // std::string
#include <limits> //numeric_limits

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

/**
 * @brief      Classe para Tokenizer
 */
class Tokenizer
{
    public:
        
        /**
         * @brief      Representa um resultado para a operação parse
         */
        struct Result
        {
            //=== Alias
            typedef size_t size_type; //<! Used for column location.

            //<! Lista de possíveis erros de sintaxe.
            enum code_t {
                    OK = 0,
                    UNEXPECTED_END_OF_EXPRESSION,
                    ILL_FORMED_INTEGER,
                    MISSING_TERM,
                    EXTRANEOUS_SYMBOL,
                    MISSING_CLOSING_PARENTHESIS,
                    INTEGER_OUT_OF_RANGE
            };

            //=== Membros (público).
            code_t type;      //<! Código de Erro.
            size_type at_col; //<! Guarda a coluna do erro.

            /**
             * @brief      Construtor do Result
             *
             * @param[in]  type_  O tipo
             * @param[in]  col_   A coluna de erro
             */
            explicit Result( code_t type_=OK , size_type col_=0u )
                    : type{ type_ }
                    , at_col{ col_ }
            { /* empty */ }
        };

        //==== Aliases
        typedef short int required_int_type;
        typedef long long int input_int_type;

        //==== Public interface
        
        /**
         * @brief      Recebe uma expressão, realiza o parsing e retorna o resultado.
         *
         * @param[in]  e_    Expressão
         *
         * @return     Um Result sobre a expressão
         */
        Result parse( std::string e_ );
        
        /**
         * @brief      Pega a lista de Tokens
         *
         * @return     A lista de Tokens
         */
        std::vector< Token > get_tokens( void ) const;

        //==== Special methods
        
        /**
         * @brief      Construtor Default
         */
        Tokenizer() = default;

        /**
         * @brief      Destroi o objeto
         */
        ~Tokenizer() = default;
        
        /**
         * @brief      Construtor Cópia
         *
         * @param[in]  <unnamed>  O que se deseja copiar
         */
        Tokenizer( const Tokenizer & ) = delete;

        /**
         * @brief      Operador de atribuição
         *
         * @param[in]  <unnamed>  O que se deseja atribuir
         *
         * @return     Um Tokenizer com a atribuição
         */
        Tokenizer & operator=( const Tokenizer & ) = delete;

    private:
        //=== Aliases
        static constexpr bool SUCCESS{ true };
        static constexpr bool FAILURE{ false };

        /**
         * @brief      Tabelas de símbolos
         */
        enum class terminal_symbol_t{  // The symbols:-
            TS_PLUS,	         //<! "+"
            TS_MINUS,	         //<! "-"
            TS_MOD,              //<! "%"
            TS_SLASH,            //<! "/"
            TS_ASTERISK,         //<! "*"
            TS_CARRET,           //<! "^"
            TS_CLOSING_SCOPE,    //<! ")"
            TS_OPENING_SCOPE,    //<! "("
            TS_ZERO,             //<! "0"
            TS_NON_ZERO_DIGIT,   //<! "1"->"9"
            TS_WS,               //<! White-space
            TS_TAB,              //<! Tab
            TS_EOS,              //<! End Of String
            TS_INVALID	         //<! Invalid Token
        };

        //==== Private members.
        std::string expr;                //<! A expressão para ser parsed
        std::string::iterator it_curr_symb; //<! Ponteiro para o atual char da expressão.
        std::vector< Token > token_list; //<! Lista de Tokens final extraída da expressão.

        /**
         * @brief      Converte o caractere para um dos símbolos da tabela
         *
         * @param[in]  <unnamed>  Caractere a ser convertido
         *
         * @return     Retorna um símbolo da tabela
         */
        terminal_symbol_t lexer( char ) const;

        /**
         * @brief      converte para uma string o símbolo recebido
         *
         * @param[in]  s_    O símbolo a ser convertido
         *
         * @return     Uma string com o simbolo equivalente recebido 
         */
        std::string token_str( terminal_symbol_t s_ ) const;

        //=== Support methods.
        
        /**
         * @brief      Avança o iterador para o próximo caractere da expressão.
         */
        void next_symbol( void );

        /**
         * @brief      Verifica se o caractere atual é igual ao simbolo solicitado
         *
         * @param[in]  s_    { parameter_description }
         *
         * @return     True se foren iguais, False caso contrário
         */
        bool peek( terminal_symbol_t s_ ) const;

        /**
         * @brief      Tenta aceitar o símbolo solicitado
         *
         * @param[in]  s_    Símbolo solicitado
         *
         * @return     True se for aceito, False caso contrário
         */
        bool accept( terminal_symbol_t s_ );

        /**
         * @brief   Ignora qualquer espaço/Tab e tenta aceitar o símbolo solicitado.
         *          se for aceito, avança para o próximo caractere
         *
         * @param[in]  <unnamed>  Símbolo solicitado
         *
         * @return     True se aceito, False caso contrário
         */
        bool expect( terminal_symbol_t );

        /**
         * @brief      Ignora qualquer espaço/Tab e para no próximo caractere
         */
        void skip_ws( void );

        /**
         * @brief     Verifica se chegamos ao final da seqüência de expressão
         *
         * @return     True se chegou ao final da expressão, False caso contrário
         */
        bool end_input( void ) const;

        /**
         * @brief      Converte String para inteiro
         *
         * @param[in]  <unnamed>  A string a ser convertida
         *
         * @return     O valor inteiro correspondente a string
         */
        input_int_type str_to_int( std::string );

        //=== NTS methods.
        
        /**
         * @brief      Verifica se é uma expressão
         *
         * @return     Um Result com a expressão
         */
        Result expression();

        /**
         * @brief      Verifica se é um termo
         *
         * @return     Um Result com o termo
         */
        Result term();

        /**
         * @brief      Verifica se é um inteiro
         *
         * @return     Result com o inteiro
         */
        Result integer();

        /**
         * @brief      Verifica se é um núemro natural
         *
         * @return     Result com o número natural
         */
        Result natural_number();

        /**
         * @brief      Verifica se é um dígito diferente de zero
         *
         * @return     True se for um dígito diferente de zero,
         *             False caso contrário
         */
        bool digit_excl_zero();

        /**
         * @brief      Verifica se é um dígito
         *
         * @return     True se for dígito, false caso contrário
         */
        bool digit();
};

#endif
