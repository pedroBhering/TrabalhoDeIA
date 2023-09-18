#include <iostream>
#include <random>
#include <No.h>

using namespace std;

void imprimeMat(char** mat, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++)
            cout << mat[i][j] << "\t";
        cout << endl;
    }
}

void regrasPossiveis(char** mat, char* vet, int n, int line , int col, No* p){
    bool passCol, passLine;
    
    if(line == 0 && col == 0)
    {
        for(int i = 0; i < n; i++)
        {
            p->empilhar(vet[i]);
        }
    }

    if(line < n && col < n){
        for(int i = 0 ; i < n; i++){
            passCol = true;
            passLine = true;
            for(int j = 0; j < line; j++)
                if(vet[i] == mat[j][col])
                    passCol = false;
            if(passCol){
                for(int j = 0; j < col; j++)
                    if(vet[i] == mat[line][j])
                        passLine = false;
                if(passLine)
                    p->empilhar(vet[i]);
            }
        }

    }

}


No* R(char** mat, int* i, int* j, int* caracteresInseridos, No* p, int n){
    
    No* q = new No();

    if(p->pilha.size() == 0){

        *caracteresInseridos = *caracteresInseridos - 1;
        mat[p->getPai()->i][p->getPai()->j] = '1';  
        q = p->getPai();

        if(*i == 0){
            *i = n - 2;
            *j = *j - 2;
        }

        else
            *i = *i - 2;

        //cout << "\nVoltou para o pai" << endl;
    }

    else{
        mat[*i][*j] = p->desempilhar();
        p->i = *i;
        p->j = *j;
         *caracteresInseridos = *caracteresInseridos + 1;
        q->setPai(p);
        //cout << "\nInseriu: " << mat[*i][*j] << endl; 
    }

    return q;
}


bool verificaSol(char** mat, int n){
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            if(mat[i][j] == '1')
                return false;
    return true;
}


void algoritmoBacktracking(char ** mat, char* vet, int n){
    int line = 0;
    int col = 0;  
    int caracteresInseridos = 0;

    No* p = new No(); 
    
    while(caracteresInseridos < n * n){

        if(p->pilha.size() == 0 && p->visit == false){
            regrasPossiveis(mat, vet, n, line, col, p);
        }

        p->visit = true;
        p = R(mat, &line, &col, &caracteresInseridos, p, n);


        if(line < n)
            line++;
        if(line == n){
            col++;
            line = 0;
        }
        
        //imprimeMat(mat, n);
    }
}

void preencheVet(char *vet, int n){
    for(int i = 0; i < n; i++)
        vet[(n-1) - i] = 'a' + i;
}

int getRandomNumber(int min, int max) {
    std::random_device rd;  // Semente aleatória do dispositivo
    std::mt19937 mt(rd()); // Mersenne Twister 19937 gerador
    std::uniform_int_distribution<int> dist(min, max - 1); // Distribuição uniforme entre min e max - 1

    return dist(mt);
}

bool verificaNum(char *vet, int n, char c){
    for(int i = 0; i <n; i++){
        if(vet[i] == c)
            return false;
    }
    return true;
}

void preencheVetAleatorio(char* vet, int n){
    for(int i = 0; i < n; i++){
        int number = getRandomNumber(0, n);
        while(!verificaNum(vet, n, 'a' + number))
            number = getRandomNumber(0, n);
        vet[i] = 'a' + number;
    }
}

int main(){

    int n;
    cout << "Digite a ordem do quadrado sendo um numero de 0 a 10: ";
    cin >> n;

    char **mat = new char *[n];

    for(int i = 0 ; i < n; i++)
       mat[i] = new char[n];
    
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            mat[i][j] = '1';
        
    char *vet = new char[n];
    char *caminhoSol = new char[n];
    preencheVet(vet, n);
    // preencheVetAleatorio(vet, n);

    algoritmoBacktracking(mat, vet, n);

    cout << "\nSolucao: \n";

    for (int i = 0; i < n; i++)
        for(int j = 0; j < n; j++){
            if(i == n - 1 && j == n - 1)
                cout << mat[j][i] << endl;
            else
                cout << mat[j][i] << ", ";
        }

    delete []mat;
    delete []vet;

    return 0;
}