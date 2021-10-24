#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "header.h"

int LineNumbers = 1; 
char token_buffer[MAX];


void Lexical_error(char car, Errors err) {
    switch (err)
    {
        case ASSIGNOP_ERROR:
            printf("\nLexical Error: - Ligne %d - il faut remplacer le caractere %c par '='\n", LineNumbers, car);
            break;
        case UNKOWN_CHARACTER:
            printf("\nLexical Error: - Ligne %d - Le caractere '%c' est mal reconnue \n", LineNumbers, car); 
            break;  
        
        case REEL_ERROR: 
            printf("\nLexical Error: - Ligne %d - Le  Nombre reel est mal traite\n", LineNumbers); 
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

Errors TraitementAfterE(FILE *pt) {
    //index = 1 := calling the func after digits 
    //index = 2 := calling the func when detecting a point as a scanner first character 

    char c;
    int existCara = 0; 
    char prev = 'e';
    for(c = fgetc(pt); isdigit(c) || c == '.' || c == 'e' || c == 'E'|| c == '-'|| c == '+'; c = fgetc(pt))
    {
        switch (c)
        {
        case '.':
            return REEL_ERROR; 
            break;
        case '+':
            if (prev != 'e' && prev != 'E')
            {
                ungetc(c, pt); 
                return NO_ERROR;
            }            
            break;
        case '-':
            if (prev != 'e' && prev != 'E')
            {
                ungetc(c, pt); 
                return NO_ERROR;
            }            
            break;
        case 'E':
        case 'e':
            return REEL_ERROR;
            break;  
        }
        prev = c; 
    }
        
    ungetc(c, pt); 
    return NO_ERROR; 
}


Errors TraitementAfterPoint(FILE *pt, int index) {
    //index = 1 := calling the func after digits 
    //index = 2 := calling the func when detecting a point as a scanner first character 

    char c;
    int checkedE=0;
    int existCara = 0; 
    char prev = '.';
    for(c = fgetc(pt); isdigit(c) || c == '.' || c == 'e' || c == 'E'|| c == '-'|| c == '+'; c = fgetc(pt))
    {
        switch (c)
        {
        case '.':
            return REEL_ERROR; 
            break;
        case '+':
            if (prev != 'e' && prev != 'E')
            {
                ungetc(c, pt); 
                return NO_ERROR;
            }            
            break;
        case '-':
            if (prev != 'e' && prev != 'E')
            {
                ungetc(c, pt); 
                return NO_ERROR;
            }            
            break;
        case 'E':
        case 'e':
            if (!checkedE)
            checkedE =1;
            else return REEL_ERROR;
            break;  
        }
        prev = c; 
    }
    if(prev == '.' && index == 2){

        ungetc(c, pt);
        return UNKOWN_CHARACTER;  
    }
        
    ungetc(c, pt); 
    return NO_ERROR; 
}


token scanner(FILE* pt)
{
    char in_char, c;

    int checkE = 0; 
    int checkPoint = 0; 

    token t; 

    clear_buffer();
    // rewind
    fseek( pt, 1, SEEK_CUR);
    if(feof(pt)) 
        return SCANEOF;

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
 
            Errors err1; 
            for(c = fgetc(pt); isdigit(c) || c == '.'|| c == 'e'|| c == 'E'; c=fgetc(pt)){
                buffer_char(c);
                if(c == '.') {
                    err1 = TraitementAfterPoint(pt, 1);
                    if(err1 != NO_ERROR) 
                        Lexical_error(in_char, err1); 
                    else  
                        return FLOAT;      
                }      
                else if(c == 'E' || c == 'e')
                {
                    err1 = TraitementAfterE(pt);
                    if(err1 != NO_ERROR) 
                        Lexical_error(in_char, err1); 
                    else  
                        return FLOAT;  
                }       
            }
            
                 
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

        else if(in_char == '.')
        {
            Errors err = TraitementAfterPoint(pt, 2); 
            if(err == NO_ERROR)
                return FLOAT;

            else 
                Lexical_error(in_char, err);  
        }
             
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

    token t;
    char c; 

    while (feof(inF) == 0)
    {
        ungetc(c, inF); 
        
        t = scanner(inF);

        fprintf(outF,"%s ",mots_cle[t]);
        if (t == SEMICOLON) fprintf(outF,"\n");
        if (t == BEGIN) fprintf(outF,"\n");
        if (t == END) fprintf(outF,"\n");
    }

    fclose(inF);
    fclose(outF);
    
    return 0;
}
