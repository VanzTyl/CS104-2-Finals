# ⚽ Soccer Seat Tickets Refund Management System

A **C++ console-based simulation** that models the operations of a soccer tournament ticketing and refund management system while demonstrating the practical application of fundamental **data structures and algorithms**.

Developed as an educational project, this application allows users to create tournament events, simulate ticket purchases, process customer refund requests, monitor organizer reputation, generate tournament standings, and persist simulation data through save and load functionality.

---

# 📌 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Data Structures & Algorithms](#-data-structures--algorithms)
- [Function Overview](#-function-overview)
- [Programming Concepts Demonstrated](#-programming-concepts-demonstrated)
- [Technologies Used](#-technologies-used)
- [Project Structure](#-project-structure)
- [Purpose](#-purpose)

---

# 📖 Overview

The Soccer Seat Tickets Refund Management System simulates the responsibilities of a soccer tournament organizer. Users can configure tournament events, generate ticket purchases, manage refund requests, maintain organizer reputation, and monitor tournament standings.

Aside from simulating a real-world workflow, the project emphasizes the implementation of classic data structures and algorithms required in introductory Data Structures and Algorithms courses.

---

# ✨ Features

### Tournament Management
- Create soccer tournament events
- Support for multiple tournament formats:
  - Round Robin
  - Winner & Losers Bracket (Double Elimination)
  - Single Elimination
- Manual or automatic event creation
- Manual or automatic team selection

### Ticket Management
- Simulate customer ticket purchases
- Random customer and ticket generation
- Adjustable ticket pricing and purchase generation

### Refund Management
- Generate refund requests
- Review refund requests in FIFO order
- Approve or reject refund requests
- Expired refund tracking
- Refund history records

### Tournament Standings
- Automatic computation of:
  - Wins
  - Losses
  - Draws
  - Points
  - Goals For (GF)
  - Goals Against (GA)
- Leaderboard generated using a Binary Search Tree

### Statistics
- Revenue tracking
- Tickets sold
- Refund statistics
- Organizer reputation
- Daily summary reports

### Persistence
- Save simulation progress
- Load previous simulations
- Multiple save files supported

### Console Interface
- Unicode box-drawing UI
- Menu-driven navigation
- Cross-platform console support

---

# 📚 Data Structures & Algorithms

## Structures (Structs)

Structures organize related information into reusable records.

### Structures Used

| Structure | Purpose |
|-----------|---------|
| `Date` | Stores purchase dates |
| `Match` | Stores match information and scores |
| `Event` | Stores tournament information |
| `Purchase` | Stores ticket purchase records |
| `RefundRequest` | Stores refund request information |
| `BSTNode` | Stores tournament standings in a Binary Search Tree |

### Demonstrated In

- `configureEvents()`
- `generateTicketPurchases()`
- `generateRefundRequests()`
- `reviewRefundRequests()`
- `computeStandingsForEvent()`

---

## Arrays

Arrays serve as the primary storage containers for the simulation.

### Arrays Used

| Array | Purpose |
|--------|---------|
| `masterPurchases[]` | Purchase database |
| `refundQueueArray[]` | Refund queue |
| `activeEvents[]` | Active tournaments |
| `completedEvents[]` | Finished tournaments |
| `expiredRefunds[]` | Expired refund requests |
| `countries[]` | Team selection |
| `firstNames[]` | Customer generation |
| `lastNames[]` | Customer generation |

### Demonstrated In

- `configureEvents()`
- `generateTicketPurchases()`
- `generateRefundRequests()`
- `showAllPurchases()`
- `computeStandingsForEvent()`

---

## Circular Queue (FIFO)

Refund requests are processed using a **First-In, First-Out (FIFO)** circular queue.

### Queue Operations

| Function | Description |
|----------|-------------|
| `enqueueRefund()` | Adds a refund request to the queue |
| `dequeueRefund()` | Removes the oldest refund request |
| `isQueueFull()` | Checks queue capacity |
| `isQueueEmpty()` | Checks if queue contains requests |

### Demonstrated In

- `generateRefundRequests()`
- `reviewRefundRequests()`
- `viewRefundQueue()`

---

## Binary Search Tree (BST)

The Binary Search Tree stores tournament standings.

Each node stores:

- Team Name
- Wins
- Losses
- Draws
- Points
- Goals For
- Goals Against

### BST Functions

| Function | Description |
|----------|-------------|
| `createBSTNode()` | Creates a new BST node |
| `insertBSTNode()` | Inserts a team into the BST |
| `inOrderTraversal()` | Displays standings in descending order |
| `computeStandingsForEvent()` | Computes team statistics |
| `freeBST()` | Frees allocated BST memory |

---

## Selection Sort

Purchase records are sorted before Binary Search.

### Function

```cpp
selectionSortPurchasesByID()
```

Purpose:

- Sorts purchases by Purchase ID
- Enables efficient Binary Search

---

## Binary Search

Binary Search is demonstrated in two different features.

### Purchase Search

Function:

```cpp
binarySearchPurchaseByID()
```

Used in:

- `searchPurchaseRecord()`
- `reviewRefundRequests()`

Purpose:

- Searches purchase records by Purchase ID

---

### Country Search

Function:

```cpp
searchCountriesByPrefix()
```

Used in:

- `selectTeamsManual()`
- `configureEvents()`

Purpose:

- Searches the alphabetically sorted country list using a prefix.

Example:

```
Input:
Ang

Output:
Angola
```

---

# 📋 Function Overview

## Event Management

| Function | Purpose |
|----------|---------|
| `configureEvents()` | Creates tournament events |
| `selectTeamsManual()` | Manual team selection |
| `computeStandingsForEvent()` | Generates standings |

---

## Purchase Management

| Function | Purpose |
|----------|---------|
| `generateTicketPurchases()` | Generates purchases |
| `showAllPurchases()` | Displays purchases |
| `searchPurchaseRecord()` | Searches purchase records |
| `selectionSortPurchasesByID()` | Sorts purchases |
| `binarySearchPurchaseByID()` | Searches purchases |

---

## Refund Management

| Function | Purpose |
|----------|---------|
| `generateRefundRequests()` | Generates refund requests |
| `reviewRefundRequests()` | Reviews refunds |
| `viewRefundQueue()` | Displays pending refunds |
| `viewRefundHistory()` | Displays processed refunds |
| `viewExpiredRefunds()` | Displays expired refunds |
| `searchRefundByID()` | Searches refund records |

---

## Queue Functions

| Function | Purpose |
|----------|---------|
| `enqueueRefund()` | Add refund |
| `dequeueRefund()` | Remove refund |
| `isQueueFull()` | Queue full check |
| `isQueueEmpty()` | Queue empty check |

---

## BST Functions

| Function | Purpose |
|----------|---------|
| `createBSTNode()` | Creates BST node |
| `insertBSTNode()` | Inserts node |
| `inOrderTraversal()` | Displays standings |
| `freeBST()` | Releases memory |

---

## Statistics

| Function | Purpose |
|----------|---------|
| `viewStatistics()` | Displays overall statistics |
| `endDaySummary()` | Shows daily report |

---

## Save & Load

| Function | Purpose |
|----------|---------|
| `saveGame()` | Saves simulation |
| `loadGame()` | Loads simulation |
| `saveAndExit()` | Save before exiting |

---

# 💡 Programming Concepts Demonstrated

- Modular Programming
- Menu-Driven Applications
- Arrays
- Arrays of Structures
- Nested Structures
- Circular Queue
- Binary Search Tree
- Dynamic Memory Allocation (`new` / `delete`)
- Selection Sort
- Binary Search
- Random Data Generation
- File Input/Output
- Cross-Platform Console Programming

---

# 🛠 Technologies Used

## Language

- C++

## Standard Libraries

- `<iostream>`
- `<iomanip>`
- `<fstream>`
- `<cstring>`
- `<cstdlib>`
- `<ctime>`

## Platform

- Windows
- Linux
- Console Application

---

# 📂 Project Structure

```
main.cpp
│
├── Tournament Management
├── Purchase Management
├── Refund Management
├── Queue Implementation
├── Binary Search Tree
├── Sorting & Searching Algorithms
├── Statistics
├── Save & Load System
└── Console User Interface
```

---

# 🎯 Purpose

The **Soccer Seat Tickets Refund Management System** was developed as an educational simulation project to demonstrate how classic **data structures and algorithms** can be applied to a practical software system.

The application combines tournament management, ticket sales, refund processing, statistics generation, searching, sorting, and persistent storage into a single interactive console program.

Through this project, the following computer science concepts are showcased:

- Structures and Nested Structures
- Arrays
- Circular Queues (FIFO)
- Binary Search Trees (BST)
- Selection Sort
- Binary Search
- Dynamic Memory Allocation
- File Handling
- Modular Programming

This project serves as a practical demonstration of how these concepts work together in a realistic management system rather than as isolated algorithm implementations.
