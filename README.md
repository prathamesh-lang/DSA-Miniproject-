# Music Playlist Management System

A menu-driven C++ console application implementing a dynamic music playlist using a **Doubly Linked List (DLL)**. Developed as a second-year Computer Engineering Data Structures mini-project.

---

## 1. Project Objective

In real-world music applications, playlists change dynamically as songs are added, deleted, reordered, and navigated. Traditional array-based structures suffer from fixed capacities and expensive $O(n)$ element-shifting during insertions and deletions.

The objective of this project is to implement an efficient, dynamic music playlist manager where:
- Each song is stored as an independently allocated node in memory.
- Forward and backward playback navigation is natively supported via bidirectional pointers.
- Insertions and deletions do not require memory shifting.
- The system remains robust against edge cases (empty playlist, boundary navigation, duplicate IDs, invalid inputs).

---

## 2. Data Structure & Pointer Architecture

### Song Node Structure
Each song is represented by an allocated `Song` node:
- `int songID`: Unique identifier for each song.
- `std::string title`: Title of the song.
- `std::string artist`: Name of the performing artist.
- `int duration`: Track duration in seconds.
- `Song* prev`: Pointer referencing the preceding song node (`nullptr` if first node).
- `Song* next`: Pointer referencing the succeeding song node (`nullptr` if last node).

### Playlist State Pointers
The list maintains three essential pointers:
1. `head`: Points to the first song in the playlist. If empty, `head == nullptr`.
2. `tail`: Points to the last song in the playlist. If empty, `tail == nullptr`.
3. `current`: Tracks the currently active/playing song. If empty, `current == nullptr`.

```text
       NULL <--- [Song 1] <===> [Song 2] <===> [Song 3] ---> NULL
                   ^              ^              ^
                  head         current          tail
```

### Doubly Linked List Invariants
- If empty: `head == nullptr`, `tail == nullptr`, `current == nullptr`.
- First node: `head->prev == nullptr`.
- Last node: `tail->next == nullptr`.
- Bidirectional linkage: If `A->next == B`, then `B->prev == A`.
- Safe selection: `current` always points to a valid remaining node, or `nullptr` if the playlist becomes empty.

---

## 3. Core Operations

| Operation | Description | Edge Cases Handled |
| :--- | :--- | :--- |
| **1. Add Song** | Appends a new song to the end of the playlist. | Rejects duplicate IDs and durations $\le 0$. If list is empty, sets `head = tail = current = newNode`. Otherwise appends to `tail` and keeps existing `current` active. |
| **2. Delete Song** | Finds a song by ID and removes it from the list. | Handles deleting only node, head node, tail node, middle node, or active song (reassigns `current` to next song, or previous if next unavailable). Reclaims memory with `delete`. |
| **3. Search Song** | Searches by Song ID or Song Title. | Case-insensitive title search. Safe message if song not found. Displays track details and playing status. |
| **4. Display Playlist** | Traverses from `head` to `tail` printing all songs. | Displays formatted table, total songs, total duration, and marks the active song with `-> [CURRENT]`. |
| **5. Next Song** | Advances `current` pointer to `current->next`. | Non-circular: If already at `tail`, reports boundary and maintains selection. |
| **6. Previous Song** | Moves `current` pointer back to `current->prev`. | Non-circular: If already at `head`, reports boundary and maintains selection. |
| **7. Exit** | Traverses and deallocates every node before terminating. | Prevents memory leaks; resets pointers to `nullptr`. |

---

## 4. Time and Space Complexity

| Operation | Time Complexity | Rationale |
| :--- | :---: | :--- |
| **Next Song** | $\mathcal{O}(1)$ | Direct pointer access via `current->next`. |
| **Previous Song** | $\mathcal{O}(1)$ | Direct pointer access via `current->prev`. |
| **Append Song (at tail)** | $\mathcal{O}(1)^*$ | Appending at the maintained `tail` pointer is constant time ($^*$checking uniqueness is $\mathcal{O}(n)$). |
| **Search (by ID or Title)** | $\mathcal{O}(n)$ | Linear scan traversing from `head` across $n$ nodes. |
| **Display Playlist** | $\mathcal{O}(n)$ | Full traversal visiting each of the $n$ nodes once. |
| **Delete Song (by ID)** | $\mathcal{O}(n)$ | Finding the target node requires $\mathcal{O}(n)$ traversal; relinking pointers once found is $\mathcal{O}(1)$. |
| **Exit / Clear Memory** | $\mathcal{O}(n)$ | Traverses and frees all $n$ allocated nodes. |
| **Space Complexity** | $\mathcal{O}(n)$ | Stores $n$ song nodes in heap memory, each holding data and two pointers (`prev` and `next`). |

---

## 5. Prerequisites

- Operating System: Windows, Linux, or macOS.
- Compiler: Any C++17 compatible compiler (e.g., GCC `g++`, Clang `clang++`, or MSVC `cl.exe`).

---

## 6. Compilation and Execution

### Using GCC (MinGW / Linux / macOS)

**Compilation:**
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o playlist
```

**Execution:**
- **Windows (PowerShell / Command Prompt):**
  ```powershell
  .\playlist.exe
  ```
- **Linux / macOS:**
  ```bash
  ./playlist
  ```

---

## 7. Project Information

- **Course:** Data Structures (SE Computer Engineering)
- **Academic Year:** 2026-2027
- **Team Name:** Torvalds
- **Students:**
  - Prathamesh Patil (Roll No. 48, PRN: U251A189)
  - Omkar Rane (Roll No. 50, PRN: U251A194)
- **Project Guide:** Prof. Vaishali Dhawas
- **Institution:** Department of Computer Engineering, Nutan Maharashtra Institute of Engineering & Technology (NMIET), Pune
