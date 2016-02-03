#include <unistd.h>
#include <stdio.h>
#define HELPMENUCONSTANT "Constant"

int main(int argc, char *argv[]){

    if(argc ==2){
        printf("argc ==2, cons = %s", HELPMENUCONSTANT);
    }else if (argc > 2){
        printf("argc ==3 or more, 2+ arg");
    }else{
        printf("arg c < 2, probably less than one argument");
    }
}
