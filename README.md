#  Sudoku Solver (C++)

##  Project Overview

This project is a simple yet powerful **Sudoku Solver** built using **C++**. It uses a technique called **backtracking** to solve a standard 9×9 Sudoku puzzle. Users can enter their own puzzle, and the program will automatically find the correct solution while making sure all Sudoku rules are followed.

To make things more interactive and easier to use, the project also includes a **Graphical User Interface (GUI)** that visually displays the puzzle and its solution.

---

##  Objectives

* Build a Sudoku solver using **recursion and backtracking**
* Store the Sudoku grid using a **2D array**
* Ensure all Sudoku rules are followed:

  * No repeated numbers in a row
  * No repeated numbers in a column
  * No repeated numbers in a 3×3 box
* Create a **simple and user-friendly interface**
* Practice solving problems using algorithms

---

##  Technologies Used

* **C++**
* **SFML (Simple and Fast Multimedia Library)** *(for GUI version)*
  OR
* **Windows API (GDI)** *(for basic GUI version)*

---



##  How It Works

1. The Sudoku grid is stored in a **9×9 matrix**
2. Empty cells are represented by **0**
3. The algorithm:

   * Finds an empty cell
   * Tries numbers from 1 to 9
   * Checks if placement is valid
   * Recursively continues
   * Backtracks if a number fails
4. Once solved, the grid is displayed on the GUI

---

##  How to Run the Project

###  For SFML Version

#### Step 1: Install SFML

Download SFML and extract it (e.g., `C:\SFML`)

#### Step 2: Compile

```
g++ main.cpp solver.cpp gui.cpp -o sudoku -std=c++17 -I C:\SFML\include -L C:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-system
```

#### Step 3: Copy Required DLLs

Copy from:

```
C:\SFML\bin
```

To your project folder:

```
sfml-graphics-2.dll
sfml-window-2.dll
sfml-system-2.dll
```

#### Step 4: Run

```
./sudoku
```

---

###  For Windows API Version

Compile using:

```
g++ SudokuSolver.cpp -o SudokuSolver.exe -lgdi32
```

Run:

```
SudokuSolver.exe
```

---

##  Features

* Interactive Sudoku grid
* Automatic solving using backtracking
* GUI-based display
* Reset functionality
* Input validation (based on Sudoku rules)

---

##  Algorithm Used

**Backtracking Algorithm**

### Key Idea:

Try all possible numbers and backtrack when a constraint is violated.

---

##  Time Complexity

* Worst Case: **O(9^(n×n))**
* Optimized through constraint checking

---

##  Future Enhancements

* Puzzle generator with difficulty levels
* Timer and scoring system
* Hint system
* Dark mode UI
* Animation during solving

---

##  Learning Outcomes

* Understanding recursion and backtracking
* GUI development in C++
* Problem-solving using constraints
* Code modularization

---

##  Author

**Humaira Mubarak**
BS Computer Science Student

---

##  Conclusion

This project demonstrates how **algorithmic problem-solving techniques like backtracking** can efficiently solve complex puzzles such as Sudoku, along with enhancing user interaction through a graphical interface.

---
