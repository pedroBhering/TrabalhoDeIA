#include <iostream>
#include <vector>
#include <set>
#include<tuple>

using namespace std;

class Node
{
private:
public:
    int** mat;
    Node* parent;
    vector<tuple<int, int>> actions;
    int i; int j;
    int n;
    int w;
    Node(int** mat,Node* parent,vector<tuple<int, int>> actions, int i, int j, int n);
    ~Node();
};

Node::Node(int** mat,Node* parent,vector<tuple<int, int>> actions, int i, int j, int n)
{   
    this->n = n;
    this->mat = mat;
    this->parent = parent;
    this->actions = actions;
    this->i = i;
    this->j = j;
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

vector<tuple<int, int>> preenche_regras(int** matriz, int n, int i, int j, int *qtdMatrizNivel){
    // cout << "entrou" << endl;
    if(i < n-1)
        i++;
    else{
        i = 0; j++;
    }
    vector<tuple<int, int>> rules;

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
            int nivel = j * n + i - n;
            qtdMatrizNivel[nivel] += 1;
            // cout << "Nivel: " << nivel+n << endl;
            // cout << qtdMatrizNivel[nivel] << endl;
            tuple<int, int> reg = make_tuple(r, qtdMatrizNivel[nivel]);
            // cout << "r = "<<get<0>(reg) << endl;
            // cout << "peso = " << get<1>(reg) << endl;
            rules.push_back(reg);
            // cout << "passou" << endl;
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

void bfs_and_dfs(int **matriz, int n){
    int i = n-1; int j = 0;
    Stack* stack = new Stack();
    set<int**> explored;

    int *qtdMatrizNivel = new int[n];
    
    for(int k = 0; k < n * (n-1); k++)
        qtdMatrizNivel[k] = 0;

    Node *no = new Node(matriz, nullptr, preenche_regras(matriz, n, i, j, qtdMatrizNivel), i, j, n);
    stack->push(no);
    
    while(true){
        if(stack->is_empty()){
            cout << "Nenhuma solução encontrada" << endl;
            exit(1);
        }


        if(no->actions.size() != 0){
        int menor = get<1>(no->actions[0]);
        int pos = 0;
        for(int k = 0; k < stack->size(); k++){
            if(stack->get(k)->actions.size()!= 0)
                if(menor > get<1>(stack->get(k)->actions[0])){
                    menor = get<1>(stack->get(k)->actions[0]);
                    pos = k;
                }
        }
        stack->swap(pos);
        }
        
        no = stack->popQueue();
        cout << "nivel = " << no->i + no->j*n<< endl;
        // imprimeMatriz(no->mat, n);

        //Encontrou solução
        if(no->i == n-1 && no->j == n-1){
            imprimeMatriz(no->mat, n);
            exit(0);
        }

        explored.insert(no->mat);

        for(size_t k = 0; k < no->actions.size(); ++k){

            int **matriz_aux = clona_matriz(no->mat, n);

            if(no->i < n-1)
                matriz_aux[no->i+1][no->j] = get<0>(no->actions[k]);
            else
                matriz_aux[0][no->j+1] = get<0>(no->actions[k]);

            if(explored.count(matriz_aux) == 0){
                Node *new_no;
                if (no->i < n - 1)
                {   
                    vector<tuple<int, int>> new_actions = preenche_regras(matriz_aux, n, no->i + 1, no->j, qtdMatrizNivel);
                    new_no = new Node(matriz_aux, no, new_actions, no->i + 1, no->j, n);
                }
                else
                {
                    vector<tuple<int, int>> new_actions = preenche_regras(matriz_aux, n, 0, no->j + 1, qtdMatrizNivel);
                    new_no = new Node(matriz_aux, no, new_actions, 0, no->j + 1, n);
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

    bfs_and_dfs(matriz, n);
}