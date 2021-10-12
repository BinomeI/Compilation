#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 26
int LineNumbers = 1; 
char token_buffer[MAX];
typedef enum tokens_types {
    BEGIN,
    END,
    READ,
    WRITE,
    ID,
    INTLATTERAL,
    LPAREN,
    RPAREN,
    SEMICOLON,
    COMMA,
    ASSIGNOP,
    PLUSOP,
    MINUSOP,
    SCANEOF
} token;

typedef enum errors_types {
    ASSIGNOP_ERROR, 
    UNKOWN_CHARACTER, 
}Errors; 


char mots_cle[][MAX] = {
    "BEGIN",
    "END",
    "READ",
    "WRITE",
    "ID",
    "INTLATTERAL",
    "LPAREN",
    "RPAREN",
    "SEMICOLON",
    "COMMA",
    "ASSIGNOP",
    "PLUSOP",
    "MINUSOP",
    "SCANEOF" 
};


void Lexical_error(char car, Errors err) {
    switch (err)
    {
        case ASSIGNOP_ERROR:
            printf("\nLexical Error: - Ligne %d - il faut remplacer le caractere %c par '='\n", LineNumbers, car);
            break;
        case UNKOWN_CHARACTER:
            printf("\nLexical Error: - Ligne %d - Le caractere '%c' est mal reconnue \n", LineNumbers, car); 
            break;  
        
    }
}

void clear_buffer()
{
    for (int i = 0; i < MAX; i++)
    {
        token_buffer[i] = '\0';
    }    
}

void buffer_char(char car){
    for(int i = 0; i < MAX; i++)
    {
        if(token_buffer[i] == '\0'){
            token_buffer[i] = car;
            break; 
        }
    }
             
}

token check_reserved(){
    if(strcmp(strupr(token_buffer), strupr(mots_cle[BEGIN])) == 0)
        return BEGIN;

    if(strcmp(strupr(token_buffer), strupr(mots_cle[WRITE])) == 0)
        return WRITE;

    if(strcmp(strupr(token_buffer), strupr(mots_cle[READ])) == 0)
        return READ;

    if(strcmp(strupr(token_buffer), strupr(mots_cle[END])) == 0)
        return END;

    return ID; 
}



token scanner(FILE* pt)
{
    char in_char, c;
    clear_buffer();
    // rewind
    fseek( pt, 1, SEEK_CUR);
    if(feof(pt)) return SCANEOF;
    while ((in_char = fgetc(pt)) != EOF)
    {
        if (isspace(in_char)) {
            if(in_char == '\n')
                LineNumbers += 1; 
            continue;
        }

        else if (isalpha(in_char)) {
            buffer_char(in_char); 

            for(c = fgetc(pt); isalnum(c) || c == '_'; c = fgetc(pt))
                buffer_char(c); 
            
            ungetc(c, pt); 
            
            return check_reserved(); 
        }

        else if(isdigit(in_char))
        {
            for(c = fgetc(pt); isdigit(c); c=fgetc(pt))
                buffer_char(c); 
            ungetc(c, pt); 
            
            return INTLATTERAL; 
        }
        else if(in_char == '(')
            return LPAREN; 
        
        else if(in_char == ')')
            return RPAREN; 
        
        else if(in_char == ';')
            return SEMICOLON; 
        

        else if(in_char == '+')
            return PLUSOP; 
        
        else if(in_char == ',')
            return COMMA;

        else if(in_char == ':')
        {
            if((c = fgetc(pt)) == '=')
                return ASSIGNOP; 
            
            Lexical_error(c, ASSIGNOP_ERROR);             
            ungetc(c, pt);
             

        }

        else if(in_char == '-')
        {
            if((c = fgetc(pt)) == '-')
            {
                do {
                    in_char = fgetc(pt); 
                }while (in_char != '\n');
                LineNumbers += 1;
                
            } else {
                ungetc(c, pt);
                
                return MINUSOP; 
            }
        }
        else {
            Lexical_error(in_char, UNKOWN_CHARACTER); 
            
        }
    }
    
}

int main(int argc, char const *argv[])
{

    FILE *inF, *outF;
    inF = fopen("InFile.txt","r+");
    outF = fopen("OutFile.txt","w");


    if(inF && outF)
        printf("\nFiles are opened\n"); 


    token t;
    char c; 

    while (feof(inF) == 0)
    {
        ungetc(c, inF); 
        printf("\nEntred to file\n");

        t = scanner(inF);
        printf("%s",mots_cle[t]);
        fprintf(outF,"%s ",mots_cle[t]);
        if (t == SEMICOLON) fprintf(outF,"\n");
        if (t == BEGIN) fprintf(outF,"\n");
        if (t == END) fprintf(outF,"\n");
    }


    fclose(inF);
    fclose(outF);
    
    return 0;
}
