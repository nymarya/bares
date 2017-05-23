/**
 * @file bares.cpp
 * @authors Gabriel Araújo de Souza e Mayra Dantas de Azevedo
 * @date 21 Maio 2017
 * @brief Arquivo com a implementação dos métodos da classe Bares.
 */

#include "bares.h"

//<! Resolve uma operação
Bares::Result Bares::execute( std::string i_n1, std::string i_n2, Token opr){

    // resultado final
    std::string fim;

    //converte string para inteiro
    std::string::size_type sz;   // alias size_t
    int n1 = std::stoi (i_n1,&sz);
    int n2 = std::stoi (i_n2,&sz);

    value_type result(0);
    Bares::Result v;

    //converte o Token para const char
    const char *d = (opr.value).c_str();
    switch ( *d )
    {
        case '^' : result = static_cast<value_type>( pow( n1, n2 ) );
                   break;
        case '*' : result =  n1 * n2;
                   break;
        case '/' : if ( n2 == 0 ){
                       v.type_b = Bares::Result::DIVISION_BY_ZERO;
                       return v;
                   }else
                       result = n1/n2;
                   break;
        case '%' : if ( n2 == 0 ){
                        v.type_b = Bares::Result::DIVISION_BY_ZERO;
                        return v;
                   }else
                       result = n1%n2;
                   break;
        case '+' : result = n1 + n2;
                   break;
        case '-' : result =  n1 - n2;
                   break;
        default: assert(false);
    }

    //Testa se está no limite de required_int_type
    if( result <= std::numeric_limits< Tokenizer::required_int_type >::max() 
        and result >= std::numeric_limits< Tokenizer::required_int_type >::min()){

        //converte para string
        std::stringstream ss;
        ss << result;
        v.value_b = ss.str();
        v.type_b = Bares::Result::OK;
        
    } else
        v.type_b = Bares::Result::NUMERIC_OVERFLOW;

    return v;
        
    
}

//<! Executa a expressão 
Bares::Result Bares::evaluate( std::vector<Token> infix ){

    infix_to_postfix(infix);
    std::stack< std::string > s;
    Bares::Result result;

    for( Token ch: expression){
        if( is_operand(ch)) s.push(ch.value);

        else if( is_operator(ch) ){
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();

            result = execute(op1, op2, ch);
            if ( result.type_b != Bares::Result::OK )
                return result;
            else
                s.push( result.value_b );
        }
        else {
            assert(false);
        }
    }

    return result;
}

//<! Converte a expressão com notação infixa para o
//   correspondente em representação posfixa
void Bares::infix_to_postfix( std::vector<Token> infix_ ){
    std::string postfix = "";
    std::stack< std::string > s;

    //Percorre a expressão
    for (Token ch : infix_){

        if( is_operand(ch))
        {
            Token add;
            add.type = ch.type;
            add.value = ch.value;
            expression.push_back(add);;
                        
        }
        else if ( is_operator(ch) ){
            //Remove todos os elementos com prioridade mais alta
            while( not s.empty() and has_higher_precedence(s.top(), ch.value) ){

                Token add;
                add.type = ch.type;
                add.value = s.top();
                expression.push_back(add);
                s.pop();

            }

            //O operador sempre entra na fila
            s.push(ch.value);
        }
        else if ( is_opening_scope(ch.value) ){
            
            s.push(ch.value);
                        
        }
        else if ( is_closing_scope(ch.value) )
        {
            //Remove todos os elementos que não são '('
            while( not is_opening_scope(s.top()) and not s.empty() )
            {
                //Vai direto para a saída
                Token add;
                add.type = Token::token_t::OPERATOR;
                add.value = s.top();

                expression.push_back(add);    //Insere o novo token
                s.pop();                      //Remove elemento
            }
            s.pop(); //Remove '(' da pilha
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

//<! Verifica se é operador
bool Bares::is_operator(Token c){
    
    return c.type == Token::token_t::OPERATOR;
}

//<! Verifica se é operando
bool Bares::is_operand( Token c){
    return c.type == Token::token_t::OPERAND;
}

//<! Verifica se é um parênteses aberto
bool Bares::is_opening_scope( std::string c){
    return (c == "(");
}

//<! Verifica se é um parênteses fechado
bool Bares::is_closing_scope(std::string c){
    return (c == ")");
}

//<! Verifica se é associação à direita
bool Bares::is_right_association(std::string c){
    return c == "^";
}

//<! Pega as precedências
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

//<! Verifica qual a maior precedência
bool Bares::has_higher_precedence( std::string op1, std::string op2){

    auto p1 = get_precedence( op1 ); //Top
    auto p2 = get_precedence( op2 ); //Novo operador

    //Caso especial:
    //Tem a mesma precedência e segue a associado pela direita
    if(p1 == p2 and is_right_association( op1 )) 
        return false;
    

    return p1 >= p2;
}