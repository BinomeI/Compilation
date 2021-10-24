#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void syntax_error(token tok){
    printf("\nKey Word: %s expected instead of: %s\n", mots_cle[tok], current_token); 
}

token next_token(FILE *fptr) {
    clear_current(); 
    fscanf(fptr, "%s", current_token); 
    for (int i = 0; i < MAX; i++)
        if(strcmp(current_token, mots_cle[i]) == 0)
            return i;    
    
    printf("\nWeirred error occured\n");
    
}

void clear_current()
{
    for (int i = 0; i < MAX; i++)
    {
        current_token[i] = '\0';
    } 
}


void match(token t)
{
    if(strcmp(mots_cle[t],current_token))
        syntax_error(t);
}

void inst(void)
{
    token tok = next_token(); 
    switch(tok) {
        case ID: 
            match(ID); match(ASSIGNOP); 
            expression(); match(SEMICOLON); 
            break; 
        
        case READ: 
            match(READ); match(LPAREN); 
            id_list(); match(RPAREN); match(SEMICOLON); 
            break; 
        
        case WRITE: 
            match(WRITE); match(LPAREN); 
            expr_list(); match(RPAREN); match(SEMICOLON); 
            break; 
        
        default: 
            syntax_error(tok); 
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
    match(ID);
    while(next_token() == COMMA)
    {
        match(COMMA);
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
        syntax_error(tok); 
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
            syntax_error(tok);
            break;
    }
}