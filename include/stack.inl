/**
 * @file stack.inl
 * @author Mayra Dantas de Azevedo
 * @date 3 Maio 2017
 * @brief Arquivo com a implementação dos métodos da classe ls::Stack.
 */

#include "stack.hpp"
#include <stdexcept>

//<! Construtor padrão.
template <typename T>
ls::Stack<T>::Stack( std::size_t cap)
    : m_top(0), 
    m_capacity(cap), 
    m_data(new T[cap])
    { /*empty*/}

//<! Destrói o objeto. 
template <typename T>
ls::Stack<T>::~Stack()
{ delete [] m_data; }

//<! Insere um elemento na pilha.
template <typename T>
void ls::Stack<T>::push(const T & value){
    if(m_top == m_capacity) reserve(m_capacity * 2);

    m_data[m_top++] = value;
}

//<! Remove último elemento da pilha.
template <typename T>
T ls::Stack<T>::pop( void ){
    if(m_top == 0) 
        throw std::length_error("[pop()] Cannot recover element from an empty vector.");

    return m_data[--m_top];
}

//<! Retorna elemento no topo da pilha.
template <typename T>
T ls::Stack<T>::top() 
{  return m_data[m_top-1]; }


//<! Aumenta a capacidade de armazenamento da pilha para um valor que é maior ou igual a new_cap.
template <typename T>
void ls::Stack<T>::reserve( std::size_t new_cap ){
    if(new_cap > m_capacity){
        T *temp = new T[ new_cap ]; //Novo vetor

        std::copy(m_data, m_data + new_cap, temp);  //Copia para novo vetor

        delete m_data;

        m_data = temp;                          //Aponta para o novo endereço
        m_capacity = new_cap; //Atualizar tamanho;
    }
}

//<! Verifica se pilha está vazia.
template <typename T>
bool ls::Stack<T>::empty() 
{  return m_top == 0; }
