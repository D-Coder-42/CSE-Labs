#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 128

typedef struct {
    char token_name[128];
    unsigned int row, col;
	int index;
	char type[32];
} Token;

typedef struct {
	char lexeme[128];
	char type[32];
	int size;
	char returntype[32];
} Symbol;

typedef struct list {
	Symbol sym;
	struct list* next;
} List;

typedef struct {
	int count;
	List* head;
} Bucket;

Bucket symTable[MAX_TOKENS];

const char *keywords[] = {
    "main", "if", "else", "int", "char", "return"
};
const int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

void initSymTable() {
	for (int i = 0; i < MAX_TOKENS; i++) {
		symTable[i].count = 0;
		symTable[i].head = NULL;
	}
}

int hash(char *str) {
	int len = strlen(str);
	int val = 0;
	for (int i = 0; i < len; i++) {
		val = (val + str[i]*31) % MAX_TOKENS;
	}
	return val;
}

int search(char *str) {
	int val = hash(str);
	if (symTable[val].count == 0) return -1;

	List *node = symTable[val].head;
	while (node != NULL) {
		if (strcmp(node->sym.lexeme, str) == 0) return val;
		node = node->next;
	}
	return -1;
}

void insert(char *lex, char *type, int size, char *rettype) {
	Symbol s;
	strcpy(s.lexeme, lex);
	strcpy(s.type, type);
	s.size = size;
	strcpy(s.returntype, rettype);

	int val = hash(s.lexeme);
	List *new = malloc(sizeof(List));
	new->sym = s;
	new->next = symTable[val].head;
	symTable[val].head = new;
	symTable[val].count++;
}

int isKeyword(const char *str) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

// Handles string literals including escape sequences
Token handleString(FILE *fp, int *row, int *col) {
    Token t; 
    t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0';
    strcpy(t.token_name, "STRING");
    
    int c = fgetc(fp); 
    (*col)++; 
    
    while (c != EOF && c != '"') {
        if (c == '\\') {
            c = fgetc(fp); (*col)++;
        }
        c = fgetc(fp); (*col)++;
    }
    return t;
}

// Does not handle decimal points since grammar only specifies integers
Token handleNumber(FILE *fp, char c, int *row, int *col) {
    Token t; 
	t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0';
    strcpy(t.token_name, "NUM");
    
    c = fgetc(fp);
    while (1) {
		if (isdigit(c)) {
			(*col)++;
			c = fgetc(fp);
		} else if (c == 'E' || c == 'e') {
			(*col)++;
			c = fgetc(fp);
			if (c == '+' || c == '-') {
				(*col)++;
				c = fgetc(fp);
			}
			while (isdigit(c)) {
				(*col)++;
				c = fgetc(fp);
			}
		} else {
			break;
		}
    }
    ungetc(c, fp); 
    return t;
}

int size(char* buf) {
	if (strcmp(buf,"int") == 0) return sizeof(int);
	if (strcmp(buf,"char") == 0) return sizeof(char);
	return 0;
}

// Handles identifiers and keywords
Token handleID(FILE *fp, char c, int *row, int *col, char* dbuf) {
    Token t; 
	t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0';
    char buffer[128];
    int i = 0;
    buffer[i++] = c;
    
    c = fgetc(fp);
    while (isalnum(c) || c == '_') { // assuming regex since not specified
        (*col)++;
        buffer[i++] = c;
        c = fgetc(fp);
    }
    buffer[i] = '\0';
    ungetc(c, fp);

    if (isKeyword(buffer)) {
        strcpy(t.token_name, buffer); 
        if (strcmp(buffer,"int") == 0 || strcmp(buffer,"char") == 0)
        	strcpy(dbuf, buffer);
    } else {
        strcpy(t.token_name, "ID");
        if (search(buffer) == -1) {
        	insert(buffer,dbuf,size(dbuf),"");
		}
		t.index = search(buffer);
    }
    return t;
}

// Handles operators including relational, assignment and logical operators
Token handleOperator(FILE *fp, char c, int *row, int *col) {
    Token t; 
	t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0';
    
    char next = fgetc(fp);
    (*col)++;

    int isPair = 0;
    if ((c == '&' && next == '&') || (c == '|' && next == '|') ||
        (c == '=' && next == '=') || (c == '!' && next == '=') ||
        (c == '<' && next == '=') || (c == '>' && next == '=')) {
        isPair = 1;
    }

    if (isPair) {
        t.token_name[0] = c; 
        t.token_name[1] = next; 
        t.token_name[2] = '\0';
    } else if (c != '&' && c != '|') { // single char operators '&' and '|' are not defined in grammar
        t.token_name[0] = c; 
        t.token_name[1] = '\0';
        ungetc(next, fp); (*col)--;
    } else {
		sprintf(t.token_name, "UNK"); // primitive error handling
		ungetc(next, fp); (*col)--;
	}
    return t;
}

// Handles single character special symbols
Token handleSymbol(FILE *fp, char c, int *row, int *col) {
	Token t; 
	t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0';

	if (strchr(";,(){}[]", c)) { // single character symbols defined in grammar
		t.token_name[0] = c; 
		t.token_name[1] = '\0';
		return t;
	} else {
		sprintf(t.token_name, "UNK"); // primitive error handling
		return t;
	}
}

// Main tokenization function
Token getNextToken(FILE *fp, int *row, int *col, char* dbuf) {
    Token t;
    int c;
    static int foundMain = 0;

    while ((c = fgetc(fp)) != EOF) {
        (*col)++;

        if (c == '\n') {
            (*row)++; *col = 0;
            printf("\n"); // to preserve line structure in output
            continue;
        }
        if (isspace(c)) continue;

        if (c == '#') {
            if (foundMain) {
                return handleSymbol(fp, c, row, col);
            }

            long currentPos = ftell(fp);
            int next;
            while (isspace(next = fgetc(fp)) && next != '\n');
            
            char buf[32];
            int i = 0;
            while (isalpha(next) && i < 31) {
                buf[i++] = next;
                next = fgetc(fp);
            }
            buf[i] = '\0';

            if (strcmp(buf, "include") == 0 || strcmp(buf, "define") == 0) {
                while (next != '\n' && next != EOF) next = fgetc(fp);
                ungetc(next, fp);
                continue;
            } else {
                fseek(fp, currentPos, SEEK_SET);
                return handleSymbol(fp, c, row, col);
            }
        }

        if (c == '/') {
            int next = fgetc(fp);
            (*col)++;
            
            if (next == '/') { // single line comment
                while (c != '\n' && c != EOF) {
                    c = fgetc(fp);
                }
                ungetc(c, fp); 
                continue;
            }
            else if (next == '*') { // multi-line comment
                int prev = 0;
                c = fgetc(fp);
                while (c != EOF) {
                    if (c == '\n') { 
						(*row)++; *col = 0;
						printf("\n");

					}
                    if (prev == '*' && c == '/') break;
                    prev = c;
                    c = fgetc(fp);
                }
                continue;
            }
            else { // division operator
                ungetc(next, fp); (*col)--;
                return handleOperator(fp, c, row, col);
            }
        }

        if (c == '"') {
            return handleString(fp, row, col);
        }
        else if (isalpha(c) || c == '_') {
            t = handleID(fp, c, row, col, dbuf);
            if (strcmp(t.token_name, "main") == 0) {
                foundMain = 1;
            }
            return t;
        }
        else if (isdigit(c)) {
            return handleNumber(fp, c, row, col);
        }
        else if (strchr("+-*/%=!<>&|", c)) {
            return handleOperator(fp, c, row, col);
        } else {
        	if (c == ';') dbuf[0] = '\0';
			return handleSymbol(fp, c, row, col);
		}
    }

    strcpy(t.token_name, "EOF");
    t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0';
    return t;
}

void displaySymTable() {
	printf("\n\nSymbol Table:\n");
	printf("\n\n%-6s %-25s %-10s %-6s %-12s\n", "Index", "Lexeme", "Type", "Size", "ReturnType");
	for (int i = 0; i < MAX_TOKENS; i++) {
		if (symTable[i].count > 0) {
			List *node = symTable[i].head;
			while (node != NULL) {
				printf("%-6d %-25s %-10s %-6d %-12s\n", i, node->sym.lexeme, node->sym.type, node->sym.size, node->sym.returntype);
			    node = node->next;
			}
        }
    }
    printf("\n");
}


int main() {
    FILE *fp = fopen("input.c", "r");
    if (!fp) {
        printf("Error: Cannot open input.c\n");
        return 0;
    }

    Token tk;
    int row = 1, col = 0;
    char dbuf[16] = "";

    while (1) {
        tk = getNextToken(fp, &row, &col, dbuf);
        if (strcmp(tk.token_name, "EOF") == 0) break;

        printf("<%s,%d,%d,%d>", tk.token_name, tk.row, tk.col, tk.index);
    }

    displaySymTable();

    fclose(fp);
    return 0;
}