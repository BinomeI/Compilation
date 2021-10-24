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
    ASSIGNOP_ERROR, 
    UNKOWN_CHARACTER, 
    REEL_ERROR, 
    NO_ERROR
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

int LineNumbers = 1; 
char token_buffer[MAX];

char current_token[MAX];
char expected_token[MAX];
FILE* fil;

