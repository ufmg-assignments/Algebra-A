#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

//funcao de exponenciacao binaria utilizada no tp1
void exp_binaria(mpz_t r,const mpz_t b, const mpz_t e, const mpz_t n){
    
    mpz_t expoente, potencia, dois, resultado, auxiliar;
    mpz_inits(expoente, potencia, dois, resultado, auxiliar, NULL);
    
    //deve-se colocar os valores passados em variaveis, pois eles sao constantes
    mpz_set(expoente,e);
    mpz_set(potencia,b);
    
    //dois sera uma constante auxiliar e o resultado é iniciado com o valor 1
    mpz_set_ui(dois, 2);
    mpz_set_ui(r, 1);
    
    //enquanto o expoente for maior que zero
    while(mpz_cmp_ui(expoente,0) > 0){
        mpz_mod(resultado,expoente,dois);
        
        //analise se o resultado eh impar
        if(mpz_cmp_ui(resultado,1)==0){
            
            //atualizacao do resultado
            mpz_mul(auxiliar, r, potencia);
            mpz_mod(r,auxiliar,n);
        }
        
        //atualizacao do expoente e da potencia
        mpz_mul(potencia,potencia,potencia);
        mpz_mod(potencia,potencia,n);
        mpz_div(expoente,expoente,dois);
    }
    
    //liberacao da memoria
    mpz_clears(expoente, potencia, dois, resultado, auxiliar, NULL);
}

void codifica(mpz_t r, const char *str){
    
    //declaracao das variaveis
    int i = 0;
    mpz_t multiplicador,resultado, termo_soma;
    mpz_inits(multiplicador, resultado, termo_soma, NULL);
    int ascii;
    
    //inicializacao dos valores
    mpz_set_ui(multiplicador, 1);
    mpz_set_ui(resultado, 0);
    
    //enquanto houver caracter, o resultado é incrementado
    while(str[i] != '\0'){
        
        //converte-se o caracter para inteiro
        ascii = (int)str[i];
        
        //realiza-se as operacoes
        mpz_mul_ui(termo_soma, multiplicador, ascii);
        mpz_add(resultado, resultado, termo_soma);
        
        //atualiza-se o multiplicador e o indice da string
        mpz_mul_ui(multiplicador, multiplicador, 256);
        i++;
    }
    
    //atualiza-se a variavel que ira guardar o resultado
    mpz_set(r, resultado);
    
    //liberacao da memoria
    mpz_clears(multiplicador, resultado, termo_soma, NULL);
}


char *decodifica(const mpz_t n){
    
    //declaracao das variaveis
    int aux, indice_atual = 0;
    mpz_t resto_atual, numero;
    mpz_inits(resto_atual,numero, NULL);
    mpz_set(numero, n);
    
    //alocacao da memoria para a string decodificada
    char *msg_dec = malloc(501*sizeof(char));
    
    //enquanto o numero for diferente de 0
    //ha mais caracteres a serem decodificados
    while(mpz_cmp_ui(numero, 0) != 0){
        
        //calculo do valor inteiro correspondente ao caracter em ASCII
        mpz_mod_ui(resto_atual, numero, 256);
        aux = mpz_get_ui(resto_atual);
        
        //conversao do valor obtido
        msg_dec[indice_atual] = (char)aux;
        
        //atualizacao das variaveis
        mpz_sub(numero, numero, resto_atual);
        mpz_div_ui(numero, numero, 256);
        indice_atual++;
    }
    
    //o ultimo caracter indicara o fim da string
    msg_dec[indice_atual] = '\0';
    
    //liberacao da memoria
    mpz_clears(resto_atual,numero, NULL);
    
    return msg_dec;
}

void criptografa(mpz_t C, const mpz_t M, const mpz_t n, const mpz_t e){
    
    //utiliza-se a funcao exp_binaria para se criptografar o numero
    exp_binaria(C, M, e, n);
}

void descriptografa(mpz_t M, const mpz_t C, const mpz_t n, const mpz_t d){
    
    //utiliza-se a funcao exp_binaria para se descriptografar o numero
    exp_binaria(M, C, d, n);
}