
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
            syntax_error(); 
            break; 
    }
}

void inst_list(void)
{
    // <inst_list> ::= <inst> {<inst>}
    inst(); 
    while(true){
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
