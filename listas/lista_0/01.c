#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Temos arrays de ponteiros!
char *unidades[] = {"", "UM", "DOIS", "TRES", "QUATRO", "CINCO", "SEIS", "SETE", "OITO", "NOVE"};
char *de10a19[] = {"DEZ", "ONZE", "DOZE", "TREZE", "CATORZE", "QUINZE", "DEZESSEIS", "DEZESSETE", "DEZOITO", "DEZENOVE"};
char *dezenas[] = {"","DEZ", "VINTE", "TRINTA", "QUARENTA", "CINQUENTA", "SESSENTA", "SETENTA", "OITENTA", "NOVENTA"};
char *centenas[] = {"CEM", "CENTO", "DUZENTOS", "TREZENTOS", "QUATROCENTOS", "QUINHENTOS", "SEISCENTOS", "SETECENTOS", "OITOCENTOS", "NOVECENTOS"};

void converterNumero(int num){
    if(num == 0){
        return;
    }

    if(num >= 100){
        if(num == 100){
            printf(" CEM ");
        } else{
            printf(" %s ", centenas[num/100]);
        }

        num %= 100;
        if(num > 0){
            printf(" E ");
        }
    }

    if (num >= 20) {
        printf(" %s ", dezenas[num / 10]);
        num %= 10;
        if (num > 0) {
            printf(" E ");
        }
    } else if (num >= 10) {
        printf("%s", de10a19[num - 10]);
        return;
    }

    if (num > 0) {
        printf("%s", unidades[num]);
    }
}

int main(){

    int t;
    printf("Insira a quantidade de casos testes: ");
    scanf("%d", &t); 

    while (t--) {
        double valor;
        scanf("%lf", &valor); 

        int reais = (int)valor;
        int centavos = round((valor - reais) * 100); 

        // Imprimindo a parte inteira (reais)
        if (reais > 0) {
            converterNumero(reais);
            if (reais == 1){
                printf(" REAL");
            }else{
                printf(" REAIS");
            }
        }

        // Imprimindo a parte decimal (centavos)
        if (centavos > 0) {
            if (reais > 0) {
                printf(" E ");
            }
            converterNumero(centavos);
            if (centavos == 1){
                printf(" CENTAVO");
            }else{
                printf(" CENTAVOS");
            }
        }

        printf("\n");
    }

    return 0;
}