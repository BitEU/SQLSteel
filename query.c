/*
 * Query Executor Module
 * Handles SELECT, INSERT, DELETE, UPDATE operations
 */

#include "sqlsteel.h"

/* Get integer field value from record */
int get_field_value(const char *field, SuspectRecord *rec) {
    if (strcmp(field, "ID") == 0) return rec->id;
    if (strcmp(field, "AGE") == 0) return rec->age;
    if (strcmp(field, "SUSPICION_LEVEL") == 0) return rec->suspicion_level;
    if (strcmp(field, "CONTACTS_MONITORED") == 0) return rec->contacts_monitored;
    if (strcmp(field, "REPORTS_FILED") == 0) return rec->reports_filed;
    return 0;
}

/* Get string field value from record */
void get_field_string(const char *field, SuspectRecord *rec, char *dest, int max_len) {
    if (strcmp(field, "NAME") == 0) {
        strncpy(dest, rec->name, max_len);
    } else if (strcmp(field, "CITY") == 0) {
        strncpy(dest, rec->city, max_len);
    } else if (strcmp(field, "OCCUPATION") == 0) {
        strncpy(dest, rec->occupation, max_len);
    } else if (strcmp(field, "NOTES") == 0) {
        strncpy(dest, rec->notes, max_len);
    } else {
        dest[0] = '\0';
    }
    dest[max_len - 1] = '\0';
}

/* Evaluate a single condition */
int evaluate_condition(const char *field, const char *op, const char *value, SuspectRecord *rec) {
    int field_val, comp_val;
    char field_str[64];
    char value_upper[64];
    
    /* Try integer comparison first */
    field_val = get_field_value(field, rec);
    comp_val = atoi(value);
    
    /* Check if this is an integer field */
    if (strcmp(field, "ID") == 0 || strcmp(field, "AGE") == 0 || 
        strcmp(field, "SUSPICION_LEVEL") == 0 || strcmp(field, "CONTACTS_MONITORED") == 0 ||
        strcmp(field, "REPORTS_FILED") == 0) {
        
        if (strcmp(op, "=") == 0) return field_val == comp_val;
        if (strcmp(op, ">") == 0) return field_val > comp_val;
        if (strcmp(op, "<") == 0) return field_val < comp_val;
        if (strcmp(op, ">=") == 0) return field_val >= comp_val;
        if (strcmp(op, "<=") == 0) return field_val <= comp_val;
    }
    /* String comparison */
    else {
        get_field_string(field, rec, field_str, sizeof(field_str));
        strncpy(value_upper, value, sizeof(value_upper) - 1);
        value_upper[sizeof(value_upper) - 1] = '\0';
        to_upper(value_upper);
        
        if (strcmp(op, "=") == 0) {
            return strcmp(field_str, value_upper) == 0;
        }
    }
    
    return 0;
}

/* Evaluate WHERE clause */
int evaluate_where_clause(Parser *parser, SuspectRecord *rec, int *pos) {
    int result = 1;
    int i = *pos;
    
    /* Simple WHERE evaluation: field op value [AND field op value]* */
    while (i < parser->token_count) {
        if (parser->tokens[i].type == TOKEN_SEMICOLON) break;
        
        if (parser->tokens[i].type == TOKEN_IDENTIFIER) {
            char field[MAX_TOKEN_LEN];
            char op[4];
            char value[MAX_TOKEN_LEN];
            int cond_result;
            
            strncpy(field, parser->tokens[i].value, MAX_TOKEN_LEN - 1);
            field[MAX_TOKEN_LEN - 1] = '\0';
            i++;
            
            if (i >= parser->token_count) break;
            
            /* Get operator */
            if (parser->tokens[i].type == TOKEN_EQUALS) strcpy(op, "=");
            else if (parser->tokens[i].type == TOKEN_GT) strcpy(op, ">");
            else if (parser->tokens[i].type == TOKEN_LT) strcpy(op, "<");
            else if (parser->tokens[i].type == TOKEN_GTE) strcpy(op, ">=");
            else if (parser->tokens[i].type == TOKEN_LTE) strcpy(op, "<=");
            else break;
            i++;
            
            if (i >= parser->token_count) break;
            
            /* Get value */
            if (parser->tokens[i].type == TOKEN_NUMBER || 
                parser->tokens[i].type == TOKEN_STRING ||
                parser->tokens[i].type == TOKEN_IDENTIFIER) {
                strncpy(value, parser->tokens[i].value, MAX_TOKEN_LEN - 1);
                value[MAX_TOKEN_LEN - 1] = '\0';
                i++;
            } else break;
            
            /* Evaluate condition */
            cond_result = evaluate_condition(field, op, value, rec);
            result = result && cond_result;
            
            /* Check for AND */
            if (i < parser->token_count && parser->tokens[i].type == TOKEN_AND) {
                i++;
                continue;
            } else {
                break;
            }
        } else {
            i++;
        }
    }
    
    *pos = i;
    return result;
}

/* Execute SELECT statement */
void execute_select(Parser *parser) {
    int i, count = 0;
    int has_where = 0;
    int where_pos = 0;
    
    /* Find WHERE clause */
    for (i = 0; i < parser->token_count; i++) {
        if (parser->tokens[i].type == TOKEN_WHERE) {
            has_where = 1;
            where_pos = i + 1;
            break;
        }
    }
    
    print_header();
    
    for (i = 0; i < MAX_RECORDS; i++) {
        if (g_database[i].active) {
            int matches = 1;
            
            if (has_where) {
                int pos = where_pos;
                matches = evaluate_where_clause(parser, &g_database[i], &pos);
            }
            
            if (matches) {
                print_record(&g_database[i]);
                count++;
            }
        }
    }
    
    print_separator();
    printf("RECORDS RETURNED: %d\n", count);
}

/* Execute INSERT statement */
void execute_insert(Parser *parser) {
    int i, slot = -1;
    SuspectRecord new_rec;
    
    /* Find empty slot */
    for (i = 0; i < MAX_RECORDS; i++) {
        if (!g_database[i].active) {
            slot = i;
            break;
        }
    }
    
    if (slot == -1) {
        printf("ERROR: DATABASE FULL (MAX %d RECORDS)\n", MAX_RECORDS);
        return;
    }
    
    /* Parse INSERT INTO suspects VALUES (...) */
    /* Simplified: expects 8 values in order */
    int value_start = 0;
    for (i = 0; i < parser->token_count; i++) {
        if (parser->tokens[i].type == TOKEN_VALUES) {
            value_start = i + 1;
            break;
        }
    }
    
    if (value_start == 0) {
        printf("ERROR: INVALID INSERT SYNTAX\n");
        printf("USAGE: INSERT INTO SUSPECTS VALUES ('NAME', 'CITY', 'OCCUPATION', AGE, SUSPICION, CONTACTS, REPORTS, 'NOTES')\n");
        return;
    }
    
    /* Skip opening paren */
    if (parser->tokens[value_start].type == TOKEN_LPAREN) value_start++;
    
    /* Parse values */
    int val_idx = value_start;
    int field_num = 0;
    
    new_rec.active = 1;
    new_rec.id = slot + 1;
    
    while (val_idx < parser->token_count && field_num < 8) {
        if (parser->tokens[val_idx].type == TOKEN_COMMA) {
            val_idx++;
            continue;
        }
        if (parser->tokens[val_idx].type == TOKEN_RPAREN) break;
        
        switch (field_num) {
            case 0: /* NAME */
                strncpy(new_rec.name, parser->tokens[val_idx].value, MAX_NAME_LEN - 1);
                new_rec.name[MAX_NAME_LEN - 1] = '\0';
                break;
            case 1: /* CITY */
                strncpy(new_rec.city, parser->tokens[val_idx].value, MAX_CITY_LEN - 1);
                new_rec.city[MAX_CITY_LEN - 1] = '\0';
                break;
            case 2: /* OCCUPATION */
                strncpy(new_rec.occupation, parser->tokens[val_idx].value, MAX_OCCUPATION_LEN - 1);
                new_rec.occupation[MAX_OCCUPATION_LEN - 1] = '\0';
                break;
            case 3: /* AGE */
                new_rec.age = atoi(parser->tokens[val_idx].value);
                break;
            case 4: /* SUSPICION_LEVEL */
                new_rec.suspicion_level = atoi(parser->tokens[val_idx].value);
                break;
            case 5: /* CONTACTS_MONITORED */
                new_rec.contacts_monitored = atoi(parser->tokens[val_idx].value);
                break;
            case 6: /* REPORTS_FILED */
                new_rec.reports_filed = atoi(parser->tokens[val_idx].value);
                break;
            case 7: /* NOTES */
                strncpy(new_rec.notes, parser->tokens[val_idx].value, MAX_NOTES_LEN - 1);
                new_rec.notes[MAX_NOTES_LEN - 1] = '\0';
                break;
        }
        
        field_num++;
        val_idx++;
    }
    
    if (field_num == 8) {
        g_database[slot] = new_rec;
        g_record_count++;
        printf("RECORD INSERTED: ID=%d (NAME: %s)\n", new_rec.id, new_rec.name);
    } else {
        printf("ERROR: INSUFFICIENT VALUES (EXPECTED 8, GOT %d)\n", field_num);
        printf("USAGE: INSERT INTO SUSPECTS VALUES ('NAME', 'CITY', 'OCCUPATION', AGE, SUSPICION, CONTACTS, REPORTS, 'NOTES')\n");
    }
}

/* Execute DELETE statement */
void execute_delete(Parser *parser) {
    int i, count = 0;
    int has_where = 0;
    int where_pos = 0;
    
    /* Find WHERE clause */
    for (i = 0; i < parser->token_count; i++) {
        if (parser->tokens[i].type == TOKEN_WHERE) {
            has_where = 1;
            where_pos = i + 1;
            break;
        }
    }
    
    if (!has_where) {
        printf("ERROR: DELETE REQUIRES WHERE CLAUSE (SAFETY)\n");
        printf("USAGE: DELETE FROM SUSPECTS WHERE <condition>\n");
        return;
    }
    
    for (i = 0; i < MAX_RECORDS; i++) {
        if (g_database[i].active) {
            int pos = where_pos;
            if (evaluate_where_clause(parser, &g_database[i], &pos)) {
                g_database[i].active = 0;
                g_record_count--;
                count++;
            }
        }
    }
    
    printf("RECORDS DELETED: %d\n", count);
}

/* Execute UPDATE statement */
void execute_update(Parser *parser) {
    int i, count = 0;
    int set_pos = 0, where_pos = 0;
    int has_where = 0;
    char update_field[MAX_TOKEN_LEN];
    char update_value[MAX_TOKEN_LEN];
    
    /* Find SET and WHERE clauses */
    for (i = 0; i < parser->token_count; i++) {
        if (parser->tokens[i].type == TOKEN_SET) {
            set_pos = i + 1;
        }
        if (parser->tokens[i].type == TOKEN_WHERE) {
            has_where = 1;
            where_pos = i + 1;
            break;
        }
    }
    
    if (set_pos == 0) {
        printf("ERROR: UPDATE REQUIRES SET CLAUSE\n");
        printf("USAGE: UPDATE SUSPECTS SET FIELD=VALUE WHERE <condition>\n");
        return;
    }
    
    /* Parse SET clause (simplified: single field) */
    if (parser->tokens[set_pos].type == TOKEN_IDENTIFIER &&
        set_pos + 1 < parser->token_count &&
        parser->tokens[set_pos + 1].type == TOKEN_EQUALS &&
        set_pos + 2 < parser->token_count) {
        
        strncpy(update_field, parser->tokens[set_pos].value, MAX_TOKEN_LEN - 1);
        update_field[MAX_TOKEN_LEN - 1] = '\0';
        
        strncpy(update_value, parser->tokens[set_pos + 2].value, MAX_TOKEN_LEN - 1);
        update_value[MAX_TOKEN_LEN - 1] = '\0';
    } else {
        printf("ERROR: INVALID SET SYNTAX\n");
        return;
    }
    
    /* Update matching records */
    for (i = 0; i < MAX_RECORDS; i++) {
        if (g_database[i].active) {
            int matches = 1;
            
            if (has_where) {
                int pos = where_pos;
                matches = evaluate_where_clause(parser, &g_database[i], &pos);
            }
            
            if (matches) {
                /* Apply update */
                if (strcmp(update_field, "SUSPICION_LEVEL") == 0) {
                    g_database[i].suspicion_level = atoi(update_value);
                } else if (strcmp(update_field, "AGE") == 0) {
                    g_database[i].age = atoi(update_value);
                } else if (strcmp(update_field, "CONTACTS_MONITORED") == 0) {
                    g_database[i].contacts_monitored = atoi(update_value);
                } else if (strcmp(update_field, "REPORTS_FILED") == 0) {
                    g_database[i].reports_filed = atoi(update_value);
                } else if (strcmp(update_field, "NOTES") == 0) {
                    strncpy(g_database[i].notes, update_value, MAX_NOTES_LEN - 1);
                    g_database[i].notes[MAX_NOTES_LEN - 1] = '\0';
                }
                count++;
            }
        }
    }
    
    printf("RECORDS UPDATED: %d\n", count);
}
