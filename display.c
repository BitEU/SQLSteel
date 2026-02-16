/*
 * Display Module
 * Handles formatted output of records
 */

#include "sqlsteel.h"

/* Print table header */
void print_header(void) {
    print_separator();
    printf("| ID  | NAME                 | CITY             | OCCUPATION       | AGE | SUSP | CONTACTS | REPORTS |\n");
    print_separator();
}

/* Print separator line */
void print_separator(void) {
    printf("+-----+----------------------+------------------+------------------+-----+------+----------+---------+\n");
}

/* Print single record (table format) */
void print_record(SuspectRecord *rec) {
    if (rec->active) {
        printf("| %-3d | %-20s | %-16s | %-16s | %-3d | %-4d | %-8d | %-7d |\n",
               rec->id, rec->name, rec->city, rec->occupation,
               rec->age, rec->suspicion_level, rec->contacts_monitored, rec->reports_filed);
    }
}

/* Print full record details */
void print_full_record(SuspectRecord *rec) {
    printf("\n=== RECORD ID: %d ===\n", rec->id);
    printf("NAME:               %s\n", rec->name);
    printf("CITY:               %s\n", rec->city);
    printf("OCCUPATION:         %s\n", rec->occupation);
    printf("AGE:                %d\n", rec->age);
    printf("SUSPICION LEVEL:    %d/10\n", rec->suspicion_level);
    printf("CONTACTS MONITORED: %d\n", rec->contacts_monitored);
    printf("REPORTS FILED:      %d\n", rec->reports_filed);
    printf("NOTES:              %s\n", rec->notes);
    printf("STATUS:             %s\n", rec->active ? "ACTIVE" : "DELETED");
    printf("=========================\n");
}
