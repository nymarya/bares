#ifndef _BARES_H_
#define _BARES_H_

#include <iostream>
#include <stack>     // stack
#include <string>    // string
#include <iomanip>
#include <cassert>   // assert
#include <cmath>     // pow
#include <sstream> // std::stringstream

#include "tokenizer.h"

class Bares{


	using value_type = long int;

	private:
		
		std::vector<Token> expression;


		bool is_operator(Token c);

		bool is_operand( Token c);

		bool is_opening_scope( std::string c);

		bool is_closing_scope( std::string c);

		bool has_higher_precedence( std::string op1, std::string op2);

		// int char2int( Token ch);

		bool is_right_association(std::string c);

		int get_precedence( std::string c);


	public:

		//==== Special methods
        /// Construtor default.
        Bares() = default;
        ~Bares() = default;
        /// Desligar cópia e atribuição.
        Bares( const Bares & ) = delete;  // Construtor cópia.
        Bares & operator=( const Bares & ) = delete; // Atribuição.

		/// Converts a expression in infix notation to a corresponding profix representation.
		void infix_to_postfix( std::vector<Token> infix_ );

		/// Executes the operation op
		std::string execute( std::string n1, std::string n2, Token opr);
		

		/// Evaluate postfix expression
		int evaluate(std::vector<Token>);
};


#endif