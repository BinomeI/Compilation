#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void syntax_error(token tok, int index);
void clear_current();
token next_token();
void match(token t);
void inst(void);
void inst_list(void);
void program();
void system_goal(void);
void id_list (void);
void expression(void);
void expr_list(void);
void add_opp(void);
void prim(void);

void syntax_error(token tok, int index){
    // index = 0: interieur de match
    // index = 1: interieur de default
    if(index == 1){
        printf("\ninto default\n");
    }
    else
        printf("\ninto match\n");
    printf("\n Error at line: %d ==> Key Word: '%s' expected instead of: '%s'\n", LineNumbers, mots_cle[tok], current_token); 
}


void clear_current()
{
    for (int i = 0; i < MAX; i++)
    {
        current_token[i] = '\0';
    } 
}

token next_token() {
    char c;
    char localBuffer[MAX];
    clear_current(); 
    // fscanf(fil, "%s", current_token); 
    int lecIndex = 0;
    while (1)
    {
        c = fgetc(fil);
        if(c=='\n')
        {
            LineNumbers++;
            break;
        }
        if(isspace(c))break;
        current_token[lecIndex]=c;
        lecIndex++;
    }
        

    printf("'%s'", current_token);
    for (int i = 0; i < MAX; i++)
        if(strcmp(current_token, mots_cle[i]) == 0)
            return i;    
    
    printf("\nWeirred error occured\n");
    
}


void match(token t)
{
    if(strcmp(mots_cle[t],current_token) != 0)
        syntax_error(t, 0);
}

void inst(void)
{
    token tok = next_token(); 
    switch(tok) {
        case ID: 
            match(ID);
            tok = next_token();
            match(ASSIGNOP); 
            expression(); match(SEMICOLON); 
            break; 
        
        case READ: 
            match(READ); 
            tok = next_token();
            match(LPAREN); 
            id_list(); 
            match(RPAREN); 
            tok = next_token();
            match(SEMICOLON); 
            break; 
        
        case WRITE: 
            match(WRITE); 
            tok = next_token();
            match(LPAREN); 
            expr_list(); 
            match(RPAREN); 
            tok = next_token();
            match(SEMICOLON); 
            break; 
        
        default: 
            syntax_error(tok, 1); 
            break; 
    }
}

void inst_list(void)
{
    // <inst_list> ::= <inst> {<inst>}
    inst(); 
    while(1){
        switch(next_token()) {
            case ID: 
            case READ: 
            case WRITE: 
                inst(); 
                break; 
            default: 
                return; 
        }
    }
}

void program(){
    match(BEGIN); 
    inst_list(); 
    match(END); 
}

void system_goal(void){
    program(); 
    match(SCANEOF); 

}

void id_list (void)
{
    token tok;
    match(ID);
    while(next_token() == COMMA)
    {
        match(COMMA);
        tok = next_token();
        match(ID);
    }
}

void expression(void)
{
    token t;
    prim();
    for(t=next_token(); t==PLUSOP || t==MINUSOP; 
        t=next_token())
        {
            add_opp();
            prim();
        }
}

void expr_list(void)
{
    expression();
    while(next_token()==COMMA)
    {
        add_opp();
        prim();
    }
}

void add_opp(void)
{
    token tok = next_token();
    if(tok == PLUSOP || tok==MINUSOP)
        match(tok);
    else
        syntax_error(tok, 1); 
}


void prim(void)
{
    token tok = next_token();
    switch(tok)
    {
        case LPAREN:
            match(LPAREN); expression();
            match(RPAREN);
        break;
        case ID:
            match(ID);
        break;
        case INTLATTERAL:
            match(INTLATTERAL);
        break;
        default:
            syntax_error(tok, 1);
            break;
    }
}

