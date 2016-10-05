/* Name: Tolaz Hewa, Alessandro Profenna, Shiqi Dong
 * Date: October 2, 2016
 * Course: CPS633 (Computer Security)
 * Lab: #1
 * Question: #1
 * Description: An authentication system
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void view_options();
void E(char *in, char *out);
char * separate_hash(char *line);
void trim_return(char *str);
void view_table();
char * hash_input_password(char *input_pass);
int password_input_check(char * pass);
void get_pass(char *pass, char *output);
void to_upper_case(char *input);
void new_password(char *un, char *hashed_input);
void check_user(char * un);
void run_process();

//main method that runs everything
int main(int argc, const char * argv[]) {
    FILE *f;
    char *str;

    f = fopen("table.txt", "ab+");

    str = (char*)calloc(sizeof(char), 3);
    while(1) {
        view_options();
        fgets(str, 3, stdin);

        if(strcmp(str, "1\n") == 0) {
            view_table();
        } else if(strcmp(str, "2\n") == 0) {
            run_process();
        } else if(strcmp(str, "3\n") == 0) {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid input!\n");
        }
    }
    fclose(f);
    free(str);
}

//Views the main menu options
void view_options(){
    printf("\n\nPlease select one of the following options:\n\n");
    printf("1. View table\n");
    printf("2. Run program\n");
    printf("3. Exit\n\n");
    printf(":");
}

// DES replacement cipher
// The function E takes 4 bytes from *in as input and
// writes 4 bytes to *out
void E(char *in, char *out) {
    out[0]=(in[0]&0x80)^(((in[0]>>1)&0x7F)^((in[0])&0x7F));
    out[1]=((in[1]&0x80)^((in[0]<<7)&0x80))^(((in[1]>>1)&0x7F)^((in[1])&0x7F));
    out[2]=((in[2]&0x80)^((in[1]<<7)&0x80))^(((in[2]>>1)&0x7F)^((in[2])&0x7F));
    out[3]=((in[3]&0x80)^((in[2]<<7)&0x80))^(((in[3]>>1)&0x7F)^((in[3])&0x7F));
}


/*
 * separates the user and the hashed password from the whole line
 * returns the hashed_password
 */
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

//views the whole table of username and hashed passwords
void view_table(){
    char *user;
    char *hash;
    FILE *f;
    int i;

    user = (char*)calloc(sizeof(char), 64);
    f = fopen("table.txt", "r");

    printf("\n\tUSERNAME\tPASSWORD-HASH\n\n");
    while(fgets(user, 64, f) != NULL) {
        hash = separate_hash(user);
        printf("\t%s", user);
        if(strlen(user) < 15) {
            for(i = 0; i < 15 - strlen(user); i++) {
                printf(" ");
            }
        }
        printf("\t%s",hash);
    }
    printf("\n\n");
    free(user);
    fclose(f);
}

//hash function to hash the password
char * hash_input_password(char *input_pass){
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

//checks the validity of the password entry
int password_input_check(char * pass) {
    while(*pass != '\0') {
        if(!((*pass > 64 && *pass < 91) || (*pass > 47 && *pass < 58))) {
            return -1;
        }
        pass++;
    }
    return 1;
}

void to_upper_case(char *input){
    while(*input != '\0'){
        if(*input > 96 && *input < 123)
            *input -= 32;
        input++;
    }
}

//Replace the current password with a new one.
void new_password(char *un, char *hashed_input) {
    FILE *f1, *f2;
    char * line;
    char * hash;

    line = (char*)calloc(sizeof(char), 64);
    hash = (char*)calloc(sizeof(char), 32);

    f1 = fopen("table.txt", "r");
    f2 = fopen("temp.txt", "w+");

    while(fgets(line, 64, f1)) {
        trim_return(line);
        hash = separate_hash(line);
        if(strcmp(line,un) == 0)
            fprintf(f2, "%s~%s\n",un,hashed_input);
        else
            fprintf(f2, "%s~%s\n",line,hash);
    }
    remove("table.txt");
    rename("temp.txt","table.txt");
    fclose(f1);
    fclose(f2);
}

//Method to check user information
void check_user(char * un) {
    FILE *f;
    char *user,*hash,*pass_input,*hashed_input;
    int counter;

    user = (char*)calloc(sizeof(char),32);
    pass_input = (char*)calloc(sizeof(char),128);
    f = fopen("table.txt", "r");
    counter = 0;

    while(fgets(user, 32, f)) { //check every user
        trim_return(user);
        separate_hash(user);
        if(strcmp(un,user) == 0) { //checks if user matches a record
            hash = &user[strlen(user)+1];
            while(counter < 3) {
                get_pass(pass_input, "Enter Password: ");
                hashed_input = hash_input_password(pass_input); //hashes the password

                if(strcmp(hashed_input,hash) == 0) { //checks if the input is verified
                    printf("Verified!\n");
                    get_pass(pass_input, "Please enter a new password: ");
                    hashed_input = hash_input_password(pass_input);
                    new_password(un, hashed_input);
                    return;
                } else {
                    printf("not verified\n");
                    counter++;
                }
            }
            if(counter >= 3) {
                printf("You have put in three unsuccessful attempts.\n\n\n");
                fclose(f);
                free(user);
                free(hashed_input);
                free(pass_input);
                return;
            }
        }
    }

    if(counter < 3) { //checks counter count
        get_pass(pass_input, "New username, enter a password: ");
        hashed_input = hash_input_password(pass_input);

        f = fopen("table.txt", "a");
        fprintf(f, "%s~%s\n", un, hashed_input);
        printf("Thank you :)\n");
        fclose(f);
        free(user);
        free(hashed_input);
        free(pass_input);
        return;
    }
    fclose(f);
    if(user != NULL)
        free(user);
    if(hashed_input != NULL)
        free(hashed_input);
    if(pass_input != NULL)
        free(pass_input);
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

//runs the program
void run_process() {
    char *input;
    input = (char*)calloc(sizeof(char), 64);

    printf("Enter Username: ");
    fgets(input, 64, stdin);
    trim_return(input);

    while(strlen(input) < 4 || strlen(input) > 32) {
        printf("Invalid username (username has to be between 4 and 32 characters)\nEnter Username: ");
        fgets(input, 32, stdin);
        trim_return(input);
    }

    check_user(input);
    if(input != NULL)
        free(input);
}
