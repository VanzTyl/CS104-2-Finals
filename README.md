# Soccer Seat Tickets Refund Management System

A **C++ console-based simulation** that models a soccer tournament ticketing and refund management system while demonstrating the practical use of fundamental **data structures and algorithms**.

The application allows users to configure tournament events, simulate ticket purchases, generate refund requests, review refunds, and monitor organizer performance through a reputation system. It also includes persistent save/load functionality, making it suitable as an educational project for learning core computer science concepts.

## Features

* ⚽ Configure soccer tournament events with multiple tournament formats
* 🎟️ Simulate customer ticket purchases for randomly generated matches
* 🔄 Process refund requests using a **FIFO Queue**
* 🌳 Manage regional venue categories using a **Binary Search Tree (BST)**
* 🔍 Search purchase records efficiently using **Binary Search**
* 📊 View detailed statistics including revenue, ticket sales, refund history, and organizer reputation
* 📅 Simulate multiple business days with daily summary reports
* 💾 Save and load simulation progress using file handling
* 🖥️ Interactive console interface with Unicode box-drawing menus

## Data Structures Used

* **Structures (Structs)** for organizing complex records
* **Arrays** for storing events, purchases, matches, and refund requests
* **Queue (Circular Queue)** for refund processing
* **Binary Search Tree (BST)** for regional venue hierarchy
* **Selection Sort** for organizing purchase records
* **Binary Search** for fast purchase lookup

## Programming Concepts Demonstrated

* Object-like programming using C-style structures
* Nested structures
* Arrays of structures
* File input/output (Save & Load system)
* Dynamic memory allocation (`new` / `delete`)
* Random data generation
* Menu-driven console application
* Cross-platform console support (Windows UTF-8 compatibility)

## Technologies

* **Language:** C++
* **Libraries:** iostream, fstream, iomanip, cstring, cstdlib, ctime
* **Platform:** Console Application (Windows/Linux compatible)

## Purpose

This project was developed as an educational simulation to demonstrate how fundamental data structures and algorithms can be applied to a real-world ticket management scenario. It combines event management, refund processing, searching, sorting, file persistence, and statistics into a single interactive console application.
