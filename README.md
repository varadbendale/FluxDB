## FluxDB

FluxDB is a relational database engine that reads and writes directly
to a file on disk — the same approach SQLite uses — built completely in C.

Databases run the entire tech industry, yet almost no one knows how this
wonderful piece of software actually works. This is that black box,
rebuilt in C, piece by piece.

## FluxDB Supports

```
SELECT [columns | CASE ... END]
  │
  ├── FROM <table>
  │     └── JOIN <table> ON <condition>
  │
  ├── WHERE <condition>
  │
  ├── GROUP BY <columns>
  │     └── HAVING <condition>
  │
  ├── ORDER BY <columns> [ASC|DESC]
  │
  └── LIMIT <n>
```

❌ No subqueries / nested SELECTs — every query is single-pass, no nesting.

## Architecture Overview

```
+----------------------------------------------------------------------+
|                          PREPROCESSING                               |
|                                                                      |
|   +---------------+        +----------------------+                  |
|   |   Tokenizer   |  --->  |   Parser (-> AST)    |                  |
|   +---------------+        +----------+-----------+                  |
+---------------------------------------|--------------------------------+
                                        v
+----------------------------------------------------------------------+
|                         EXECUTION ENGINE                             |
|                                                                      |
|      +----------------------+                                        |
|      |  Bytecode Generator  |                                        |
|      +-----------+----------+                                        |
|                  |                                                   |
|                  v                                                   |
|      +----------------------+        +------------------------+      |
|      |    Virtual Machine   | <----> |   WHERE Clause Engine  |      |
|      +-----------+----------+        +------------------------+      |
|                  |                                                   |
+------------------|----------------------------------------------------+
                   | Read/Write rows
                   v
+----------------------------------------------------------------------+
|                          STORAGE ENGINE                              |
|                                                                      |
|   +---------------+        +---------------+        +-----------+    |
|   |     Pager     | <----> |    B-Tree     | <----> |  .db file |    |
|   +---------------+        +---------------+        +-----------+    |
+----------------------------------------------------------------------+
```

## Repository Structure

```
FluxDB/
├── src
│   ├── Execution Engine
│   │   ├── Group by 
│   │   │   └── Group_by.c
│   │   ├── Join
│   │   │   ├── Join.c
│   │   │   └── Readme.md
│   │   ├── Order by 
│   │   │   └── Order_by.c
│   │   ├── Engine.c
│   │   ├── Engine.h
│   │   └── Readme.md
│   ├── Parser
│   │   ├── Create
│   │   │   └── Create_parser.c
│   │   ├── Insert
│   │   │   └── insert_parser.c
│   │   ├── Select
│   │   │   ├── Readme.md
│   │   │   └── select_parser.c
│   │   ├── Parser.h
│   │   └── Readme.md
│   ├── Repl
│   │   └── Interface
│   │       ├── Readme.md
│   │       └── terminal.py
│   ├── Storage Engine
│   │   ├── Data in Pages
│   │   │   ├── File Operations
│   │   │   │   └── Page_Manager.c
│   │   │   ├── Pages
│   │   │   │   └── insert_pager.c
│   │   │   └── Pages Header 
│   │   │       ├──   Header_page.c
│   │   │       └── Readme.md
│   │   ├── Pager 
│   │   │   └── FSM.c
│   │   └── storage_engine.h
│   └── VM Bytecodes
│       ├── bytecode.c
│       └── Readme.md
├── Issues
└── README.md
```

## Prerequisites

* `gcc` (or `clang`) — C compiler
* A Linux/macOS environment — file I/O assumes POSIX-style paths


