/*
 * Database Operations Module
 * Handles initialization, data generation, and basic operations
 */

#include "sqlsteel.h"

/* Global in-memory database */
SuspectRecord g_database[MAX_RECORDS];
sql_int_t g_record_count = 0;

/* Name generation data */
static const char *first_names[] = {
    "JOHN", "MARY", "ROBERT", "PATRICIA", "JAMES", "JENNIFER", "MICHAEL", "LINDA",
    "WILLIAM", "ELIZABETH", "DAVID", "BARBARA", "RICHARD", "SUSAN", "JOSEPH", "JESSICA",
    "THOMAS", "SARAH", "CHARLES", "KAREN", "CHRISTOPHER", "NANCY", "DANIEL", "MARGARET",
    "MATTHEW", "LISA", "ANTHONY", "BETTY", "DONALD", "DOROTHY", "MARK", "SANDRA",
    "PAUL", "ASHLEY", "STEVEN", "KIMBERLY", "ANDREW", "DONNA", "KENNETH", "EMILY",
    "GEORGE", "CAROL", "JOSHUA", "MICHELLE", "KEVIN", "AMANDA", "BRIAN", "MELISSA",
    "EDWARD", "DEBORAH", "RONALD", "STEPHANIE", "TIMOTHY", "REBECCA", "JASON", "LAURA",
    "JEFFREY", "SHARON", "RYAN", "CYNTHIA", "JACOB", "KATHLEEN", "GARY", "AMY",
    "NICHOLAS", "SHIRLEY", "ERIC", "ANGELA", "JONATHAN", "HELEN", "STEPHEN", "ANNA",
    "LARRY", "BRENDA", "JUSTIN", "PAMELA", "SCOTT", "NICOLE", "BRANDON", "RUTH",
    "FRANK", "KATHERINE", "BENJAMIN", "SAMANTHA", "GREGORY", "CHRISTINE", "SAMUEL", "CATHERINE",
    "RAYMOND", "VIRGINIA", "PATRICK", "DEBRA", "ALEXANDER", "RACHEL", "JACK", "JANET",
    "DENNIS", "EMMA", "JERRY", "CAROLYN", "TYLER", "MARIA", "AARON", "HEATHER"
};

static const char *last_names[] = {
    "SMITH", "JOHNSON", "WILLIAMS", "BROWN", "JONES", "GARCIA", "MILLER", "DAVIS",
    "RODRIGUEZ", "MARTINEZ", "HERNANDEZ", "LOPEZ", "GONZALEZ", "WILSON", "ANDERSON", "THOMAS",
    "TAYLOR", "MOORE", "JACKSON", "MARTIN", "LEE", "PEREZ", "THOMPSON", "WHITE",
    "HARRIS", "SANCHEZ", "CLARK", "RAMIREZ", "LEWIS", "ROBINSON", "WALKER", "YOUNG",
    "ALLEN", "KING", "WRIGHT", "SCOTT", "TORRES", "NGUYEN", "HILL", "FLORES",
    "GREEN", "ADAMS", "NELSON", "BAKER", "HALL", "RIVERA", "CAMPBELL", "MITCHELL",
    "CARTER", "ROBERTS", "GOMEZ", "PHILLIPS", "EVANS", "TURNER", "DIAZ", "PARKER",
    "CRUZ", "EDWARDS", "COLLINS", "REYES", "STEWART", "MORRIS", "MORALES", "MURPHY",
    "COOK", "ROGERS", "GUTIERREZ", "ORTIZ", "MORGAN", "COOPER", "PETERSON", "BAILEY",
    "REED", "KELLY", "HOWARD", "RAMOS", "KIM", "COX", "WARD", "RICHARDSON",
    "WATSON", "BROOKS", "CHAVEZ", "WOOD", "JAMES", "BENNETT", "GRAY", "MENDOZA",
    "RUIZ", "HUGHES", "PRICE", "ALVAREZ", "CASTILLO", "SANDERS", "PATEL", "MYERS",
    "LONG", "ROSS", "FOSTER", "JIMENEZ"
};

static const char *cities[] = {
    "NEW YORK", "WASHINGTON DC", "CHICAGO", "LOS ANGELES", "BOSTON",
    "SAN FRANCISCO", "PHILADELPHIA", "DETROIT", "BALTIMORE", "SEATTLE",
    "MIAMI", "ATLANTA", "DALLAS", "HOUSTON", "DENVER", "PHOENIX",
    "LAS VEGAS", "PORTLAND", "MINNEAPOLIS", "ST LOUIS"
};

static const char *occupations[] = {
    "GOVT CLERK", "SCIENTIST", "DIPLOMAT", "ENGINEER", "JOURNALIST",
    "PROFESSOR", "TRANSLATOR", "ANALYST", "SECRETARY", "MILITARY",
    "TECHNICIAN", "RESEARCHER", "COURIER", "ATTACHE", "CONSULTANT"
};

static const char *notes[] = {
    "FREQUENT USSR CONTACTS", "UNEXPLAINED WEALTH", "CODED MESSAGES",
    "DEAD DROP OBSERVED", "KNOWN ASSOCIATES", "EMBASSY MEETINGS",
    "SUSPICIOUS TRAVEL", "DOCUMENTS MISSING", "FAILED POLYGRAPH",
    "ANONYMOUS TIP", "PHONE TAP EVIDENCE", "UNDER SURVEILLANCE",
    "FINANCIAL IRREGULARITIES", "HANDLER IDENTIFIED", "SAFE HOUSE VISITS"
};

/* XORshift pseudo-random number generator (UNIVAC compatible) */
static unsigned long g_xorshift_state = 0;

static void seed_random(void) {
    unsigned long seed = (unsigned long)time(0);
    /* Ensure seed is never zero (XORshift requirement) */
    g_xorshift_state = (seed == 0) ? 12345 : seed;
}

static int pseudo_rand(void) {
    /* XORshift algorithm - very fast and good quality */
    g_xorshift_state ^= g_xorshift_state << 13;
    g_xorshift_state ^= g_xorshift_state >> 17;
    g_xorshift_state ^= g_xorshift_state << 5;
    return (int)(g_xorshift_state & 0x7FFF); /* Return 0-32767 */
}

/* Generate random name from first and last name lists */
void generate_random_name(char *dest, int max_len) {
    int first_idx = pseudo_rand() % (sizeof(first_names) / sizeof(first_names[0]));
    int last_idx = pseudo_rand() % (sizeof(last_names) / sizeof(last_names[0]));
    
#ifdef UNIVAC
    /* UNIVAC-compatible string formatting */
    int written = 0;
    int i, j;
    
    /* Copy first name */
    for (i = 0; first_names[first_idx][i] != '\0' && written < max_len - 2; i++) {
        dest[written++] = first_names[first_idx][i];
    }
    
    /* Add space */
    if (written < max_len - 1) {
        dest[written++] = ' ';
    }
    
    /* Copy last name */
    for (j = 0; last_names[last_idx][j] != '\0' && written < max_len - 1; j++) {
        dest[written++] = last_names[last_idx][j];
    }
    
    dest[written] = '\0';
#else
    snprintf(dest, max_len, "%s %s", first_names[first_idx], last_names[last_idx]);
    dest[max_len - 1] = '\0';
#endif
}

/* Initialize database */
void init_database(void) {
    int i;
    for (i = 0; i < MAX_RECORDS; i++) {
        g_database[i].active = 0;
        g_database[i].id = 0;
    }
    g_record_count = 0;
}

/* Load demo surveillance data with randomly generated names */
void load_demo_data(void) {
    int i;
    
    /* Initialize random seed from current time */
    seed_random();
    
    for (i = 0; i < 100; i++) {
        g_database[i].id = i + 1;
        g_database[i].active = 1;
        
        /* Generate random name */
        generate_random_name(g_database[i].name, MAX_NAME_LEN);
        
        /* Assign city, occupation, notes using portable string copy */
        PLATFORM_STRCPY(g_database[i].city, MAX_CITY_LEN, cities[pseudo_rand() % 20]);
        PLATFORM_STRCPY(g_database[i].occupation, MAX_OCCUPATION_LEN, occupations[pseudo_rand() % 15]);
        PLATFORM_STRCPY(g_database[i].notes, MAX_NOTES_LEN, notes[pseudo_rand() % 15]);
        
        /* Generate realistic data */
        g_database[i].age = 25 + (pseudo_rand() % 40);
        g_database[i].suspicion_level = 1 + (pseudo_rand() % 10);
        g_database[i].contacts_monitored = pseudo_rand() % 20;
        g_database[i].reports_filed = pseudo_rand() % 25;
    }
    
    g_record_count = 100;
    printf("DATABASE INITIALIZED: 100 SUSPECT RECORDS LOADED\n");
}

/* Get count of active records */
int get_active_count(void) {
    int i, count = 0;
    for (i = 0; i < MAX_RECORDS; i++) {
        if (g_database[i].active) count++;
    }
    return count;
}
