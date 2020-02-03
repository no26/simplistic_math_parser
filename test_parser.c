#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "math_parser.h"

int main(int argc, char *argv[])
{
    int ret;
    double result;
    char *mystr;
    time_t startch, stopch;


    if(argc != 2) {
        puts("Invalid syntax : test_parser mathExpression");
        return 1;
    }

    mystr = malloc(strlen(argv[1]) * sizeof(*mystr));
    strcpy(mystr,argv[1]);

    printf("math expression : %s \n", mystr);

    startch = clock();

    ret = eval_str_to_bt(mystr,&result);

    stopch = clock();

    switch(ret) {
    case -1:
        puts("Erreur... null pointer");
        break;

    case -2:
        puts("Erreur... internal malloc");
        break;

    case -3:
        puts("Erreur... invalid mathexpr syntaxe");
        break;

    default:
        printf("Le resultat est : %f \n", result);
    }

    printf("Temps d'execution : %f \n",(float) ((stopch - startch) / CLOCKS_PER_SEC));


    free(mystr);
    return 0;
}



