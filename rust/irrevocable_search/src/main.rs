use std::io;
use std::process;


fn verifica_conflito_linha(matriz : &mut Vec<Vec<i32>>, n : usize, i : usize, val : i32) -> bool {
    let mut j_prime : usize = 0;

    //Verifico se é diferente de zero, pois significa que aquela posição está preenchida
    while matriz[i][j_prime] != 0 && j_prime < n {

        //Verifico se o cara que vai ser inserido é o mesmo que algum dos que já estão na linha i da matriz
        if matriz[i][j_prime] == val {
            //Se for igual a alguém que já está naquela linha da matriz  
            return true;
        }

        //Atualizo o contador
        j_prime = j_prime + 1;
    }

    //Caso seja diferente de todo mundo na linha
    return false;
}

fn verifica_conflito_coluna(matriz : &mut Vec<Vec<i32>>, n : usize, j : usize, val : i32) -> bool {
    let mut i_prime : usize = 0;

    //Verifico se é diferente de zero, pois significa que aquela posição está preenchida
    while matriz[i_prime][j] != 0 && i_prime < n {

        //Verifico se o cara que vai ser inserido é o mesmo que algum dos que já estão na coluna j da matriz
        if matriz[i_prime][j] == val {
            //Se for igual a alguém que já está naquela coluna da matriz  
            return true;
        }

        //Atualizo o contador
        i_prime = i_prime + 1;
    }

    //Caso seja diferente de todo mundo na coluna
    return false;
}

fn inicializa_matriz(matriz : &mut Vec<Vec<i32>>, n : usize) {
    for i in 0..n {
        matriz[i][0] = (i + 1) as i32;
    }
}

fn imprime_matriz(matriz : &mut Vec<Vec<i32>>, n : usize) {
    for i in 0..n{
        for j in 0..n{
            print!("{:3}", matriz[i][j])
        }
        println!("");
    }
}

fn main() {
    println!("*-*-* Quadrado Latino *-*-*");
    println!("Digite o valor de n: ");
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("Falha ao ler o valor de n!");

    //Conversão para inteiro da string lida
    let n : usize = input.trim().parse().unwrap();

    //Criação da matriz dinâmica nxn preenchida com zeros 
    let mut matriz: Vec<Vec<i32>> = vec![vec![0; n]; n];

    inicializa_matriz(&mut matriz, n);

    //Para cada linha
    for i in 0..n {
        //Para cada coluna a partir da segunda, visto que a primeira já está inicialmente preenchida  
        for j in 1..n {
            //Flag para verificar se houve impasse ou não.
            let mut flag = false;

            //Para cada regra (Usando regras em ordem crescente)
            for val in 1..n+1 {
                if !verifica_conflito_coluna(&mut matriz, n, j, val as i32) &&  !verifica_conflito_linha(&mut matriz, n, i, val as i32){
                    matriz[i][j] = val as i32;
                    flag = true;
                    break;
                }
            }

            //Descomente para usar as regras em ordem decrescente
            // for val in (1..=n).rev() {
            //     if !verifica_conflito_coluna(&mut matriz, n, j, val as i32) &&  !verifica_conflito_linha(&mut matriz, n, i, val as i32){
            //         matriz[i][j] = val as i32;
            //         flag = true;
            //         break;
            //     }
            // }

            if !flag{
                println!("Estado de impasse.");
                process::exit(1);
            }
            
        }
    }

    println!("Possível solução para o quadrado latino de ordem {}x{}", n, n);
    imprime_matriz(&mut matriz, n);
}
