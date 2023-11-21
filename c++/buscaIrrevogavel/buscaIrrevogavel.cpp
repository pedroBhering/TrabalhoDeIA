#include <iostream>
#include <random>

using namespace std;

void imprimeMat(char** mat, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++)
            cout << mat[i][j] << "\t";
        cout << endl;
    }
}


char verificaRegra(char** mat, char* vet, int n, int line , int col){
    bool passCol, passLine;
    
    if(line == 0 && col == 0)
        return vet[0];

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
                    return vet[i];
            }
        }

    }

    return ' ';
}

bool R(char** mat, int* caracteresInseridos, char c, int i, int j){
    
    if(c == ' '){
        cout << "Estado de impasse" << endl;
        cout << "Busca termina em falha" << endl;
        return false;
    }

    else{
        mat[i][j] = c;
        *caracteresInseridos = *caracteresInseridos + 1;
    }
    return true;
}


bool verificaSol(int* caracteresInseridos, int n){
        if(*caracteresInseridos < n * n)
            return false;
    return true;
}

void algoritmoIrrevogavel(char** mat, char* vet, int n){
    bool s = false;
    int line = 0;
    int col = 0;
    int caracteresInseridos = 0;

    while(!s &&  R(mat, &caracteresInseridos, verificaRegra(mat, vet, n, line, col), line, col)){
        
        if(verificaSol(&caracteresInseridos, n)){
            s = true;
            cout << "Encotrou solucao" << endl;
        }

        if(line < n)
            line++;
        if(line == n){
            col++;
            line = 0;
        }
    }

    imprimeMat(mat, n);
}

void preencheVet(char *vet, int n){
    for(int i = 0; i < n; i++)
        vet[i] = 'a' + i;
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
    
    //preencheVet(vet, n);
    preencheVetAleatorio(vet, n);

    algoritmoIrrevogavel(mat, vet, n);

    delete []mat;
    delete []vet;

    return 0;
}