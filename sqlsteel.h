/*
 * SQLSteel Header File
 * Ultra-Lightweight In-Memory Database for UNIVAC 1219
 */

#ifndef SQLSTEEL_H
#define SQLSTEEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* Memory constraints for UNIVAC (40kB total) */
#define MAX_RECORDS 100
#define MAX_NAME_LEN 24
#define MAX_CITY_LEN 20
#define MAX_OCCUPATION_LEN 18
#define MAX_NOTES_LEN 30
#define MAX_SQL_LEN 256
#define MAX_TOKENS 20
#define MAX_TOKEN_LEN 32

/* Table structure - Suspected Soviet Sympathies Database */
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char city[MAX_CITY_LEN];
    char occupation[MAX_OCCUPATION_LEN];
    int age;
    int suspicion_level;      /* 1-10 scale */
    int contacts_monitored;
    int reports_filed;
    char notes[MAX_NOTES_LEN];
    int active;               /* 1=active, 0=deleted */
} SuspectRecord;

/* SQL Token Types */
typedef enum {
    TOKEN_SELECT,
    TOKEN_INSERT,
    TOKEN_DELETE,
    TOKEN_UPDATE,
    TOKEN_WHERE,
    TOKEN_FROM,
    TOKEN_INTO,
    TOKEN_VALUES,
    TOKEN_SET,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_ASTERISK,
    TOKEN_EQUALS,
    TOKEN_GT,
    TOKEN_LT,
    TOKEN_GTE,
    TOKEN_LTE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

/* Token structure */
typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LEN];
} Token;

/* SQL Parser state */
typedef struct {
    Token tokens[MAX_TOKENS];
    int token_count;
    int current_token;
} Parser;

/* Function prototypes - Database operations */
void init_database(void);
void load_demo_data(void);
int get_active_count(void);
void generate_random_name(char *dest, int max_len);

/* Function prototypes - SQL Engine */
void execute_sql(const char *sql);
void tokenize_sql(const char *sql, Parser *parser);
void parse_and_execute(Parser *parser);
void execute_select(Parser *parser);
void execute_insert(Parser *parser);
void execute_delete(Parser *parser);
void execute_update(Parser *parser);

/* Function prototypes - Query evaluation */
int evaluate_where_clause(Parser *parser, SuspectRecord *rec, int *pos);
int evaluate_condition(const char *field, const char *op, const char *value, SuspectRecord *rec);
int get_field_value(const char *field, SuspectRecord *rec);
void get_field_string(const char *field, SuspectRecord *rec, char *dest, int max_len);

/* Function prototypes - Display */
void print_header(void);
void print_record(SuspectRecord *rec);
void print_separator(void);
void print_full_record(SuspectRecord *rec);

/* Function prototypes - Utilities */
void to_upper(char *str);
int is_keyword(const char *word);
void skip_whitespace(const char **p);
const char* get_next_token(const char *sql, char *token, int max_len);

/* Global database */
extern SuspectRecord g_database[MAX_RECORDS];
extern int g_record_count;

#endif /* SQLSTEEL_H */
