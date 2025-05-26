Overview
This project implements Conway's Game of Life, a cellular automaton that simulates the evolution of a grid of cells based on specific rules. The program includes additional features like graph analysis of cell patterns, tree-based simulation tracking and Hamiltonian Paths finder.

Key Features
Core Game of Life: Simulates cell evolution with classic rules

Multiple Output Modes: Supports different test cases with varying output formats (in console or in files)

Game Of Life implementation (Task 1)

Stack Saving: Saves cells that change their state (Task 2)

Tree Simulation: Tracks simulation paths using a binary tree structure (Task 3)

Graph Analysis: Finds longest connected paths of live cells (Task 4 bonus)

Memory Management: Properly handles dynamic memory allocation and freeing

File Structure
coords.c: Coordinate list management

generations.c: Generation tracking functionality

matrix.c: Core matrix operations and game logic

graph.c: Graph analysis for connected cells

newrules.c: Alternative simulation rules and tree implementation

GameOfLife.c: Main program

Usage

To compile use "make"
Run with input and output file parameters:

./a.out input.txt output.txt

Input Format
The input file should contain:

Test number (1-4)

Grid dimensions (lines, columns)

Number of generations to simulate

The initial grid (X for live cells, + for dead cells)

Output
Output varies by test case:

Test 1: Prints each generation's grid

Test 2: Shows cell changes between generations

Test 3: Tree-based simulation paths and graph analysis

Test 4: Hamiltonian Paths Lengths and Paths