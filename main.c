#include<stdio.h>
#include "buffer.h"

int main(){
    Buffer *temp= b_alloc(1,1, 'm');

    FILE * fi = fopen("text","r");

    if(temp == NULL){
        printf("Yes its NUll");
    }
    
    while(!feof(fi)){
        char ch = fgetc(fi);
        b_compact(temp,ch);
    }

    b_print(temp,1);
    return 0;
}
