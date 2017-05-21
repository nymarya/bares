/**
 * @file token.h
 * @authors Gabriel Araújo de Souza e Mayra Dantas de Azevedo
 * @date 21 Maio 2017
 * @brief Arquivo contendo as definições da classe Token.
 */

#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>   // std::string
#include <iostream> // std::ostream

/**
 * @brief      Representação de um Token
 */
struct Token
{
    public:

        /**
         * @brief      Enumeração para os tipos de Token
         */
        enum class token_t : int
        {
            OPERAND = 0,   // Basically numbers.
            OPERATOR,      // "+", "-". "^", "%", "*", "/"
            CLOSING_SCOPE, // ")"
            OPENING_SCOPE, // "("
        };

        std::string value; //<! O valor original do Token.
        token_t type;      //<! O tipo do Token.

        /**
         * @brief      Construtor Default
         *
         * @param[in]  v_    Valor do Token
         * @param[in]  t_    Tipo do Token
         */
        explicit Token( std::string v_="", token_t t_ = token_t::OPERAND )
            : value( v_ )
            , type( t_ )
        {/* empty */}

        /**
         * @brief      Apenas para ajudar a debugar o código
         *
         * @param      os_   A operação sistema
         * @param[in]  t_    O Token
         *
         * @return     uma ostream com a avaliação
         */
        friend std::ostream & operator<<( std::ostream& os_, const Token & t_ )
        {
            std::string types[] = { "OPERAND", "OPERATOR", "CLOSING SCOPE", "OPENING SCOPE" };

            os_ << "<" << t_.value << "," << types[(int)(t_.type)] << ">";

            return os_;
        }
};

#endif
