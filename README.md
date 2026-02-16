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

## Supported SQL Features

### Query Modifiers
- **DISTINCT** - Removes duplicate rows from result set
- **LIMIT** - Restricts the number of rows returned
- **OFFSET** - Skips the specified number of initial rows
- **FETCH** - SQL standard syntax for limiting rows (FETCH FIRST/NEXT n ROWS ONLY)
- **ORDER BY** - Sorts results by specified field (ASC or DESC)

### Operators
- **Comparison**: `=`, `>`, `<`, `>=`, `<=`, `<>`, `!=`
- **LIKE** - Pattern matching with wildcards (`%` for any characters, `_` for single character)
- **IN** - Matches any value in a list
- **BETWEEN** - Matches values within a range (inclusive)

### Logical Operators
- **AND** - Requires both conditions to be true
- **OR** - Requires at least one condition to be true
- **NOT** - Reverses the logical condition

## SQL Command Examples

### SELECT Queries

#### Basic SELECT
```sql
SELECT * FROM SUSPECTS;
SELECT * FROM SUSPECTS WHERE AGE > 30 AND CITY = 'NEW YORK';
```

#### DISTINCT - Remove duplicate rows
```sql
SELECT DISTINCT * FROM SUSPECTS WHERE CITY = 'BOSTON';
```

#### ORDER BY - Sort results
```sql
SELECT * FROM SUSPECTS ORDER BY AGE;
SELECT * FROM SUSPECTS ORDER BY NAME DESC;
SELECT * FROM SUSPECTS WHERE CITY = 'CHICAGO' ORDER BY AGE ASC;
```

#### LIMIT and OFFSET - Restrict and skip rows
```sql
SELECT * FROM SUSPECTS LIMIT 10;
SELECT * FROM SUSPECTS ORDER BY AGE LIMIT 5;
SELECT * FROM SUSPECTS OFFSET 10 LIMIT 20;
```

#### FETCH - SQL standard row limiting
```sql
SELECT * FROM SUSPECTS FETCH FIRST 10 ROWS ONLY;
SELECT * FROM SUSPECTS FETCH NEXT 5 ROWS ONLY;
```

#### LIKE - Pattern matching
```sql
SELECT * FROM SUSPECTS WHERE NAME LIKE 'JOHN%';
SELECT * FROM SUSPECTS WHERE CITY LIKE '%NEW%';
SELECT * FROM SUSPECTS WHERE NAME LIKE 'J_HN DOE';
```

#### IN - Match any value in list
```sql
SELECT * FROM SUSPECTS WHERE CITY IN ('NEW YORK', 'BOSTON', 'CHICAGO');
SELECT * FROM SUSPECTS WHERE AGE IN (25, 30, 35, 40);
```

#### BETWEEN - Range matching
```sql
SELECT * FROM SUSPECTS WHERE AGE BETWEEN 25 AND 40;
SELECT * FROM SUSPECTS WHERE ID BETWEEN 1 AND 10;
```

#### Logical Operators - AND, OR, NOT
```sql
SELECT * FROM SUSPECTS WHERE AGE > 30 AND CITY = 'NEW YORK';
SELECT * FROM SUSPECTS WHERE CITY = 'BOSTON' OR CITY = 'CHICAGO';
SELECT * FROM SUSPECTS WHERE NOT AGE > 50;
SELECT * FROM SUSPECTS WHERE AGE > 25 AND (CITY = 'NEW YORK' OR CITY = 'BOSTON');
```

#### Complex Queries - Combining multiple features
```sql
SELECT DISTINCT * FROM SUSPECTS WHERE AGE BETWEEN 30 AND 50 ORDER BY CITY LIMIT 10;
SELECT * FROM SUSPECTS WHERE NAME LIKE 'J%' AND CITY IN ('NEW YORK', 'BOSTON') ORDER BY AGE DESC;
SELECT * FROM SUSPECTS WHERE NOT CITY = 'CHICAGO' ORDER BY NAME FETCH FIRST 20 ROWS ONLY;
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