//
//  main.c
//  633_Lab_1
//
//  Created by Tolaz on 2016-09-22.
//  Copyright © 2016 Tolaz. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void view_options(){
    printf("Please select one of the following options:\n\n");
    printf("1. View table\n");
    printf("2. Run program\n");
    printf("3. Exit\n");
}

int main(int argc, const char * argv[]) {
    FILE *f;
    char *str;
    
    str = (char*)calloc(sizeof(char), 4);
    f = fopen("table.txt", "wb+");
    
    fprintf(f, "Hello There!");
    
    view_options();
    fgets(str, 4, stdin);
    
    while(1) {
        if(strcmp(str, "1\n") == 0){
            printf("You have chosen to view the table\n");
        } else if(strcmp(str, "2\n") == 0){
            printf("You have chosen to the run the program\n");
        } else if(strcmp(str,"3\n") == 0){
            printf("You have chosen to exit\n");
            return 0;
        } else {
            printf("Invalid input!\n");
        }
    }
}
