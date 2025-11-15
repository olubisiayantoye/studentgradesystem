# Student Grade Management System (Console C++)

## Project Overview
A simple console-based Student Grade Management System written in C++. It supports:
- Add, display, search, edit and delete student records
- Automatic average and letter grade calculation
- Save/load records to/from a text file
- Basic input validation and persistent ID generation across loads

This project was created as part of CSE310 Modules 1 W02 Assignment requirements and demonstrates CRUD operations, file I/O, and basic data validation in C++.

---

## Features
- Add student (name, subject, three scores)
- Display all students in a formatted table
- Search by student ID or partial/full name (case-insensitive)
- Edit student information & scores
- Delete student (with confirmation)
- Save to file (`students.txt`) using a pipe (`|`) delimiter (safe for commas)
- Load from file (supports both `|` and `,` files)
- Persistent student ID generation (won't reuse IDs after loading existing data)
- Input validation for numeric scores (0–100)

---

## Requirements
- Any modern C++ compiler (g++ recommended)
- No external libraries required

---

## Build & Run

### Linux / macOS (with g++)
```bash
g++ -std=c++11 -O2 -o grade_manager main.cpp
./grade_manager
