# Network Routing and Failure Recovery Simulator

A terminal-based network simulator written in **C**. It models routers and links as a weighted graph, finds the cheapest routes with **Dijkstra's algorithm**, forwards packets hop by hop using per-router routing tables, and recomputes routes automatically when a link or router fails.

> **Course project:** Software Project Lab 1 (SPL-1), 3rd semester
> **Status:** Work in progress. See the [Roadmap](#roadmap) for what is done and what is planned.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [How It Works](#how-it-works)
- [Project Structure](#project-structure)
- [Build and Run](#build-and-run)
- [Example Output](#example-output)
- [Roadmap](#roadmap)
- [Author](#author)

---

## Overview

Real networks have many routers, many possible paths, and links that sometimes fail. This project simulates that in a small, easy-to-inspect form:

- **Routers** are nodes in a graph.
- **Links** are weighted edges between routers (the weight is the link cost).
- **Packets** travel from a source router to a destination router, one hop at a time.
- **Routing tables** tell each router which neighbor to forward to next.
- **Failures** (a link or a router going down) trigger recalculation so traffic can take a new path.

Everything runs in the terminal with no external libraries.

## Features

Planned features (tick them off as they are completed):

**Network management**
- [ ] Add and remove routers
- [ ] Add and remove links with a cost
- [ ] Display the network (routers, links, costs, status)
- [ ] Input validation (duplicate names, self-links, invalid costs)

**Routing**
- [ ] BFS / DFS for reachability and connectivity checks
- [ ] Dijkstra's shortest path
- [ ] Routing table for every router (destination, next hop, cost)

**Simulation**
- [ ] Send a packet hop by hop with a step-by-step trace
- [ ] TTL (Time To Live) to stop endless forwarding
- [ ] Delivery statistics (sent, delivered, dropped, average hops and cost)

**Failure and recovery**
- [ ] Fail and restore a link
- [ ] Fail and restore a router
- [ ] Automatic route recalculation
- [ ] Warning when a failure splits the network into disconnected parts

**Persistence**
- [ ] Save the network to a file and load it back

## How It Works

### Graph model

The network is stored as an undirected, weighted graph using an adjacency matrix:

- `cost[a][b]` holds the cost of the link between routers `a` and `b` (`0` means no link).
- `link_up[a][b]` records whether the link is currently working, so a failed link keeps its cost and can be restored later.
- Each router has a status (UP or DOWN).

### Routing

1. **Dijkstra's algorithm** runs from every router to find the cheapest path to all others. Failed links and DOWN routers are skipped.
2. The result is stored as a **routing table** per router: for each destination, the next hop and the total cost.

### Forwarding

A packet looks up the destination in the current router's table, moves to the next hop, and repeats until it arrives. It is dropped if there is no route, if its TTL reaches 0, or if the next link is down.

### Failure recovery

When the topology or a status changes, the simulator rebuilds all routing tables so later packets use the new best paths.

### Algorithms and data structures

| Concept | Used for |
|---|---|
| Adjacency matrix | Storing routers and links |
| Dijkstra | Cheapest path and routing tables |
| BFS / DFS | Reachability and connectivity checks |
| Queue (array-based) | BFS traversal |
| Structs and enums | Routers, packets, routing entries, packet status |
| File I/O | Saving and loading networks |

## Project Structure

```
network-routing-simulator/
├── main.c            # Menu and program flow
├── graph.h / graph.c # Router, Link, Network structs; add/remove/display
├── traversal.c       # BFS and DFS
├── dijkstra.c        # Shortest path algorithm
├── routing_table.c   # Build and print routing tables
├── packet.c          # Packet forwarding simulation
├── failure.c         # Fail/restore routers and links
├── file_io.c         # Save and load network files
├── stats.c           # Delivery statistics
├── README.md
└── .gitignore
```

> Not every file exists yet. The project is built in stages (see the roadmap).

## Build and Run

**Requirements:** a C compiler such as GCC.

```bash
# Clone the repository
git clone https://github.com/YOUR-USERNAME/network-routing-simulator.git
cd network-routing-simulator

# Compile (list the .c files that exist at the moment)
gcc -Wall -Wextra -o simulator main.c graph.c

# Run
./simulator          # Linux / macOS / Git Bash
simulator.exe        # Windows (Command Prompt)
```

## Example Output

The following shows the intended behavior for a 5-router network.

Links: R1-R2 (2), R2-R3 (3), R1-R3 (4), R1-R4 (6), R3-R5 (1), R4-R5 (2)

**Normal network, packet from R1 to R5:**

```
Packet 1: R1 -> R5 (TTL 16)
  hop 1: R1 -> R3 (link cost 4, TTL left 15)
  hop 2: R3 -> R5 (link cost 1, TTL left 14)
Status: DELIVERED | hops: 2 | total cost: 5
```

**After link R3-R5 fails and the tables are rebuilt:**

```
Packet 2: R1 -> R5 (TTL 16)
  hop 1: R1 -> R4 (link cost 6, TTL left 15)
  hop 2: R4 -> R5 (link cost 2, TTL left 14)
Status: DELIVERED | hops: 2 | total cost: 8
```

## Roadmap

- [ ] **Stage 1:** Project structure, structs, add/display routers and links (SPL-1 mid)
- [ ] **Stage 2:** BFS/DFS and connectivity checks
- [ ] **Stage 3:** Dijkstra and routing tables
- [ ] **Stage 4:** Packet simulation with TTL
- [ ] **Stage 5:** Failure handling and automatic recovery
- [ ] **Stage 6:** Save/load, statistics, testing, and final report

**Possible extras:** Bellman-Ford comparison, per-hop delay, random packet loss, Graphviz export for diagrams.

## Author

**ABID HASAN**
Roll: 1748
Software Engineering, DHAKA UNIVERSITY
Supervisor: Dr. Ahmedul Kabir


*Built for Software Project Lab 1.*
