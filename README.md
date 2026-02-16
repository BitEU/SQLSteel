# SQLSteel - Ultra-Lightweight In-Memory Database for UNIVAC 1219

An ultra-minimal SQL database engine designed for 1960s UNIVAC systems with only 40kB of memory. Everything runs in RAM - no disk I/O whatsoever.

## Features

- **Full SQL Parser**: Tokenizer and parser for SQL commands
- **SQL Operations**: SELECT, INSERT, UPDATE, DELETE
- **WHERE Clauses**: Supports conditions with =, >, <, >=, <=, AND operators
- **Dynamic Data Generation**: Random name generation using first/last name combinations
- **In-Memory Only**: All data created and destroyed at runtime
- **Modular Design**: Proper separation with header files

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

## Memory Footprint

- Database: ~15KB (100 records × ~150 bytes each)
- Code: ~10-15KB (depending on compiler optimization)
- Total: ~25-30KB (well under 40KB limit)

## UNIVAC Compatibility

- No dynamic memory allocation (malloc/free)
- Fixed-size arrays
- Simple pseudo-random number generator (no stdlib rand())
- No floating-point operations
- Standard C89/C99 only
- No platform-specific dependencies

## Technical Details

### Tokenizer
Converts SQL strings into tokens:
- Keywords (SELECT, INSERT, WHERE, etc.)
- Identifiers (field names)
- Operators (=, >, <, >=, <=)
- Strings ('value')
- Numbers (123)

### Parser
Recursive descent parser that builds execution plan from tokens.

### Query Evaluator
Evaluates WHERE clauses by:
1. Iterating through all active records
2. Testing each condition against record fields
3. Combining conditions with AND logic
4. Returning matching records

## Limitations

- Maximum 100 records
- Single table only
- No JOIN operations
- No ORDER BY or GROUP BY
- No aggregate functions (COUNT, SUM, etc.)
- WHERE clauses support AND but not OR
- Single field updates only
- String matching is exact (case-insensitive)

## License

Public domain - designed for educational purposes and UNIVAC nostalgia.
