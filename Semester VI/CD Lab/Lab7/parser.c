#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- PARSER CONTEXT ---------- */

typedef struct {
    FILE *fp;
    Token lookahead;
    int row;
    int col;
    char dbuf[16];
} Parser;

/* ---------- ERROR HANDLING ---------- */

void syntaxError(Parser *p, const char *expected)
{
    printf("\nSyntax Error at row %d, col %d\n", 
           p->lookahead.row, p->lookahead.col);

    printf("Expected: %s\n", expected);
    printf("Found   : %s\n", p->lookahead.token_name);

    exit(1);
}

/* ---------- TOKEN MATCH ---------- */

void match(Parser *p, const char *expected)
{
    if(strcmp(p->lookahead.token_name, expected) == 0)
    {
        p->lookahead = getNextToken(p->fp, &p->row, &p->col, p->dbuf);
    }
    else
    {
        syntaxError(p, expected);
    }
}

/* ---------- FORWARD DECLARATIONS ---------- */

void program(Parser *p);
void declarations(Parser *p);
void datatype(Parser *p);
void identifier_list(Parser *p);
void identifier_list_prime(Parser *p);
void assign_stat(Parser *p);
void assign_stat_prime(Parser *p);

/* ---------- GRAMMAR FUNCTIONS ---------- */

void program(Parser *p)
{
    match(p,"main");
    match(p,"(");
    match(p,")");
    match(p,"{");

    declarations(p);
    assign_stat(p);

    match(p,"}");
}

void declarations(Parser *p)
{
    if(strcmp(p->lookahead.token_name,"int")==0 ||
       strcmp(p->lookahead.token_name,"char")==0)
    {
        datatype(p);
        identifier_list(p);
        match(p,";");
        declarations(p);
    }
    /* epsilon */
}

void datatype(Parser *p)
{
    if(strcmp(p->lookahead.token_name,"int")==0)
        match(p,"int");
    else if(strcmp(p->lookahead.token_name,"char")==0)
        match(p,"char");
    else
        syntaxError(p,"int or char");
}

void identifier_list(Parser *p)
{
    if(strcmp(p->lookahead.token_name,"ID")==0)
    {
        match(p,"ID");
        identifier_list_prime(p);
    }
    else
        syntaxError(p,"identifier");
}

void identifier_list_prime(Parser *p)
{
    if(strcmp(p->lookahead.token_name,",")==0)
    {
        match(p,",");
        identifier_list(p);
    }
    /* epsilon */
}

void assign_stat(Parser *p)
{
    if(strcmp(p->lookahead.token_name,"ID")==0)
    {
        match(p,"ID");
        match(p,"=");
        assign_stat_prime(p);
    }
    else
        syntaxError(p,"identifier");
}

void assign_stat_prime(Parser *p)
{
    if(strcmp(p->lookahead.token_name,"ID")==0)
    {
        match(p,"ID");
        match(p,";");
    }
    else if(strcmp(p->lookahead.token_name,"NUM")==0)
    {
        match(p,"NUM");
        match(p,";");
    }
    else
        syntaxError(p,"identifier or number");
}

/* ---------- MAIN ---------- */

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 0;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("Error: Cannot open %s\n", argv[1]);
        return 0;
    }

    initSymTable();

    Parser parser;

    parser.fp = fp;
    parser.row = 1;
    parser.col = 0;
    parser.dbuf[0] = '\0';

    /* first token */
    parser.lookahead = getNextToken(fp, &parser.row, &parser.col, parser.dbuf);

    program(&parser);

    if(strcmp(parser.lookahead.token_name,"EOF") == 0)
        printf("\nParsing Successful\n");
    else
        syntaxError(&parser,"EOF");

    displaySymTable();

    fclose(fp);
    return 0;
}