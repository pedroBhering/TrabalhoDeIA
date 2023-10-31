#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Node
{
private:
public:
    int** mat; // a matriz naquele estado
    Node* parent; // o pai do nó
    vector<int> actions; // as ações possíveis para aquele estado
    int i; int j; // a posição do espaço vazio
    int n; // o tamanho da matriz
    int w; // o peso do nó
    int cont_nao_preenchidos; // o número de posições não preenchidas na matriz
    Node(int** mat,Node* parent,vector<int> actions, int i, int j, int n, int w, int cont_nao_preenchidos); // construtor
    ~Node(); // destrutor
};

Node::Node(int** mat,Node* parent,vector<int> actions, int i, int j, int n, int w, int cont_nao_preenchidos)
{   
    this->n = n;
    this->mat = mat;
    this->parent = parent;
    this->actions = actions;
    this->i = i;
    this->j = j;
    this->w = w;
    this->cont_nao_preenchidos = cont_nao_preenchidos;
}

Node::~Node()
{
    for (int i = 0; i < n; i++) {
        delete[] mat[i];
    }
    delete [] mat;
    delete parent;
}

class Stack // pilha de nós abertos
{
    private:
        vector<Node*> stack; // um vetor de ponteiros para os nós
    public:
        Stack(/* args */);
        ~Stack();
        void push(Node* no); // inserir no final da lista de abertos, mudar para ordenação direto na inserção
        Node* pop(); // remove do final do vetor e retorna o nó removido
        Node* popQueue(); // remove do inicio do vetor e retorna o nó removido
        bool is_empty(); // verifica se a lista de abertos está vazia
        int size(){return stack.size();}; // retorna o tamanho da lista de abertos

    // Node* get(int i){
    //     return stack[i];
    // }

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
    // stack.push_back(no);

    int pos = 0;
    for(size_t i = 0; i < stack.size(); i++)
    {
        if(stack[i]->w >= no->w) // se o peso do nó da posição i for maior ou igual ao peso do nó a ser inserido, insere na posição i
        {
            pos = i;
            break;
        }
    }
    stack.insert(stack.begin() + pos, no);
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

//mudar para poder personalizar a matriz inicial
void inicializaMatriz(int **matriz, int n)
{
    for(int i = 0; i < n; i++)
        matriz[i][0] = i + 1;
} // inicializa a matriz com os valores de 1 a n na primeira coluna

int inicializaMatriz(int **matriz, int n, int k)
{
    return n*n - k;
}

void imprimeMatriz(int **matriz, int n){

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
} // imprime a matriz

void imprimeCaminho(Node* no, int n){
    if(no->parent != nullptr){
        imprimeCaminho(no->parent, n);
    }
    imprimeMatriz(no->mat, n);
    cout << "|" << endl;
    cout << "V" << endl;
} // imprime o caminho da solução

// void imprimeArvore(Node* no, int n){
//     if(no->parent != nullptr){
//         imprimeArvore(no->parent, n);
//     }
//     imprimeMatriz(no->mat, n);
//     cout << "peso: " << no->w << endl;
//     cout << "cont_nao_preenchidos: " << no->cont_nao_preenchidos << endl;
//     cout << "nivel: " << no->i + no->j*n << endl;
//     cout << "actions: ";
//     for(int i = 0; i < no->actions.size(); i++)
//         cout << no->actions[i] << " ";
//     cout << endl;
//     cout << "|" << endl;
//     cout << "V" << endl;
// } // imprime a árvore de busca

vector<int> preenche_regras(int** matriz, int n, int i, int j){
    // cout << "entrou" << endl;
    if(i < n-1) // se não estiver na última linha, incrementa i
        i++;
    else{ // se estiver na última linha, incrementa j e zera i
        i = 0; j++;
    }
    vector<int> rules;

    for(int r = 1; r < n+1; r++){ // r é o valor que será inserido na posição ij e que é testado
        bool flag = true;

        for(int x = 0; x < n; x++){
            if(matriz[x][j] == r){
                flag = false;
                break;
            }
        }

        if(flag){
            for(int y = 0; y < n; y++){
                if (matriz[i][y] == r){
                    flag = false;
                    break;
                }
            }
        }
        //pesquisa o valor em toda a linha e em toda a coluna, pois a matriz pode começar tendo valores inseridos em qualquer posição

        if(flag){
            rules.push_back(r);
        }
    }

    return rules;
} // preenche as regras de acordo com o estado atual. retorna um vetor de inteiros com os valores que podem ser inseridos na posição ij

int** clona_matriz(int **matriz, int n){
    int **copy = new int*[n];

    for(int i = 0; i < n; i++){
        copy[i] = new int[n];
        for(int j = 0; j < n; j++){
            copy[i][j] = matriz[i][j];
        }
    }

    return copy;
} // clona a matri, retorna uma matriz nova

void gulosa(int **matriz, int n, int nao_preenchidos)
{
    int i = n-1; int j = 0; // i=2, j=0 // pode ser passado por parâmetro onde será feita a primeira inserção

    Stack* stack = new Stack();
    set<int**> explored; //permite que não criemos matrizes iguais na exploração

    // int *qtdMatrizNivel = new int[n*(n-1)]; //vetor para armazenar a quantidade de matrizes por nível. O numero de niveis é a quantidade de valores vazios na matriz inicial
    int *qtdMatrizNivel = new int[nao_preenchidos];

    for(int k = 0; k < n * (n-1); k++)
        qtdMatrizNivel[k] = 0;
    //vetor contador para dar o pesar a heurística

    Node *no = new Node(matriz, nullptr, preenche_regras(matriz, n, i, j), i, j, n, 0, nao_preenchidos);
    stack->push(no);

    
    while(true){
        if(stack->is_empty())
        {
            cout << "Nenhuma solução encontrada" << endl;
            exit(1);
        }

        // cout << "Antes" << endl;
        // stack->imprimeAbertos();

        // if(stack->size() != 0) // a cada iteração, faz a busca do nó de menor peso, passa ele para a primeira posição da stack
        // {
        //     int menor = stack->get(0)->w;
        //     // cout << "menor: " << menor << endl;
        //     // cout << "ImprimeMatriz do menor: " << endl;
        //     // imprimeMatriz(stack->get(0)->mat, n);
        //     int pos = 0;
        //     for(int k = 1; k < stack->size(); k++)
        //     {
        //         if(menor > stack->get(k)->w)
        //         {
        //             menor = stack->get(k)->w;
        //             pos = k;
        //         }
        //     }
        //     stack->swap(pos); // pega a posição do menor na stack e coloca ele no ínicio da stack
        //     // cout << "menor: " << menor << endl;
        //     // cout << "ImprimeMatriz Novo menor: " << endl;
        //     // imprimeMatriz(stack->get(0)->mat, n);
        // }
        // cout << "Depois" << endl;
        // stack->imprimeAbertos();

        no = stack->popQueue();
        // cout << "nivel = " << no->i + no->j*n<< endl;
        // imprimeMatriz(no->mat, n);

        //Encontrou solução
        if(no->i == n-1 && no->j == n-1)//imprimir o caminho solução
        {
            cout << "Solucao: " <<endl;
            // imprimeMatriz(no->mat, n);
            // cout << "\n\n\n\n";
            imprimeCaminho(no, n);
            cout << "Solucao encontrada" << endl;
            // imprimeArvore(no, n);
            exit(0);
        }

        explored.insert(no->mat);

        for(size_t k = 0; k < no->actions.size(); ++k)
        {

            int **matriz_aux = clona_matriz(no->mat, n);

            if(no->i < n-1) // estabeleze a posição da próxima inserção
                matriz_aux[no->i+1][no->j] = no->actions[k];
            else
                matriz_aux[0][no->j+1] = no->actions[k];

            if(explored.count(matriz_aux) == 0) // se a matriz não foi explorada ainda, cria um novo nó e insere na stack
            {
                Node *new_no;
                int cont_nao_preench_new = no->cont_nao_preenchidos - 1;
                if (no->i < n - 1)
                {   
                    vector<int> new_actions = preenche_regras(matriz_aux, n, no->i + 1, no->j);
                    qtdMatrizNivel[nao_preenchidos - cont_nao_preench_new - 1] += 1;
                    new_no = new Node(matriz_aux, no, new_actions, no->i + 1, no->j, n, qtdMatrizNivel[nao_preenchidos - cont_nao_preench_new + 1], cont_nao_preench_new);

                }
                else
                {
                    vector<int> new_actions = preenche_regras(matriz_aux, n, 0, no->j + 1);
                    qtdMatrizNivel[nao_preenchidos - cont_nao_preench_new - 1] += 1;
                    new_no = new Node(matriz_aux, no, new_actions, 0, no->j + 1, n, qtdMatrizNivel[nao_preenchidos - cont_nao_preench_new + 1], cont_nao_preench_new);
                }

                stack->push(new_no); //vamos mudar a maniera de inserção
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
    //faz inicializa matriz retornar o número de posições inicializadas

    int nao_preenchidos = n*n - n; // número de posições que não foram inicializadas

    gulosa(matriz, n, nao_preenchidos);
}