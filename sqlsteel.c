/*
 * SQLSteel - Ultra-Lightweight In-Memory Database for UNIVAC 1219
 * Main Program Module
 * 
 * Memory Budget: 40kB total
 * Features: Full SQL parser with SELECT, INSERT, DELETE, UPDATE
 * Data: 100 records of "Suspected Soviet Sympathies" surveillance data
 * 
 * Designed for 1960s UNIVAC - All data stored in runtime memory
 * No disk I/O - everything created and destroyed in memory
 */

#include "sqlsteel.h"

#include "sqlsteel.h"

/* Display statistics */
void cmd_stats(void) {
    printf("\n=== DATABASE STATISTICS ===\n");
    printf("TOTAL ACTIVE RECORDS: %d\n", get_active_count());
    printf("TOTAL DELETED SLOTS: %d\n", MAX_RECORDS - get_active_count());
    printf("MEMORY USAGE: ~%d BYTES\n", (int)(sizeof(g_database) + sizeof(g_record_count)));
}

/* Display help menu */
void cmd_help(void) {
    printf("\n=== SQLSTEEL - SQL COMMAND REFERENCE ===\n");
    printf("\nSUPPORTED SQL COMMANDS:\n");
    printf("\n1. SELECT:\n");
    printf("   SELECT * FROM SUSPECTS;\n");
    printf("   SELECT * FROM SUSPECTS WHERE AGE > 30 AND CITY = 'NEW YORK';\n");
    printf("\n2. INSERT:\n");
    printf("   INSERT INTO SUSPECTS VALUES ('NAME', 'CITY', AGE);\n");
    printf("   Example: INSERT INTO SUSPECTS VALUES ('JOHN DOE', 'BOSTON', 35);\n");
    printf("\n3. UPDATE:\n");
    printf("   UPDATE SUSPECTS SET AGE = 40 WHERE ID = 5;\n");
    printf("\n4. DELETE:\n");
    printf("   DELETE FROM SUSPECTS WHERE ID = 5;\n");
    printf("\nOTHER COMMANDS:\n");
    printf("   STATS - Show database statistics\n");
    printf("   HELP  - Show this menu\n");
    printf("   EXIT  - Terminate program\n");
    printf("\nFIELDS: ID, NAME, CITY, AGE\n");
    printf("OPERATORS: =, >, <, >=, <=, AND\n");
    printf("\nMEMORY: NO DISK STORAGE\n");
}

/* Main program */
int main(void) {
    char sql_buffer[MAX_SQL_LEN];
    
    printf("\n");
    printf("===================================================\n");
    printf("  SQLSTEEL - IN-MEMORY DATABASE SYSTEM\n");
    printf("===================================================\n");
    printf("\n");
    
    init_database();
    load_demo_data();
    
    printf("\nSYSTEM READY. TYPE 'HELP' FOR COMMAND REFERENCE.\n");
    printf("TYPE SQL COMMANDS OR 'EXIT' TO QUIT.\n");
    
    while (1) {
        printf("\nSQL> ");
        
        if (fgets(sql_buffer, MAX_SQL_LEN, stdin) == NULL) {
            break;
        }
        
        /* Remove newline */
        sql_buffer[strcspn(sql_buffer, "\n")] = '\0';
        
        /* Skip empty lines */
        if (strlen(sql_buffer) == 0) continue;
        
        /* Convert to uppercase for comparison */
        char cmd_check[MAX_SQL_LEN];
        PLATFORM_STRCPY(cmd_check, MAX_SQL_LEN, sql_buffer);
        to_upper(cmd_check);
        
        /* Check for special commands */
        if (strcmp(cmd_check, "EXIT") == 0 || strcmp(cmd_check, "QUIT") == 0) {
            printf("\nTERMINATING SQLSTEEL...\n");
            break;
        }
        else if (strcmp(cmd_check, "HELP") == 0) {
            cmd_help();
        }
        else if (strcmp(cmd_check, "STATS") == 0) {
            cmd_stats();
        }
        else {
            /* Execute SQL command */
            execute_sql(sql_buffer);
        }
    }
    
    return 0;
}
