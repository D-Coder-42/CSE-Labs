#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS     128
#define MAX_TOKEN_LEN  128

// --- 1. Preprocessor & Comments ---
const char *PREPROCESSOR_START         = "#";
const char *SINGLE_LINE_COMMENT_MARKER = "//";
const char *MULTI_LINE_COMMENT_START   = "/*";
const char *MULTI_LINE_COMMENT_END     = "*/";

// --- 2. Identifiers ---
// Standard IDs start with [A-Za-z_]. Add extra allowed start chars here.
const char *EXTRA_ID_START_CHARS       = ""; 

// --- 3. Strings ---
// Characters that start a string literal.
const char *STRING_DELIMITERS          = "\""; 

// --- 4. Keywords ---
const char *keywords[] = {
    "main", "if", "else", "while", "return", "void", "int", "char",
    /* START OPTIONAL: C-Specific Types */
    "float", "struct"
    /* END OPTIONAL */
};

// --- 5. Data Types (for size calculation) ---
const char *datatypes[] = { 
    "int", "char", 
    /* START OPTIONAL: C-Specific Types */
    "float", "struct" 
    /* END OPTIONAL */
};

// --- 6. Operators & Special Symbols ---
const char *operator_starts = "+-*/%=!<>&|"; 
const char *special_symbols = ";,(){}[]"; 
const char *multi_char_symbols[] = { NULL };

typedef struct {
    char token_name[MAX_TOKEN_LEN];
    unsigned int row, col;
    int index;
    char type[32];
    /* START PARAMS: Remove if function parameters are not needed */
    char params[MAX_TOKEN_LEN];
    /* END PARAMS */
} Token;

typedef struct {
    char lexeme[MAX_TOKEN_LEN];
    char type[32];
    int size;
    char returntype[32];
    /* START PARAMS: Remove if function parameters are not needed */
    char params[MAX_TOKEN_LEN];
    /* END PARAMS */
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
const int keyword_count = sizeof(keywords) / sizeof(keywords[0]);
const int datatype_count = sizeof(datatypes) / sizeof(datatypes[0]);

// --- Utility Functions ---

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

Symbol* getSymbol(char *str) {
    int val = hash(str);
    if (symTable[val].count == 0) return NULL;
    List *node = symTable[val].head;
    while (node != NULL) {
        if (strcmp(node->sym.lexeme, str) == 0) return &node->sym;
        node = node->next;
    }
    return NULL;
}

void insert(char *lex, char *type, int size, char *rettype, char *params) {
    if (search(lex) != -1) return;
    Symbol s;
    strcpy(s.lexeme, lex); strcpy(s.type, type); s.size = size; strcpy(s.returntype, rettype);
    if (params) strcpy(s.params, params); else s.params[0] = '\0';

    int val = hash(s.lexeme);
    List *new = malloc(sizeof(List));
    new->sym = s; new->next = symTable[val].head;
    symTable[val].head = new; symTable[val].count++;
}

int isKeyword(const char *str) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

int isDataType(const char *str) {
    for (int i = 0; i < datatype_count; i++) {
        if (strcmp(str, datatypes[i]) == 0) return 1;
    }
    return 0;
}

int size(char* buf) {
    if (strcmp(buf,"int") == 0) return sizeof(int);
    if (strcmp(buf,"char") == 0) return sizeof(char);
    
    /* START C_SPECIFIC: Float support */
    if (strcmp(buf,"float") == 0) return sizeof(float);
    /* END C_SPECIFIC */
    
    /* START C_SPECIFIC: Struct size lookup */
    Symbol* s = getSymbol(buf);
    if (s != NULL) return s->size;
    /* END C_SPECIFIC */
    
    return 0;
}

// --- Handler Functions ---

// Helper: Checks marker without consuming if false
int checkMarker(FILE *fp, const char *marker) {
    int len = strlen(marker);
    char buf[128]; 
    int i;
    for(i = 0; i < len; i++) {
        int c = fgetc(fp);
        if(c == EOF) { while(i > 0) ungetc(buf[--i], fp); return 0; }
        buf[i] = c;
        if(buf[i] != marker[i]) {
            ungetc(c, fp);
            while(i > 0) ungetc(buf[--i], fp);
            return 0;
        }
    }
    return 1; 
}

/* START C_SPECIFIC: Struct Size Calculation */
int precalculateStructSize(FILE *fp) {
    long originalPos = ftell(fp);
    int totalSize = 0;
    char buffer[128];
    int c;

    while ((c = fgetc(fp)) != EOF && c != '{'); 
    if (c != '{') { fseek(fp, originalPos, SEEK_SET); return 0; }

    while ((c = fgetc(fp)) != EOF && c != '}') {
        if (isspace(c) || c == ';') continue;
        if (isalpha(c)) {
            int i = 0; buffer[i++] = c;
            while (isalnum(c = fgetc(fp)) || c == '_') buffer[i++] = c;
            buffer[i] = '\0';
            int baseSize = size(buffer); 
            if (baseSize > 0) {
                while (1) {
                    while (isspace(c)) c = fgetc(fp);
                    while (isalnum(c) || c == '_') c = fgetc(fp); 
                    while (isspace(c)) c = fgetc(fp);
                    if (c == '[') {
                        char numBuf[16]; int k = 0; c = fgetc(fp);
                        while (isdigit(c)) { numBuf[k++] = c; c = fgetc(fp); }
                        numBuf[k] = '\0';
                        totalSize += (baseSize * atoi(numBuf));
                        while (c != ']' && c != EOF) c = fgetc(fp); c = fgetc(fp); 
                    } else { totalSize += baseSize; }
                    while (isspace(c)) c = fgetc(fp);
                    if (c == ',') { c = fgetc(fp); continue; } 
                    if (c == ';') break; 
                    if (c == '}') { ungetc(c, fp); break; }
                    if (c == EOF) break; 
                }
            }
        }
    }
    fseek(fp, originalPos, SEEK_SET); 
    return totalSize;
}
/* END C_SPECIFIC */

Token handleString(FILE *fp, int *row, int *col, char quoteType) {
    Token t; t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0'; t.params[0] = '\0';
    strcpy(t.token_name, "STRING_LITERAL");
    int c = fgetc(fp); (*col)++; 
    while (c != EOF && c != quoteType) {
        if (c == '\\') { c = fgetc(fp); (*col)++; } // Handle escape
        c = fgetc(fp); (*col)++;
    }
    return t;
}

Token handleNumber(FILE *fp, char c, int *row, int *col) {
    Token t; t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0'; t.params[0] = '\0';
    strcpy(t.token_name, "NUM");
    c = fgetc(fp);
    /* START C_SPECIFIC: Float support logic */
    int has_dot = 0;
    while (1) {
        if (isdigit(c)) { (*col)++; c = fgetc(fp); }
        else if (c == '.' && !has_dot) { has_dot = 1; (*col)++; c = fgetc(fp); } 
        else if (c == 'E' || c == 'e') {
            (*col)++; c = fgetc(fp);
            if (c == '+' || c == '-') { (*col)++; c = fgetc(fp); }
            while (isdigit(c)) { (*col)++; c = fgetc(fp); }
        } else { break; }
    }
    /* END C_SPECIFIC */
    ungetc(c, fp); 
    return t;
}

Token handleIdentifier(char *buffer, FILE *fp, int *row, int *col, char* dbuf) {
    Token t; 
    t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0'; t.params[0] = '\0';
    int c;
    strcpy(t.token_name, "ID");
    
    if (search(buffer) == -1) {
        /* START C_SPECIFIC: Struct Handling */
        if (strcmp(dbuf, "struct") == 0) { 
            int sSize = 0;
            c = fgetc(fp); 
            // Accurately consume whitespace
            while(isspace(c)) { if(c=='\n'){(*row)++; *col=0;} else (*col)++; c = fgetc(fp); }
            
            if (c == '{') { ungetc(c, fp); sSize = precalculateStructSize(fp); insert(buffer, "struct", sSize, "", ""); }
            else { ungetc(c, fp); sSize = size(buffer); }
            strcpy(dbuf, buffer); 
        } 
        /* END C_SPECIFIC */
        else {
            c = fgetc(fp); 
            // Accurately consume whitespace
            while (isspace(c)) { if(c=='\n'){(*row)++; *col=0;} else (*col)++; c = fgetc(fp); }

            /* START PARAM_LOGIC: Function Params */
            if (c == '(') {
                long pos = ftell(fp); char params_buf[128] = ""; int p_idx = 0; int p_c = fgetc(fp);
                while (p_c != ')' && p_c != EOF && p_idx < 127) { params_buf[p_idx++] = (char)p_c; p_c = fgetc(fp); }
                params_buf[p_idx] = '\0'; fseek(fp, pos, SEEK_SET); 
                insert(buffer, "func", 0, dbuf, params_buf); strcpy(t.params, params_buf); dbuf[0] = '\0';
            }
            /* END PARAM_LOGIC */
            else if (c == '[') {
                char temp[16]; int k = 0;
                while (1) { c = fgetc(fp); if (isdigit(c)) temp[k++] = c; else if (c == ']') break; }
                temp[k] = '\0'; int arrSize = atoi(temp) * size(dbuf);
                insert(buffer, dbuf, arrSize, "", "");
            } else { insert(buffer, dbuf, size(dbuf), "", ""); }
            ungetc(c, fp);
        }
    }
    t.index = search(buffer);
    return t;
}

Token handleOperator(FILE *fp, char c, int *row, int *col) {
    Token t; t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0'; t.params[0] = '\0';
    char next = fgetc(fp); (*col)++;
    
    // Generalized Pair Check (C/Java/Perl/JS all mostly use these)
    int isPair = 0;
    if ((c == '&' && next == '&') || (c == '|' && next == '|') || (c == '=' && next == '=') || 
        (c == '!' && next == '=') || (c == '<' && next == '=') || (c == '>' && next == '=') || 
        (c == '+' && next == '=') || (c == '-' && next == '=') || (c == '*' && next == '=') || 
        (c == '/' && next == '=') || (c == '%' && next == '=') || 
        (c == '+' && next == '+') || (c == '-' && next == '-')) isPair = 1;

    if (isPair) { t.token_name[0] = c; t.token_name[1] = next; t.token_name[2] = '\0'; }
    else if (c != '&' && c != '|') { 
        t.token_name[0] = c; t.token_name[1] = '\0'; ungetc(next, fp); (*col)--; 
    } else { sprintf(t.token_name, "UNK"); ungetc(next, fp); (*col)--; }
    return t;
}

Token handleSymbol(FILE *fp, char c, int *row, int *col) {
    Token t; t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0'; t.params[0] = '\0';
    
    char next = fgetc(fp); (*col)++;
    char potential[3] = {c, next, '\0'};
    
    int isPair = 0;
    for (int i = 0; multi_char_symbols[i] != NULL; i++) {
        if (strcmp(potential, multi_char_symbols[i]) == 0) {
            isPair = 1;
            break;
        }
    }

    if (isPair) {
        strcpy(t.token_name, potential);
    } else {
        ungetc(next, fp); (*col)--;
        if (strchr(special_symbols, c)) { 
            t.token_name[0] = c; t.token_name[1] = '\0'; 
        } else {
            sprintf(t.token_name, "UNK");
        }
    }
    return t;
}

// Main Tokenizer Loop
Token getNextToken(FILE *fp, int *row, int *col, char* dbuf) {
    Token t; int c; static int foundMain = 0;
    while (1) {
        c = fgetc(fp);
        if (c == EOF) break;
        (*col)++;

        if (c == '\n') { (*row)++; *col = 0; continue; }
        if (isspace(c)) continue;

        // 1. Preprocessor Check (Config Driven)
        if (c == PREPROCESSOR_START[0]) {
            ungetc(c, fp); 
            if (checkMarker(fp, PREPROCESSOR_START)) {
                (*col) += strlen(PREPROCESSOR_START) - 1;
                
                // C-style special handling for #include/#define keywords
                if (foundMain && strcmp(PREPROCESSOR_START, "#") == 0) return handleSymbol(fp, PREPROCESSOR_START[0], row, col);
                
                long currentPos = ftell(fp); int next;
                while (isspace(next = fgetc(fp)) && next != '\n');
                char buf[32]; int i = 0;
                while (isalpha(next) && i < 31) { buf[i++] = next; next = fgetc(fp); } buf[i] = '\0';
                
                // If it is #include or #define, we must skip the line
                if (strcmp(buf, "include") == 0 || strcmp(buf, "define") == 0) {
                    while (next != '\n' && next != EOF) next = fgetc(fp); 
                    ungetc(next, fp); // Put back newline so next loop hits row++
                    continue;
                } else { 
                    fseek(fp, currentPos, SEEK_SET); 
                    return handleSymbol(fp, PREPROCESSOR_START[0], row, col); 
                }
            }
            c = fgetc(fp); 
        }

        // 2. Comments Check (Config Driven)
        if (c == SINGLE_LINE_COMMENT_MARKER[0]) {
             ungetc(c, fp); 
             if (checkMarker(fp, SINGLE_LINE_COMMENT_MARKER)) {
                 (*col) += strlen(SINGLE_LINE_COMMENT_MARKER) - 1;
                 while (c != '\n' && c != EOF) c = fgetc(fp);
                 ungetc(c, fp); continue; 
             }
             c = fgetc(fp);
        }
        if (c == MULTI_LINE_COMMENT_START[0]) {
             ungetc(c, fp);
             if (checkMarker(fp, MULTI_LINE_COMMENT_START)) {
                 (*col) += strlen(MULTI_LINE_COMMENT_START) - 1;
                 while ((c = fgetc(fp)) != EOF) {
                     if (c == '\n') { (*row)++; *col = 0; }
                     if (c == MULTI_LINE_COMMENT_END[0]) {
                         ungetc(c, fp);
                         if (checkMarker(fp, MULTI_LINE_COMMENT_END)) break; 
                         c = fgetc(fp);
                     }
                 }
                 continue;
             }
             c = fgetc(fp);
        }

        // 3. Strings (Config Driven)
        if (strchr(STRING_DELIMITERS, c)) return handleString(fp, row, col, c);

        // 4. Identifiers (Generalized)
        // Check standard alphas OR extra chars (like $ for Perl, . for CSS)
        else if (isalpha(c) || c == '_' || strchr(EXTRA_ID_START_CHARS, c)) {
            char buffer[128]; int i = 0; buffer[i++] = c;
            c = fgetc(fp);
            // Read standard ID body characters
            while (isalnum(c) || c == '_') { (*col)++; buffer[i++] = c; c = fgetc(fp); }
            buffer[i] = '\0';
            ungetc(c, fp);

            if (isKeyword(buffer)) {
                strcpy(t.token_name, buffer);
                if (isDataType(buffer)) strcpy(dbuf, buffer);
                t.row = *row; t.col = *col - (strlen(buffer) - 1); t.index = -1; t.type[0]='\0'; t.params[0]='\0';
                return t;
            } else {
                int startCol = *col - (strlen(buffer) - 1);
                t = handleIdentifier(buffer, fp, row, col, dbuf);
                t.row = *row; t.col = startCol; 
                if (strcmp(t.token_name, "main") == 0) foundMain = 1;
                return t;
            }
        }
        else if (isdigit(c)) return handleNumber(fp, c, row, col);
        else if (strchr(operator_starts, c)) return handleOperator(fp, c, row, col);
        else { if (c == ';') dbuf[0] = '\0'; return handleSymbol(fp, c, row, col); }
    }
    strcpy(t.token_name, "EOF"); t.row = *row; t.col = *col; t.index = -1; t.type[0] = '\0'; t.params[0] = '\0';
    return t;
}

void displaySymTable() {
    printf("\n\nSymbol Table:\n");
    printf("\n%-6s %-20s %-10s %-6s %-12s %-20s\n", "Index", "Lexeme", "Type", "Size", "ReturnType", "Parameters");
    for (int i = 0; i < MAX_TOKENS; i++) {
        if (symTable[i].count > 0) {
            List *node = symTable[i].head;
            while (node != NULL) {
                printf("%-6d %-20s %-10s %-6d %-12s %-20s\n", i, node->sym.lexeme, node->sym.type, node->sym.size, node->sym.returntype, node->sym.params);
                node = node->next;
            }
        }
    }
    printf("\n");
}