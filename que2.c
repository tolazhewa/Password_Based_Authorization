#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void E(char *in, char *out);
int check_user(char *un);
void check_pass(char *un, char* hashed_input, int r);
void trim_return(char *str);
char * hash_input_password(char *input_pass, int r);
char * separate_hash(char *line);
int password_input_check(char * pass);
void to_upper_case(char *input);
int XOR(char *hash, int rnd);
void get_pass(char *pass_input, char *output);


//main method that runs everything
int main(int argc, const char * argv[]) {
    FILE *f;
    int r;
    char *un, *pass_input, *hashed_input;

    f = fopen("table.txt", "r");
    if(!f) {
      printf("File not found, exiting program...\n");
      return -1;
    }

    srand(time(NULL));
    r = rand();

    un = (char*)calloc(sizeof(char), 64);
    pass_input = (char*)calloc(sizeof(char), 128);

    printf("Enter Username: ");
    fgets(un, 64, stdin);
    trim_return(un);
    if(!check_user(un)) {
      printf("User not found, exiting program...\n");
      return -2;
    }
    get_pass(pass_input,"Enter Password: ");
    hashed_input = hash_input_password(pass_input,r); //hashes the password
    check_pass(un,hashed_input,r);

    free(un);
    free(pass_input);
    fclose(f);
}

int check_user(char *un) {
  FILE *f;
  char *line;

  f = fopen("table.txt", "r");

  line = (char*)calloc(sizeof(char), 128);

  while(fgets(line, 128, f)) { //check every user
      trim_return(line);
      separate_hash(line);
      if(!strcmp(un,line)){
        fclose(f);
        return 1;
      }
  }
  fclose(f);
  return 0;

}


void check_pass(char* un, char * hashed_input, int r) {
    FILE *f;
    f = fopen("table.txt", "r");
    char *user, *hash;
    int ip1, ip2, ip3, p1, p2, p3;

    //client password
    ip1 = XOR(hashed_input,r);
    ip2 = XOR(&hashed_input[4],r);
    ip3 = XOR(&hashed_input[8],r);

    user = (char*)calloc(sizeof(char), 128);

    //server password
    while(fgets(user, 128, f)) { //check every user
        trim_return(user);
        separate_hash(user);
        if(!strcmp(un,user)) { //checks if user matches a record
            hash = &user[strlen(user)+1];
            p1 = XOR(hash,r);
            p2 = XOR(&hash[4],r);
            p3 = XOR(&hash[8],r);
            if(ip1 == p1 && ip2 == p2 && ip3 == p3)
                printf("access granted\n");
            else
                printf("access denied\n");
        }
    }
    fclose(f);
}

int XOR(char *hash, int rnd) {
    return rnd^*(int*)hash;
}

int password_input_check(char * pass) {
    while(*pass != '\0') {
        if(!((*pass > 64 && *pass < 91) || (*pass > 47 && *pass < 58))) {
            return -1;
        }
        pass++;
    }
    return 1;
}

void to_upper_case(char *input) {
    while(*input != '\0') {
        if(*input > 96 && *input < 123)
            *input -= 32;
        input++;
    }
}

char * separate_hash(char *line) {
    while( *line != '~')
        line++;
    *line = '\0';
    line++;
    return line;
}

//takes off the \n from fgets inputs
void trim_return(char *str){
    while(*str != '\n')
        str++;
    *str = '\0';
}

void E(char *in, char *out) {
    out[0]=(in[0]&0x80)^(((in[0]>>1)&0x7F)^((in[0])&0x7F));
    out[1]=((in[1]&0x80)^((in[0]<<7)&0x80))^(((in[1]>>1)&0x7F)^((in[1])&0x7F));
    out[2]=((in[2]&0x80)^((in[1]<<7)&0x80))^(((in[2]>>1)&0x7F)^((in[2])&0x7F));
    out[3]=((in[3]&0x80)^((in[2]<<7)&0x80))^(((in[3]>>1)&0x7F)^((in[3])&0x7F));
}

char * hash_input_password(char *input_pass, int r){
    char *a, *b, *c, *tr;
    tr = (char*)calloc(sizeof(char),13);

    a = input_pass;
    b = &input_pass[4];
    c = &input_pass[8];

    E(a,tr);
    E(b,&tr[4]);
    E(c,&tr[8]);
    tr[12] = '\0';

    return tr;
}

void get_pass(char *pass_input, char *output) {
  int i;

  printf("%s", output);
  fgets(pass_input, 128, stdin);
  trim_return(pass_input);
  to_upper_case(pass_input);

  while(password_input_check(pass_input) != 1) { //checks the validity of password inputted
      printf("invalid password entry (only A-Z, a-z, and 0-9 are allowed)\nEnter Password: ");
      fgets(pass_input, 128, stdin);
      trim_return(pass_input);
      to_upper_case(pass_input);
  }
  if(strlen(pass_input) < 12) { //makes sure the input is no less than 12 (fills everything else with /0s)
      for(i = 0; i < 12 - strlen(pass_input); i++) {
          pass_input[strlen(pass_input)+i] = '\0';
      }
  } else if(strlen(pass_input) > 12) {
      for(i = 0; i < strlen(pass_input) - 12; i++) {
          pass_input[12+i] = '\0';
      }
  }
}
