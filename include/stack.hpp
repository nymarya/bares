#ifndef _STACK_H_
#define _STACK_H_

#include <iostream>
#include <stdexcept>
#include <cassert>
#include <iterator>

namespace ls{ 
    template <typename T>
    class Stack
    {
        private:
            static const std::size_t SIZE = 10;
            std::size_t m_top;      //<! Topo da pilha
            std::size_t m_capacity; //<!Tamanho físico
            T * m_data;             //<! Area de armazenamento

            //Amplia a area de armazenamento
            void reserve( std::size_t new_cap);

        public:
            Stack( std::size_t cap = SIZE);
            
            ~Stack();

            Stack( const Stack &) = delete;
            Stack & operator=(const Stack &) = delete;

            void push( const T & value);
            T pop ( void );           //pode lançar std::length_error
            T top( void );  

            bool empty( void );
    };
}

#include "stack.inl"

#endif