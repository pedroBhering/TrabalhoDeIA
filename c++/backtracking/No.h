#include <iostream>
#include <stack>

class No {
public:

    No(){}
    ~No(){}

    No* pai = NULL;
    std::stack<char> pilha;
    int i, j;
    bool visit = false;

    void setPai(No* pai){
        this->pai = pai;
    }

    No* getPai()
    {
        return this->pai;
    }


    // Função para empilhar um caractere na pilha do nó
    void empilhar(char regra) {
        pilha.push(regra);
    }

    // Função para desempilhar um caractere da pilha do nó
    char desempilhar() {
        if (!pilha.empty()) {
            char topo = pilha.top();
            pilha.pop();
            return topo;
        }
        return '\0'; // Retorne algum valor de erro se a pilha estiver vazia
    }
};


