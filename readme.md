# UniHash — Hash Table Based Student Record Management System

A menu-driven C++ application built for **Advanced Data Structures and Algorithms (ADSA)** at **AISSMS Institute of Information Technology, Pune**. UniHash stores and manages student records using a **Hash Table with Separate Chaining** (`M = 7` buckets) and persistent file storage.

---

## Features

- **Division-Method Hashing:** Maps each unique Student ID to a bucket index (`0` to `6`) using `h(id) = id % 7`.
- **Separate Chaining:** Resolves hash collisions cleanly using singly linked lists (`struct Node`).
- **Duplicate ID Prevention:** Rejects insertions if a Student ID already exists in its bucket chain.
- **Three Display Modes:**
  1. **Bucket-wise (`0` to `6`):** Displays records bucket by bucket in chain order.
  2. **Sorted by ID:** Collects all records and displays them in ascending order of Student ID.
  3. **Chain Summary:** Visualizes each bucket's chain length, linked Student IDs, total record count, and live load factor (`n / 7`).
- **Update & Safe Deletion:** Updates student CGPA in place and handles all three linked-list deletion cases (sole node in bucket, chain head, and middle/tail node).
- **Persistent File Handling:** Automatically loads records from `students.txt` at startup and saves updates after every modification and on exit.
- **Zero Memory Leaks:** Dynamically allocates each `Node` with `new` and releases all chains in `~StudentHashTable()` using `delete`.

---

## Project Structure

```text
UniHash/
├── hash_student.cpp   # C++ source code (Node struct, StudentHashTable class, CLI menu)
├── students.txt       # Pipe-delimited student records (ID|Name|CGPA|Attendance)
└── README.md          # Project documentation
```

---

## Getting Started

### Prerequisites

- A C++11 (or later) compiler such as `g++`.

### Compile & Run

**Linux / macOS:**

```bash
g++ -std=c++11 hash_student.cpp -o hash_student
./hash_student
```

**Windows (MinGW / Command Prompt):**

```powershell
g++ -std=c++11 hash_student.cpp -o hash_student.exe
.\hash_student.exe
```

> **Note:** Make sure `students.txt` is in the same directory as the compiled executable to pre-load sample records. If the file does not exist, the program starts with an empty hash table and creates `students.txt` automatically on save.

---

## Sample Data Format (`students.txt`)

Records are stored one per line using `|` as a delimiter so student names can contain spaces:

```text
105|Aarav Sharma|8.75|92
107|Ananya Nair|9.3|96
108|Rohan Kulkarni|9.1|95
101|Sneha Patil|7.95|85
102|Priya Deshmukh|8.4|88
103|Vikram Joshi|8.85|90
110|Kunal Mehta|8.15|82
```

---

## Menu Operations

```text
===== HASH TABLE STUDENT RECORD SYSTEM =====
1. Insert Student
2. Search Student
3. Display Students (Bucket-wise / Sorted / Chain Summary)
4. Update CGPA
5. Delete Student
6. Exit
```

---

## Time & Space Complexity

| Operation | Average Case | Worst Case |
| :--- | :--- | :--- |
| Search by Student ID | `O(1)` | `O(n)` |
| Insert Student | `O(1)` | `O(n)` |
| Update CGPA | `O(1)` | `O(n)` |
| Delete Student | `O(1)` | `O(n)` |
| Bucket-wise / Chain Summary | `O(n + M)` | `O(n + M)` |
| Sorted Display | `O(n log n)` | `O(n log n)` |

- **Space Complexity:** `O(n + M)` where `M = 7` buckets and `n` is the number of student records.

---

## Future Scope

- **Dynamic Rehashing:** Automatically resize the bucket array when the load factor exceeds a threshold.
- **Graphical User Interface (GUI):** Build a desktop or web frontend for visual bucket inspection.
- **Extended Academic Tracking:** Support subject-wise grade sheets and attendance alerts.

---
