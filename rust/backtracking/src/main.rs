
use std::io;

fn inicializa_matriz(matriz : &mut Vec<Vec<i32>>, n : usize){
    // Preencho a primeira coluna
    for i in 0..n{
        matriz[i][0] = (i+1) as i32;
    }
}

fn imprime_matriz(matriz : &Vec<Vec<i32>>, n : usize){
    for i in 0..n{
        for j in 0..n{
            print!("{:3}", matriz[i][j]);
        }
        println!("");
    }
}


fn backtracking(matriz : &mut Vec<Vec<i32>>, n : usize) -> bool
{
    return backtracking_aux(matriz, &mut 0, &mut 1, n, &mut 0);
}

fn backtracking_aux(matriz : &mut Vec<Vec<i32>>, i : &mut usize, j : &mut usize, n : usize, num_atribuicoes : &mut i32) -> bool
{
    let n_aux : i32 = n as i32;

    if (*num_atribuicoes) == n_aux.pow(2) - n_aux {
        imprime_matriz(matriz, n);
        return true; //Busca bem-sucedida
    }

    //Preenche os disponíveis com as regras disponíveis para aplicar naquele estado da matriz. Cada regra leva a um estado válido dentro do quadrado latino.
    let disponives = preenche_regras(matriz, n, *i, *j);

    for regra in disponives{
        matriz[*i][*j] = regra;
        
        //Atualiza i e j para a próxima chamada!
        if *i == n - 1{
            *i = 0;
            *j += 1;
        }else {
            *i += 1;
        }

        //Atualizo o número de atribuições
        *num_atribuicoes += 1;

        if backtracking_aux(matriz, i, j, n, num_atribuicoes){
            return true;
        }

        matriz[*i][*j] = 0;//Desfaço o valor que eu atribui

        //Desfaz por causa do backtracking
        if *i == 0{
            *i = n - 1;
            *j -= 1;
        }else {
            *i -= 1;
        }

        *num_atribuicoes -= 1;
    }

    return false;
}

fn preenche_regras(matriz: &Vec<Vec<i32>>, n : usize, i : usize, j : usize) -> Vec<i32> {

    //Para cada regra 
    let mut rules : Vec<i32> = Vec::new();

    //Para cada regra possível 
    for r in 1..n+1 {
        let mut flag = true;

        //Testar para coluna
        for x in 0..i{
            if matriz[x][j] == (r as i32) {
                flag = false;
                break;
            }
        }

        if flag{
            
            //Testar para linha
            for y in 0..j{
                if matriz[i][y] == (r as i32){
                    flag = false;
                    break;
                }
            }
        }

        //Adiciono na lista de regras que podem ser utilizadas 
        if flag {
            rules.push(r as i32);
        }
    } 

    return rules;
}

fn main(){
    println!("*-*-* Quadrado Latino *-*-*");
    println!("Digite o valor de n: ");
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("Falha ao ler o valor do n!");

    let n : i32 = input.trim().parse().unwrap();

    let mut matriz : Vec<Vec<i32>> = vec![vec![0; n as usize]; n as usize];

    inicializa_matriz(&mut matriz, n as usize);
    
    println!("Resultado da busca: {}", backtracking(&mut matriz, n as usize));
}