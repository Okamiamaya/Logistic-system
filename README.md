# Logistic System

A command-line laboratory inventory management system, built to solve a real problem: spending too much time hunting for a single supply in a disorganized lab.

## About

Hello, I'm Okamiamaya. I often build robots on weekends, and while working on them, I found myself wasting significant amounts of time searching for a single specific supply. The problem stemmed from the sheer diversity of items in my laboratory, which were also frequently left unorganized.

To address this, I built **Logistic System** — a tool to manage laboratory inventory the way a logistics system manages shipments: items are stored in structured, trackable locations rather than scattered arbitrarily.

This is a student project. While I have some background in competitive programming, I had no prior experience building a full system before starting this. Through studying for **AP Computer Science Principles (AP CSP)** and **AP Computer Science A (AP CSA)**, I developed a stronger foundation in robustness, UI/UX design, and object-oriented concepts, which I applied while building the second version. For the third version, my focus shifted to improving algorithmic efficiency and further refining the user experience.

## Features

- Organize supplies into lab categories, with each lab tracking its own items and quantities
- Add, remove, and search supplies across the entire system or within a specific lab
- Automatic handling of "unclassified" supplies — items added without a known lab are held safely and can be sorted in later, instead of being lost
- Global and per-lab views of everything currently in stock
- Input validation with clear success/failure feedback on every command
- Dual command entry: type either a command's name or its numeric shortcut

## Getting Started

### Requirements

- A C++ compiler that supports C++11 or later (e.g. `g++`)

### Build

```bash
g++ -O2 -o logistic_system Logistic_System_V3.cpp
```

### Run

```bash
./logistic_system
```

The system starts by printing the full list of available commands. Type `Help` (or `13`) at any time to see it again.

## Usage

Commands are entered as either their name or numeric shortcut, followed by any required arguments on the following lines when prompted.

| # | Command | Arguments | Description |
|---|---------|-----------|-------------|
| 1 | `addLab` | `lab_name` | Create a new lab category |
| 2 | `removeLab` | `lab_name` | Remove a lab; its supplies are moved to unclassified data |
| 3 | `searchLab` | — | List all lab categories |
| 4 | `searchLabClassification` | `lab_name` | List all supplies within a specific lab |
| 5 | `searchAllLabClassification` | — | List all supplies across every lab |
| 6 | `searchSupplyClassification` | — | List all distinct supply types in the system |
| 7 | `addSupply` | `supply_name`, `lab_name`, `amount` | Add supply quantity to a lab |
| 8 | `removeSupply` | `supply_name`, `lab_name`, `amount` | Remove supply quantity from a lab |
| 9 | `searchSupply` | `supply_name` | Show which labs carry a given supply, and how much |
| 10 | `searchUnclassifiedData` | — | List supplies not yet assigned to a lab |
| 11 | `deleteUnclassifiedData` | — | Clear all unclassified data |
| 12 | `classifyData` | `supply_name`, `lab_name` | Assign an unclassified supply to a lab |
| 13 | `Help` | — | Reprint the full command list |
| 14 | `End the system` | — | Exit the program |

### Example session

```
> addLab
Chemistry Lab

> addSupply
Beaker
Chemistry Lab
25

> searchLabClassification
Chemistry Lab
1. Beaker 25
```

## Design Notes

- Commands are matched using a lightweight rolling hash rather than string comparison, so both full command names and numeric shortcuts resolve to the same action.
- Supplies added to a lab that doesn't yet exist are automatically routed to unclassified data instead of being rejected or lost, keeping the workflow forgiving for quick, out-of-order data entry.
- All operations report a clear success or failure message, so the state of the system is never ambiguous after a command runs.

## Version History

- **V1** — Initial working prototype
- **V2** — Introduced structured commands and object-based organization
- **V3** — Focused on efficiency improvements and UX polish: input validation, consistent feedback messaging, and safer lookup patterns

## Author

**Okamiamaya**
