# dArray: A Generic Dynamic Array Library in C

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Standard](https://img.shields.io/badge/Standard-C11-orange.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A simple, yet powerful and memory-safe dynamic array (vector) implementation in C, designed to be generic and handle different data types like `int`, `float`, and `double`.

This project was developed as a deep-dive learning exercise into C's memory management, data structures and documentation.
## Features

- **Generic Type Support:** Easily works with `int`, `float`, and `double`.
- **Dynamic Resizing:** Automatically grows in capacity as elements are added.
- **Full Suite of Operations:**
  - Creation/Destruction: `array_new`, `array_delete`
  - Modification: `append`, `pop`, `insert`, `set`, `clear`
  - Removal: `remove_by_index`, `remove_by_value`
- **Memory Management:** Includes `shrink` to optimize memory usage.
- **Getter:** Provides "getter" functions for size and capacity (`array_get_size`, `array_get_capacity`).
- **Algorithms Included:**
  - Linear Search (`array_find`)
  - In-place Reversal (`array_reverse`)
  - Sorting (`array_sort` using `qsort`)
  - Binary Search (`array_binary_search`)

## Getting Started

Follow these instructions to compile and run the demonstration program.

### Prerequisites

- A C compiler (like `gcc` or `clang`)
- The `make` build automation tool

### Building

From the project's root directory, simply run `make`:

```bash
make
