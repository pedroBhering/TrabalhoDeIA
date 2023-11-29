#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Node
{
private:
public:
    int** mat;
    Node* parent;
    vector<int> actions;
    int i; int j;
    int n;
    int w;
    int w_caminho;
    Node(int** mat,Node* parent,vector<int> actions, int i, int j, int n, int w, int w_caminho);
    ~Node();
};

Node::Node(int** mat,Node* parent,vector<int> actions, int i, int j, int n, int w, int w_caminho)
{   
    this->n = n;
    this->mat = mat;
    this->parent = parent;
    this->actions = actions;
    this->i = i;
    this->j = j;
    this->w = w;
    this->w_caminho = w_caminho;
}

Node::~Node()
{
    for (int i = 0; i < n; i++) {
        delete[] mat[i];
    }
    delete [] mat;
    delete parent;
}

class Stack
{
private:
    vector<Node*> stack;
public:
    Stack(/* args */);
    ~Stack();
    void push(Node* no);
    Node* pop();
    Node* popQueue();
    bool is_empty();
    int size(){return stack.size();};
    void swap(int pos){
        Node* aux = stack[0];
        stack[0] = stack[pos];
        stack[pos] = aux;
    }

    Node* get(int i){
        return stack[i];
    }

    void imprimeAbertos(){
        for(int i = 0; i < stack.size(); i++){
            for(int j = 0; j < stack[0]->n; j++){
                for(int k = 0; k < stack[0]->n; k++)
                    cout << stack[i]->mat[j][k] << " ";
                cout << "\n";
            }
        }
    }
    // bool contains_matrix(int **matrix_ext);
};

Stack::Stack(/* args */)
{
}

Stack::~Stack()
{
}

// bool Stack::contains_matrix(int **matrix_ext){
//     //Função para verificar se dentro da pilha já existia o estado novo, ou seja, função para podar na lista de abertos. (Muito cara!)
// }

void Stack::push(Node* no){
    stack.push_back(no);
}

Node* Stack::pop(){
    Node* no = stack.back();
    stack.pop_back();
    return no;
}

Node* Stack::popQueue(){
    Node* no = stack[0];
    stack.erase(stack.begin());
    return no;
}

bool Stack::is_empty(){
    return stack.empty();
}

void inicializaMatriz(int **matriz, int n){
    for(int i = 0; i < n; i++)
        matriz[i][0] = i + 1;
}

void imprimeMatriz(int **matriz, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}


void imprimeCaminho(Node* no, int n){
    if(no->parent != nullptr){
        imprimeCaminho(no->parent, n);
    }
    imprimeMatriz(no->mat, n);
    cout << "|" << endl;
    cout << "V" << endl;
}

vector<int> preenche_regras(int** matriz, int n, int i, int j){
    // cout << "entrou" << endl;
    if(i < n-1)
        i++;
    else{
        i = 0; j++;
    }
    vector<int> rules;

    for(int r = 1; r < n+1; r++){
        bool flag = true;

        for(int x = 0; x < i; x++){
            if(matriz[x][j] == r){
                flag = false;
                break;
            }
        }

        if(flag){
            for(int y = 0; y < j; y++){
                if (matriz[i][y] == r){
                    flag = false;
                    break;
                }
            }
        }

        if(flag){
            rules.push_back(r);
        }
    }

    return rules;
}

int** clona_matriz(int **matriz, int n){
    int **copy = new int*[n];

    for(int i = 0; i < n; i++){
        copy[i] = new int[n];
        for(int j = 0; j < n; j++){
            copy[i][j] = matriz[i][j];
        }
    }

    return copy;
}

void ordenada(int **matriz, int n){
    int i = n-1; int j = 0;
    Stack* stack = new Stack();
    set<int**> explored;

    int *qtdMatrizNivel = new int[n*(n-1)];
    
    for(int k = 0; k < n * (n-1); k++)
        qtdMatrizNivel[k] = 0;

    Node *no = new Node(matriz, nullptr, preenche_regras(matriz, n, i, j), i, j, n, 0, 0);
    stack->push(no);
    
    while(true){
        if(stack->is_empty()){
            cout << "Nenhuma solução encontrada" << endl;
            exit(1);
        }

        // cout << "Antes" << endl;
        // stack->imprimeAbertos();

        if(stack->size() != 0){
        int menor = stack->get(0)->w_caminho;
        // cout << "menor: " << menor << endl;
        // cout << "ImprimeMatriz do menor: " << endl;
        // imprimeMatriz(stack->get(0)->mat, n);
        int pos = 0;
        for(int k = 1; k < stack->size(); k++){
            if(menor > stack->get(k)->w_caminho){
                menor = stack->get(k)->w_caminho;
                pos = k;

                }
            }
            stack->swap(pos);
            // cout << "menor: " << menor << endl;
            // cout << "ImprimeMatriz Novo menor: " << endl;
            // imprimeMatriz(stack->get(0)->mat, n);
        }
        // cout << "Depois" << endl;
        // stack->imprimeAbertos();
        no = stack->popQueue();
        // cout << "nivel = " << no->i + no->j*n<< endl;
        // imprimeMatriz(no->mat, n);

        //Encontrou solução
        if(no->i == n-1 && no->j == n-1){
            cout << "Solucao: " <<endl;
            imprimeMatriz(no->mat, n);
            cout << "\nCaminho Solucao:" << endl;
            imprimeCaminho(no, n);
            exit(0);
        }

        explored.insert(no->mat);

        for(size_t k = 0; k < no->actions.size(); ++k){

            int **matriz_aux = clona_matriz(no->mat, n);

            if(no->i < n-1)
                matriz_aux[no->i+1][no->j] = no->actions[k];
            else
                matriz_aux[0][no->j+1] = no->actions[k];

            if(explored.count(matriz_aux) == 0){
                Node *new_no;
                int peso_caminho = 0;
                int peso_aresta = 0;
                if (no->i < n - 1)
                {   
                    vector<int> new_actions = preenche_regras(matriz_aux, n, no->i + 1, no->j);
                    qtdMatrizNivel[no->j * n + (no->i + 1) - n] += 1;
                    peso_aresta = qtdMatrizNivel[no->j * n + (no->i + 1) - n];
                    peso_caminho = no->parent->w_caminho + peso_aresta;
                    new_no = new Node(matriz_aux, no, new_actions, no->i + 1, no->j, n, peso_aresta, peso_caminho);

                }
                else
                {
                    vector<int> new_actions = preenche_regras(matriz_aux, n, 0, no->j + 1);
                    qtdMatrizNivel[(no->j+1) * n - n] += 1;
                    peso_aresta = qtdMatrizNivel[(no->j+1) * n - n];
                    peso_caminho = no->parent->w_caminho + peso_aresta;
                    new_no = new Node(matriz_aux, no, new_actions, 0, no->j + 1, n, peso_aresta, peso_caminho);
                }

                stack->push(new_no);
            }
        }
    }
}

int main(void){
    int n;
    cout << "Digite o valor de n: ";
    cin >> n;
    
    int **matriz = new int*[n];

    for(int i = 0; i < n; i++){
        matriz[i] = new int[n];
        for(int j = 0; j < n; j++){
            matriz[i][j] = 0;
        }
    }

    inicializaMatriz(matriz, n);

    ordenada(matriz, n);
}