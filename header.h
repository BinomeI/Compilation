#pragma once
#include <stdio.h>
#define MAX 26
typedef enum tokens_types {
    BEGIN,
    END,
    READ,
    WRITE,
    ID,
    INTLATTERAL,
    FLOAT,
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
    NO_ERROR,
    ASSIGNOP_ERROR, 
    UNKOWN_CHARACTER, 
    PRIM_ERROR, 
    INST_ERROR, 
    ADDOPERROR, 
    MATCH_ERROR, 
    REEL_ERROR, 
    FATAL_ERROR
}Errors; 

char mots_cle[][MAX] = {
    "BEGIN",
    "END",
    "READ",
    "WRITE",
    "ID",
    "INTLATTERAL",
    "FLOAT",
    "LPAREN",
    "RPAREN",
    "SEMICOLON",
    "COMMA",
    "ASSIGNOP",
    "PLUSOP",
    "MINUSOP",
    "SCANEOF"
};

Errors LexicaleError = NO_ERROR; 


int LineNumbers = 1; 
char token_buffer[MAX];

char current_token[MAX];
char expected_token[MAX];
char RESERVE = '1'; 
FILE* fil;

unsigned short int BeginCounter = 0; 

