# SimpleDB-C 🗄️

A simple, console-based (CLI) database engine written from scratch in pure C.

This project was created for educational purposes to understand the fundamental concepts behind Database Management Systems (DBMS): how data is stored on disk (in binary format), how to manage memory manually and how to use git.

## 🚀 Features

* **CLI Interface:** An interactive loop that accepts commands, similar to a standard SQL shell.
* **SQL-like Commands:** Supports basic operations such as `insert` and `select`.
* **Persistence:** Data is preserved after the program closes; it is saved to a local binary file (`.db`).
* **Binary Format:** Uses C structure serialization to efficiently write data to disk, rather than using plain text/CSV.
* **No Dependencies:** Built using only standard C libraries.

## 🛠️ Technical Concepts Explored

This project explores the following C programming concepts:
* **Pointers & Dynamic Allocation:** Manual memory management using `malloc` and `free`.
* **File I/O:** Reading and writing data structures directly to binary files (using `fwrite`, `fread`, etc.).
* **Parsing:** Tokenizing user input to interpret commands.
* **Data Structures:** Organizing table rows in memory.

## 💻 How to Build and Run

You will need a C compiler (such as `gcc`).

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/AlexandruAchilleF/database-engine-in-C.git
    cd database-engine-in-C
    ```

2.  **Compile:**
    ```bash
    gcc main.c -o db
    ```

3.  **Run:**
    ```bash
    ./db
    ```

## 📝 Usage Example

```text
db > insert firstuser user@mail.com
Insert Executed.

db > insert user2 seconduser@generic.web
Insert Executed.

db > select
ID   | Username                        | Email
--------------------------------------------------------------
1    | firstuser                       | user@mail.com
2    | user2                           | seconduser@generic.web
--------------------------------------------------------------
Select Executed.

db > .exit
