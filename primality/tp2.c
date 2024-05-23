#include <stdio.h>
#include <gmp.h>

//funcao fornecida na documentacao
void numero_aleatorio(mpz_t r, gmp_randstate_t rnd, const mpz_t n) {
    mp_bitcnt_t num_bits = mpz_sizeinbase(n, 2);
    do {
        mpz_urandomb(r, rnd, num_bits);
    } while (!(mpz_cmp_ui(r, 1) >= 0 && mpz_cmp(r, n) <= 0));
}

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

int talvez_primo(const mpz_t a, const mpz_t n, const mpz_t n1, unsigned int t, const mpz_t q){
    
    //declaracao das variaveis
    unsigned int expoente2_atual = 1;
    mpz_t resto;
    mpz_init(resto);
    
    //utiliza-se a funcao exp_binaria para calcular o resto inicial
    exp_binaria(resto,a,q,n);
    
    //se o resto inicial for 1, o teste resulta em inconclusivo
    if(mpz_cmp_ui(resto,1) == 0){
        mpz_clear(resto);
        return 1;
    }
    
    //avalia-se ate que o expoente seja maior do que t
    while(expoente2_atual != t+1){
        
        //se o resto for igual a n-1, o teste retorna inconclusivo
        if(mpz_cmp(resto,n1)==0){
            mpz_clear(resto);
            return 1;   
        }
        
        //atualiza-se as variaveis
        mpz_mul(resto,resto,resto);
        mpz_mod(resto,resto,n);
        expoente2_atual +=1;
    }
    
    //liberacao da memoria
    mpz_clear(resto);
    
    //caso nenhuma das condicoes seja satisfeita, n eh composto
    return 0;
}

int provavelmente_primo(const mpz_t n, unsigned int k, gmp_randstate_t rnd){
    
    //declaracao e inicializacao das variaveis utilizadas
    mpz_t n_1, resto_atual, base, aux_impar;
    mpz_inits(n_1, resto_atual, base, aux_impar, NULL);
    unsigned int exp_2 = 0;
    int resultado_iteracao = 0;
    mpz_sub_ui(n_1, n, 1);
    mpz_set(aux_impar, n_1);
    mpz_mod_ui(resto_atual, aux_impar, 2);
    
    //gerar parametros para chamar a funcao talvez_primo
    while(mpz_cmp_ui(resto_atual,1) != 0){
        mpz_div_ui(aux_impar, aux_impar,2);
        mpz_mod_ui(resto_atual, aux_impar, 2);
        exp_2++;
    }
    
    //realizacao das k iteracoes
    for(unsigned int i = 0; i!= k; i++){
        
        //geracao da base 
        numero_aleatorio(base, rnd, n);
        while(mpz_cmp_ui(base,1) == 0 || mpz_cmp_ui(base,0) == 0 || mpz_cmp(base,n) == 0){
           numero_aleatorio(base, rnd, n);
        }
        
        //chama-se a funcao talvez_primo passando os parametros gerados
        resultado_iteracao = talvez_primo(base, n, n_1, exp_2, aux_impar);
        
        //se o resultado for 0, o numero eh composto
        if(resultado_iteracao == 0){
            
            //liberacao da memoria
            mpz_clears(n_1, resto_atual, base, aux_impar, NULL);
            return 0;
        }
    }
    
    //liberacao da memoria
    mpz_clears(n_1, resto_atual, base, aux_impar, NULL);
    
    //caso nenhuma das condicoes nas k iteracoes seja satisfeita, n eh provavelmente primo
    return 1;
    
}

void primo_aleatorio(mpz_t r, unsigned int b, gmp_randstate_t rnd){

    //variaveis utilizadas
    mpz_t primeiro_do_intervalo, primo, complementar_invervalo, maximo_comp;
    mpz_inits(primeiro_do_intervalo, primo, complementar_invervalo,maximo_comp, NULL);
    mpz_set_ui(primeiro_do_intervalo,1);
    unsigned int i = 1;
    
    //primeiro, calcula-se o primeiro numero do intervalo 
    while(i!=b){
        mpz_mul_ui(primeiro_do_intervalo,primeiro_do_intervalo,2);
        i++;
    }
    
    //gera-se um numero aleatorio menor do que o primeiro do intervalo
    mpz_sub_ui(maximo_comp, primeiro_do_intervalo, 1);
    numero_aleatorio(complementar_invervalo, rnd, maximo_comp);
    
    //assim, consegue-se gerar um numero que respeite o intervalo estabelecido
    //somando-se o primeiro do intervalo com o aleatorio gerado
    mpz_add(primo, primeiro_do_intervalo,complementar_invervalo);
    
    //analisa-se o numero gerado utilizando a funcao provavelmente_primo
    //deve se utilizar 20 como numero de repeticoes para garantir que a 
    //probabilidade de ser primo seja tal como especificado na descricao do tp
    int resultado = provavelmente_primo(primo, 20, rnd);
    
    //enquanto a funcao retornar que numero gerado for composto, repete-se o procedimento
    while(resultado != 1){
        numero_aleatorio(complementar_invervalo, rnd, maximo_comp);
        mpz_add(primo, primeiro_do_intervalo,complementar_invervalo);
        resultado = provavelmente_primo(primo, 20, rnd);
    }
    
    //atualiza-se a variavel que ira guardar o resultado
    mpz_set(r, primo);
 
    //liberacao da memoria
    mpz_clears(primeiro_do_intervalo, primo, complementar_invervalo,maximo_comp, NULL);
}