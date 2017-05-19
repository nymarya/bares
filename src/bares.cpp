#include "bares.h"

// int Bares::char2int( Token ch){
//     auto str = ch.value;
//     char * writable = new char[str.size() + 1];
//     std::copy(str.begin(), str.end(), writable);
//     writable[str.size()] = '\0'; 

//     return *writable-'0';
// }

std::string Bares::execute( std::string i_n1, std::string i_n2, Token opr){

    // resultado final
    std::string fim;

    //converte string para inteiro
    std::string::size_type sz;   // alias of size_t
    int n1 = std::stoi (i_n1,&sz);
    int n2 = std::stoi (i_n2,&sz);

    value_type result(0);

    //converte o Token para const char
    const char *d = (opr.value).c_str();
    switch ( *d )
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

    std::stringstream ss;
    ss << result;
    return ss.str();
    
}

int Bares::evaluate( std::vector<Token> infix ){

    infix_to_postfix(infix);
    std::stack< std::string > s;

    for( Token ch: expression){
        if( is_operand(ch)) s.push(ch.value);

        else if( is_operator(ch) ){
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();

            auto result = execute(op1, op2, ch);
            s.push( result );
        }
        else {
            assert(false);
        }
    }
    //converte string para inteiro
    std::string::size_type sz;   // alias of size_t
    int n1 = std::stoi (s.top(),&sz);
    return n1; 
}

void Bares::infix_to_postfix( std::vector<Token> infix_ ){
    //Stores the postfix expression
    std::string postfix = "";

    //Stack to help us convert the exp
    std::stack< std::string > s;

    //chave master do (
    bool master_key = false;

    //std::vector<Token>::iterator it = expression.begin();
    //Tranverse the expression
    for (Token ch : infix_){

        if ( master_key and not is_closing_scope(ch.value))
        {
            Token add;
            if ( is_operand(ch) )
                {
                    add.type = ch.type;
                    add.value = ch.value;
                    expression.push_back(add);
                } 
            else
            {
                s.push(ch.value);
            }
        }
        else if( is_operand(ch))
        {
            Token add;
            add.type = ch.type;
            add.value = ch.value;
            expression.push_back(add);;
                        
        }
        else if ( is_operator(ch) ){
            //Pops out all the element with higher priority
            while( not s.empty() and has_higher_precedence(s.top(), ch.value) ){

                Token add;
                add.type = ch.type;
                add.value = s.top();
                expression.push_back(add);
                s.pop();

            }

            //the incoming operator always goes into the stack
            s.push(ch.value);
        }
        else if ( is_opening_scope(ch.value) ){
            
            s.push(ch.value);
            master_key = true;
            
        }
        else if ( is_closing_scope(ch.value) )
        {
            //pop out all the elemens that are not '('
            master_key = false;
            while( not is_opening_scope(s.top()) and not s.empty() )
            {
                //goes to the output
                Token add;
                add.type = Token::token_t::OPERATOR;
                add.value = s.top();

                expression.push_back(add);    //pops out the element
                s.pop();
            }
            s.pop(); //remove the '(' from the stack
        }
    }

   
    while (not s.empty()){
        
        Token add;
        add.type = Token::token_t::OPERATOR;
        add.value = s.top();
        expression.push_back(add);

        s.pop();
    }

    

}

bool Bares::is_operator(Token c){
    
    return c.type == Token::token_t::OPERATOR;
}

bool Bares::is_operand( Token c){
    return c.type == Token::token_t::OPERAND;
}

bool Bares::is_opening_scope( std::string c){
    return (c == "(");
}

bool Bares::is_closing_scope(std::string c){
    return (c == ")");
}

bool Bares::is_right_association(std::string c){
    return c == "^";
}

int Bares::get_precedence( std::string c){
    int weigth = 0;
    const char *d = c.c_str();
    switch( *d ){
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

bool Bares::has_higher_precedence( std::string op1, std::string op2){

    auto p1 = get_precedence( op1 ); //Top
    auto p2 = get_precedence( op2 ); //New operator

    //special case:
    //Has the same precedence and is right association
    if(p1 == p2 and is_right_association( op1 )) 
        return false;
    

    return p1 >= p2;
}