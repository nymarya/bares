#ifndef _BARES_H_
#define _BARES_H_

#include <iostream>
#include <stack>     // stack
#include <string>    // string
#include <iomanip>
#include <cassert>   // assert
#include <cmath>     // pow
#include "tokenizer.h"

class Bares{


	using value_type = long int;

	private:
		std::string expression = "";


		bool is_operator(char c);

		bool is_operand( char c);

		bool is_opening_scope( char c);

		bool is_closing_scope(char c);

		bool has_higher_precedence( char op1, char op2);

		int char2int( char ch);

		bool is_right_association(char c);

		int get_precedence( char c);


	public:

		//==== Special methods
        /// Construtor default.
        Bares() = default;
        ~Bares() = default;
        /// Desligar cópia e atribuição.
        Bares( const Bares & ) = delete;  // Construtor cópia.
        Bares & operator=( const Bares & ) = delete; // Atribuição.

		/// Converts a expression in infix notation to a corresponding profix representation.
		void infix_to_postfix( std::string );

		/// Executes the operation op
		int execute( value_type n1, value_type n2, char opr);

		/// Evaluate postfix expression
		int evaluate( std::string);
};


#endif