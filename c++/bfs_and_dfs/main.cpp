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
    Node(int** mat,Node* parent,vector<int> actions, int i, int j);
    ~Node();
};

Node::Node(int** mat,Node* parent,vector<int> actions, int i, int j)
{
    this->mat = mat;
    this->parent = parent;
    this->actions = actions;
    this->i = i;
    this->j = j;
}

Node::~Node()
{
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

vector<int> preenche_regras(int** matriz, int n, int i, int j){
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

void bfs_and_dfs(int **matriz, int n){
    int i = n-1; int j = 0;
    Stack* stack = new Stack();
    set<int**> explored;
    Node *no = new Node(matriz, nullptr, preenche_regras(matriz, n, i, j), i, j);

    stack->push(no);
    
    while(true){
        if(stack->is_empty()){
            cout << "Nenhuma solução encontrada" << endl;
            exit(1);
        }

        no = stack->pop();

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
                matriz_aux[no->i+1][no->j] = no->actions[k];
            else
                matriz_aux[0][no->j+1] = no->actions[k];

            if(explored.count(matriz_aux) == 0){
                Node *new_no;
                if (no->i < n - 1)
                {
                    vector<int> new_actions = preenche_regras(matriz_aux, n, no->i + 1, no->j);
                    new_no = new Node(matriz_aux, no, new_actions, no->i + 1, no->j);
                }
                else
                {
                    vector<int> new_actions = preenche_regras(matriz_aux, n, 0, no->j + 1);
                    new_no = new Node(matriz_aux, no, new_actions, 0, no->j + 1);
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