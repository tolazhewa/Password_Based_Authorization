#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void find_hash(char *input);
char * separate_hash(char *line);
void trim_return(char *str);
void produce_all_comb();
void E(char *in, char *out);

int main(int argc, const char * argv[]) {
    char *input;

    produce_all_comb();
    input = (char*)calloc(sizeof(char), 6);

    while(fgets(input, 16, stdin)) {
        trim_return(input);
        if(strlen(input) != 4) {
            printf("Invalid input...");
            free(input);
            return -1;
        }
        find_hash(input);
    }
    free(input);
}

void find_hash(char *input){
    FILE *f;
    char *line, *hash;

    f = fopen("allcomb.txt", "r");
    line = (char*)calloc(sizeof(char),128);

    while(fgets(line, 128, f)) { //check every entry
        trim_return(line);
        hash = separate_hash(line);
        if(!strcmp(hash,input)) {
            printf("Hash: %s is: %s\n", hash, line);
            return;
        }
    }
    printf("Hash not found.");
    free(line);
}

char * separate_hash(char *line) {
    while( *line != '-')
        line++;
    *line = '\0';
    line++;
    return line;
}

void trim_return(char *str){
    while(*str != '\n')
        str++;
    *str = '\0';
}

void produce_all_comb(){
    FILE *f;
    char *str, *hash;
    int i,j,k,l;

    f = fopen("allcomb.txt", "w+");

    str = (char *)calloc(sizeof(char), 5);
    hash = (char *)calloc(sizeof(char), 5);
    strcpy(str, "AAAA");

    for(i = 0; i < 26; i++) {
        for(j = 0; j < 26; j++) {
            for(k = 0; k < 26; k++) {
                for(l = 0; l < 26; l++) {
                    E(str,hash);
                    fprintf(f, "%s-%s\n", str, hash);
                    str[3]++;
                }
                str[2]++;
                str[3]-=26;
            }
            str[1]++;
            str[2]-=26;
        }
        str[0]++;
        str[1]-=26;
    }
    fclose(f);
}

void E(char *in, char *out) {
    out[0]=(in[0]&0x80)^(((in[0]>>1)&0x7F)^((in[0])&0x7F));
    out[1]=((in[1]&0x80)^((in[0]<<7)&0x80))^(((in[1]>>1)&0x7F)^((in[1])&0x7F));
    out[2]=((in[2]&0x80)^((in[1]<<7)&0x80))^(((in[2]>>1)&0x7F)^((in[2])&0x7F));
    out[3]=((in[3]&0x80)^((in[2]<<7)&0x80))^(((in[3]>>1)&0x7F)^((in[3])&0x7F));
}
