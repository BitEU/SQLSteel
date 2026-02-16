/*
 * Display Module
 * Handles formatted output of records
 */

#include "sqlsteel.h"

/* Print table header */
void print_header(void) {
    print_separator();
    printf("# ID  # NAME                 # CITY             # AGE #\n");
    print_separator();
}

/* Print separator line */
void print_separator(void) {
    printf("#-----#----------------------#------------------#-----#\n");
}

/* Print single record (table format) */
void print_record(SuspectRecord *rec) {
    if (rec->active) {
        printf("# %-3d # %-20s # %-16s # %-3d #\n",
               rec->id, rec->name, rec->city,
               rec->age);
    }
}

/* Print full record details */
void print_full_record(SuspectRecord *rec) {
    printf("\n=== RECORD ID: %d ===\n", rec->id);
    printf("NAME:               %s\n", rec->name);
    printf("CITY:               %s\n", rec->city);
    printf("AGE:                %d\n", rec->age);
    printf("STATUS:             %s\n", rec->active ? "ACTIVE" : "DELETED");
    printf("=========================\n");
}
