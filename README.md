# SQLSteel - Ultra-Lightweight In-Memory Database for UNIVAC 1219

An ultra-minimal SQL database engine. Everything runs in memory - no disk I/O or persistence whatsoever.

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

Each record contains:
- ID (unique identifier)
- Name (randomly generated)
- City
- Age

## SQL Command Examples

### SELECT Queries
```sql
SELECT * FROM SUSPECTS;
SELECT * FROM SUSPECTS WHERE AGE > 30 AND CITY = 'NEW YORK';
```

### INSERT Records
```sql
INSERT INTO SUSPECTS VALUES ('JOHN DOE', 'BOSTON', 35);
INSERT INTO SUSPECTS VALUES ('JANE SMITH', 'CHICAGO', 42);
```

### UPDATE Records
```sql
UPDATE SUSPECTS SET AGE = 40 WHERE ID = 5;
```

### DELETE Records
```sql
DELETE FROM SUSPECTS WHERE ID = 5;
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