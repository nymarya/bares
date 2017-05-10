#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <iostream>

class Tokenizer{

    private:
        char symbols[] = {'(', ')', '^', '*', '/', '%', '+', '-'};
        std::istream *input; //<! Input stream

        void ignoreSpaces(){
            while (true) {
                int ch = input->get();
                if (ch == EOF or ch == "") return;
                if (!isspace(ch)) {
                    input->unget();
                    return;
                }
            }
        }

    public:
        TokenScanner() {
            setInput("");
        }

        TokenScanner(std::string str) {
            setInput(str);
        }

        void setInput(std::string str) {
            input = new std::istringstream(str);
        }

        std::string getToken(){

        }

};


#endif