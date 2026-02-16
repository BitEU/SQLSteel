# SQLSteel - Ultra-Lightweight In-Memory Database for UNIVAC 1219

An ultra-minimal SQL database engine. Everything runs in RAM - no disk I/O or persistence whatsoever.

## Architecture

```
sqlsteel.h      - Main header file with type definitions
sqlsteel.c      - Main program and command loop
database.c      - Database initialization and data generation
sqlengine.c     - SQL tokenizer and parser
query.c         - Query executor (SELECT, INSERT, UPDATE, DELETE)
display.c       - Output formatting
```

## Sample Data

100 records of "Suspected Soviet Sympathies" with randomly generated names from a pool of:
- 100+ first names
- 100+ last names  
- 20 cities
- 15 occupations
- 15 note types

Each record contains:
- ID (unique identifier)
- Name (randomly generated)
- City
- Occupation
- Age
- Suspicion Level (1-10)
- Contacts Monitored
- Reports Filed
- Notes

## SQL Command Examples

### SELECT Queries
```sql
SELECT * FROM SUSPECTS;
SELECT * FROM SUSPECTS WHERE SUSPICION_LEVEL >= 8;
SELECT * FROM SUSPECTS WHERE AGE > 30 AND CITY = 'NEW YORK';
SELECT * FROM SUSPECTS WHERE CONTACTS_MONITORED > 10;
```

### INSERT Records
```sql
INSERT INTO SUSPECTS VALUES ('JOHN DOE', 'BOSTON', 'ENGINEER', 35, 7, 5, 10, 'SUSPICIOUS');
INSERT INTO SUSPECTS VALUES ('JANE SMITH', 'CHICAGO', 'SCIENTIST', 42, 9, 12, 18, 'HIGH PRIORITY');
```

### UPDATE Records
```sql
UPDATE SUSPECTS SET SUSPICION_LEVEL = 10 WHERE ID = 5;
UPDATE SUSPECTS SET NOTES = 'CLEARED' WHERE SUSPICION_LEVEL < 3;
UPDATE SUSPECTS SET CONTACTS_MONITORED = 0 WHERE AGE < 25;
```

### DELETE Records
```sql
DELETE FROM SUSPECTS WHERE ID = 5;
DELETE FROM SUSPECTS WHERE SUSPICION_LEVEL < 2;
DELETE FROM SUSPECTS WHERE AGE > 65;
```

## Special Commands

- `STATS` - Display database statistics
- `HELP` - Show command reference
- `EXIT` or `QUIT` - Terminate program

## Building

Run `build_oregon.bat` and choose your compiler:

### Option 1: Windows (MinGW)
```batch
build_oregon.bat
[Select 1 for Windows]
[Select 1 for MinGW]
```

### Option 2: Windows (MSVC)
```batch
build_oregon.bat
[Select 1 for Windows]
[Select 2 for MSVC]
```

### Option 3: UNIVAC Cross-Compile
```batch
build_oregon.bat
[Select 2 for UNIVAC]
```