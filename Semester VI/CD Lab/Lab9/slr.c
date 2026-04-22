#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR   -1
#define SHIFT   1
#define REDUCE  2
#define ACCEPT  3

#define MAX_STACK 100
#define MAX_INPUT 100

typedef struct {
    int action;
    int state;
} Entry;

Entry Action[12][6] = {
    {{ERROR,-1}, {ERROR,-1}, {SHIFT, 5}, {ERROR,-1}, {SHIFT, 4}, {ERROR,-1}},
    {{SHIFT, 6}, {ERROR,-1}, {ERROR,-1}, {ERROR,-1}, {ERROR,-1}, {ACCEPT,0}},
    {{REDUCE,2}, {SHIFT,10}, {ERROR,-1}, {REDUCE,2}, {ERROR,-1}, {REDUCE,2}},
    {{REDUCE,4}, {REDUCE,4}, {ERROR,-1}, {REDUCE,4}, {ERROR,-1}, {REDUCE,4}},
    {{REDUCE,6}, {REDUCE,6}, {ERROR,-1}, {REDUCE,6}, {ERROR,-1}, {REDUCE,6}},
    {{ERROR,-1}, {ERROR,-1}, {SHIFT, 5}, {ERROR,-1}, {SHIFT, 4}, {ERROR,-1}},
    {{ERROR,-1}, {ERROR,-1}, {SHIFT, 5}, {ERROR,-1}, {SHIFT, 4}, {ERROR,-1}},
    {{SHIFT, 6}, {ERROR,-1}, {ERROR,-1}, {SHIFT, 9}, {ERROR,-1}, {ERROR,-1}},
    {{REDUCE,1}, {SHIFT,10}, {ERROR,-1}, {REDUCE,1}, {ERROR,-1}, {REDUCE,1}},
    {{REDUCE,5}, {REDUCE,5}, {ERROR,-1}, {REDUCE,5}, {ERROR,-1}, {REDUCE,5}},
    {{ERROR,-1}, {ERROR,-1}, {SHIFT, 5}, {ERROR,-1}, {SHIFT, 4}, {ERROR,-1}},
    {{REDUCE,3}, {REDUCE,3}, {ERROR,-1}, {REDUCE,3}, {ERROR,-1}, {REDUCE,3}}
};

int GoTo[12][3] = {
     { 1, 2, 3},
     {-1,-1,-1},
     {-1,-1,-1},
     {-1,-1,-1},
     {-1,-1,-1},
     { 7, 2, 3},
     {-1, 8, 3},
     {-1,-1,-1},
     {-1,-1,-1},
     {-1,-1,-1},
     {-1,-1,11},
     {-1,-1,-1}
};

int rule_len[] = {0, 3, 1, 3, 1, 3, 1};
char rule_lhs[] = {' ', 'E', 'E', 'T', 'T', 'F', 'F'};

char raw_input[MAX_INPUT];
int raw_ip = 0;

int get_action_col(char c) {
    switch(c) {
        case '+': return 0;
        case '*': return 1;
        case '(': return 2;
        case ')': return 3;
        case 'i': return 4; 
        case '$': return 5;
        default:  return -1;
    }
}

int get_goto_col(char c) {
    switch(c) {
        case 'E': return 0;
        case 'T': return 1;
        case 'F': return 2;
        default:  return -1;
    }
}

char getNextToken() {
    char c = raw_input[raw_ip];
    if (c == '\0' || c == '\n') {
        return '$';
    }
    
    if (c == 'i' && raw_input[raw_ip + 1] == 'd') {
        raw_ip += 2;
        return 'i'; 
    }
    
    raw_ip++;
    return c;
}

void parse() {
    int stack[MAX_STACK];
    int top = 0;
    stack[top] = 0;

    int step = 1;
    char lookahead = getNextToken();

    while (1) {
        int state = stack[top];
        int col = get_action_col(lookahead);

        if (col == -1) {
            printf("\nLexical Error: Invalid character in input.\n");
            return;
        }

        Entry current = Action[state][col];
        char stack_str[MAX_STACK * 4] = "";
        int offset = 0;
        for (int i = 0; i <= top; i++) {
            offset += sprintf(stack_str + offset, "%d ", stack[i]);
        }
        
        char token_str[3];
        if (lookahead == 'i') strcpy(token_str, "id");
        else { token_str[0] = lookahead; token_str[1] = '\0'; }

        if (current.action == SHIFT) {
            // printf("Shift %d\n", current.state);
            stack[++top] = current.state;
            lookahead = getNextToken(); 
        } 
        else if (current.action == REDUCE) {
            int rule_idx = current.state;
            top -= rule_len[rule_idx];
            
            int current_state = stack[top];
            int nt_col = get_goto_col(rule_lhs[rule_idx]);
            stack[++top] = GoTo[current_state][nt_col];
        } 
        else if (current.action == ACCEPT) {
            printf("Parsing Successful!\n");
            break;
        } 
        else { 
            printf("Error\n\nSyntax error at token '%s'.\n", token_str);
            break;
        }
    }
}

int main(int argc, char const *argv[]) {
    printf("Enter the input string: ");
    scanf("%99s", raw_input); 
    
	parse();
    return 0;
}