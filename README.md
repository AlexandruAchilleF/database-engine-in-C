# SimpleDB-C 🗄️

A simple, console-based (CLI) database engine written from scratch in pure C.

This project was created for educational purposes to understand the fundamental concepts behind Database Management Systems (DBMS): how data is stored on disk (in binary format), how to manage memory manually, and how to structure a C project.

## 🚀 Features

* **CLI Interface:** An interactive loop that accepts commands, similar to a standard SQL shell.
* **SQL-like Commands:** Supports operations such as `insert`, `select`, `update`, and `delete`.
* **Meta-commands:** Supports system commands like `.exit` and `.help`.
* **Persistence:** Data is preserved after the program closes; it is saved to a local binary file (`file.db`).
* **Binary Format:** Uses C structure serialization to efficiently write data to disk.
* **No Dependencies:** Built using only standard C libraries.

## 🛠️ Technical Concepts Explored

This project explores the following C programming concepts:
* **Pointers & Dynamic Allocation:** Manual memory management using `malloc` and `free`.
* **File I/O:** Reading and writing data structures directly to binary files (using `fwrite`, `fread`, `fseek`).
* **Multi-file Compilation:** Linking multiple source files (`main.c`, `functions.c`) and header files.
* **Parsing:** Tokenizing user input to interpret commands using `sscanf`.

## 💻 How to Build and Run

You will need a C compiler (such as `gcc` or `clang`).

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/AlexandruAchilleF/database-engine-in-C.git
    cd database-engine-in-C
    ```

2.  **Compile:**
    *Note: You must compile both source files together.*
    ```bash
    gcc main.c functions.c -o db
    ```

3.  **Run:**
    ```bash
    ./db
    ```

## 📝 Usage Example

```text
db > .help

------- Help Menu -------
.exit   : Exit the database.
insert <username> <email> : Insert user.
select  : Show users.
delete <id> : Delete user.
update <id> <username> <email> : Update user.
-------------------------

db > insert firstuser user@mail.com
Executed.

db > insert user2 seconduser@generic.web
Executed.

db > select
ID   | Username                        | Email
--------------------------------------------------------------
1    | firstuser                       | user@mail.com
2    | user2                           | seconduser@generic.web
--------------------------------------------------------------
Executed.

db > update 1 newname new@mail.com
Executed.

db > delete 2
Executed.

db > select
ID   | Username                        | Email
--------------------------------------------------------------
1    | newname                         | new@mail.com
--------------------------------------------------------------
Executed.

db > .exit
