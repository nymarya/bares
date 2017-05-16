#include "bares.h"

int Bares::char2int( char ch){
    return ch-'0';
}

int Bares::execute( value_type n1, value_type n2, char opr){

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

int Bares::evaluate( std::string infix){

    infix_to_postfix(infix);

    std::stack< int> s;

    for( auto ch: expression){
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

void Bares::infix_to_postfix( std::string infix_ ){

    //Stores the postfix expression
    std::string postfix = "";

    //Stack to help us convert the exp
    std::stack< char > s;

    //Tranverse the expression
    for (char  ch : infix_){

        if( is_operand(ch))
            expression += ch;
        else if ( is_operator(ch) ){

            //Pops out all the element with higher priority
            while( not s.empty() and has_higher_precedence(s.top(), ch) ){
                expression += s.top();
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
                expression += s.top(); //goes to the output
                s.pop();    //pops out the element
            }
            s.pop(); //remove the '(' from the stack
        }
        else {

        }
    }

    while (not s.empty()){
        expression += s.top();
        s.pop();
    }

}

bool Bares::is_operator(char c){
    std::string operators = "+-/^*%";

    return (operators.find(c) != std::string::npos);
}

bool Bares::is_operand( char c){
    return ( c >= 48 and c <= 57);
}

bool Bares::is_opening_scope( char c){
    return (c == '(');
}

bool Bares::is_closing_scope(char c){
    return (c== ')');
}

bool Bares::is_right_association(char c){
    return c == '^';
}

int Bares::get_precedence( char c){
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

bool Bares::has_higher_precedence( char op1, char op2){

    auto p1 = get_precedence( op1 ); //Top
    auto p2 = get_precedence( op2 ); //New operator

    //special case:
    //Has the same precedence and is right association
    if(p1 == p2 and is_right_association( op1 )) 
        return false;
    

    return p1 >= p2;
}