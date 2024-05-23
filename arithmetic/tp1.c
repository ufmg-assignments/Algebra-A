#include <stdio.h>
#include <gmp.h>

void mdc_estendido(mpz_t g, mpz_t x, mpz_t y, const mpz_t a, const mpz_t b){
    
    if (mpz_cmp_ui(b,0) == 0){
        mpz_set_ui(x,1);
        mpz_set_ui(y,0);
        
        //quando b for 0, o mdc sera o valor de a
        mpz_set(g,a);
    }
    else{
        mpz_t modulo;
        mpz_init(modulo);
        mpz_mod(modulo,a,b);
        
        //nova chamada recursiva, passando b e o modulo como parametros
        mdc_estendido(g, x, y, b, modulo);
        
        //calculo do quociente
        mpz_t auxiliar, produto, quociente;
        mpz_inits(auxiliar, produto, quociente, NULL);
        mpz_set(auxiliar,y);
        mpz_div(quociente, a, b);
        
        //calculo de x e y 
        mpz_mul(produto, quociente, y);
        mpz_sub(y, x, produto);
        mpz_set(x,auxiliar);
    
        //liberacao da memoria
        mpz_clears(modulo,auxiliar,produto,quociente, NULL);
    }
}

int inverso_modular(mpz_t r, const mpz_t a, const mpz_t n){
    
    mpz_t mdc, var_auxiliar,auxiliar;
    mpz_inits(mdc, var_auxiliar, auxiliar, NULL);
    
    //usa-se a funcao anterior para calculo do mdc
    mdc_estendido(mdc,var_auxiliar,r,n,a);
    
    int resposta;
    
    //se o mdc for 1, existe solucao
    if (mpz_cmp_ui(mdc,1) == 0){
        resposta = 1;
        
        //para evitar resultados negativos, 
        mpz_mod(r,r,n);
    }
    else{
        resposta = 0;
    }
    
    //liberacao da memoria
    mpz_clears(mdc, var_auxiliar, NULL);
    
    return resposta;
}

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