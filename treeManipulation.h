#include <stdio.h>
#include "header.h"


typedef struct Nd{
    union treeNode
    {
        char *non_terminal; 
        token terminal; 
    };
    struct Nd *fils; 
    struct Nd *frere; 
    
}Node; 

Node *createNode(){
    
}