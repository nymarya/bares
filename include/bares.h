/**
 * @file bares.h
 * @authors Gabriel Araújo de Souza e Mayra Dantas de Azevedo
 * @date 21 Maio 2017
 * @brief Arquivo contendo as definições da classe Bares.
 */

#ifndef _BARES_H_
#define _BARES_H_

#include <iostream>  //std::cout, std::cin
#include <stack>     // stack
#include <string>    // string
#include <iomanip>   // std::distance
#include <cassert>   // assert
#include <cmath>     // pow
#include <sstream> // std::stringstream

#include "tokenizer.h"

/**
 * @brief      Classe para bares.
 */
class Bares{

    /**
     * Definição do tipo value_type
     */
	using value_type = long int;

	public:
    
    /**
     * @brief      Representa o resultado das operações resolvidas
     */
    struct Result
    {
        /**
         * @brief      Lista de possíveis erros de sintaxe
         */
        enum code_t {
            OK = 0,
            DIVISION_BY_ZERO,
            NUMERIC_OVERFLOW
        };

        //=== Membros (público).
        std::string value_b; //<! Guarda o resultado da operação.
        code_t type_b;      //<! Código de Error.

        /**
         * @brief      Construtor Default
         *
         * @param[in]  v_    valor da operação
         * @param[in]  t_    código de error
         */
        explicit Result( std::string v_="", code_t t_ = code_t::OK )
            : value_b( v_ )
            , type_b( t_ )
        {/* empty */}     
    };

	private:
		
        /**
         * Vector de expressões
         */
		std::vector<Token> expression;

        /**
         * @brief      Determina se é um operador
         *
         * @param[in]  c     Token para verificar se é um operador
         *
         * @return     True se operador, false caso contrário
         */
		bool is_operator(Token c);

        /**
         * @brief      Determina se é operando
         *
         * @param[in]  c   Token para verificar se é um operando.  
         *
         * @return     True se operando, False caso contrário.
         */
		bool is_operand( Token c);

        /**
         * @brief      Determina se é um parênteses aberto
         *
         * @param[in]  c     String para se verificar
         *
         * @return     True se parênteses aberto, False caso contrário.
         */
		bool is_opening_scope( std::string c);

        /**
         * @brief      Determina se é um parênteses fechado
         *
         * @param[in]  c     String para se verificar
         *
         * @return     True se parênteses fechado, False caso contrário.
         */
		bool is_closing_scope( std::string c);

		/**
         * @brief      Verifica se o op1 tem precedência maior que o op2.
         *
         * @param[in]  op1   O operador 1
         * @param[in]  op2   O operador 2
         *
         * @return     True se tem a precedência maior, False caso contrário.
         */
        bool has_higher_precedence( std::string op1, std::string op2);

		/**
         * @brief      Verifica se tem associação a direita ( para potências ).
         *
         * @param[in]  c     Operador para verificar
         *
         * @return     True se é associação a direita, False caso contrário.
         */
        bool is_right_association(std::string c);

		/**
         * @brief      Pega a precedência.
         *
         * @param[in]  c     String a ser verificada
         *
         * @return     A precedência.
         */
        int get_precedence( std::string c);


	public:

		//==== Métodos Especiais
        
        /**
         * @brief      Construtor Default
         */
        Bares() = default;

        /**
         * @brief      Destrói o objeto
         */
        ~Bares() = default;
        
        /**
         * @brief      Construtor Cópial
         *
         * @param[in]  <unnamed>  { parameter_description }
         */
        Bares( const Bares & ) = delete;

        /**
         * @brief      Atribuição
         *
         * @param[in]  <unnamed>  Cópia a ser feita
         *
         * @return     Um Bares igual ao repassado
         */
        Bares & operator=( const Bares & ) = delete;
      
        /**
         * @brief      Converte a expressão com notação infixa para o
         *             correspondente em representação posfixa
         *
         * @param[in]  infix_  Notação Infixa para ser transformada
         */
		void infix_to_postfix( std::vector<Token> infix_ );

		/**
         * @brief      Resolve uma operação
         *
         * @param[in]  n1    O primeiro operando
         * @param[in]  n2    O segundo operando
         * @param[in]  opr   O operador
         *
         * @return     Resultado da operação com a informação de error ou não
         */
		Bares::Result execute( std::string n1, std::string n2, Token opr);
        
        /**
         * @brief      Executa a expressão
         *
         * @param[in]  <unnamed>  expressão na forma de Tokens
         *
         * @return     Resultado final da expressão
         */
		Bares::Result evaluate(std::vector<Token>);
};


#endif