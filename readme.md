Implementation of Link-State Routing Protocol (CS 542 Term Project, Fall 2013)

Due date: noon, 11/25/2013.

1. Overview
	Link-state routing protocol is a main class of routing protocols. It is performed by every router (switching node) in the network. The base concept of link-state routing is that every node constructs a map of the connectivity to the network, showing which nodes are connected to which other nodes. Each node then independently calculates the next best logical path from it to every possible destination in the network. The collection of best paths will then form the node's routing table.

2. Objective
	In this term project, you are asked to develop a program to implement Link-State Routing Protocol. Your program should have two functions:

	1) Simulate the process of generating routing tables for each router in a given network,
	2) Compute optional path with least cost between any two specific routers.

3. Problem Description
	Suppose we have a network with arbitrary number of routers. The network topology is given by a matrix, called the original routing table, which only indicatesthe costs of links between all directly connected routers. We assume each router only knows its own information and has no knowledge about others at the beginning.
	In this project, to implement Link-State Routing Protocol, first your program is required to create the state of the links by each router after the input file containing the network information been loaded.This is called the link state packet or LSP. Then, the program need to flood LSPs of each router to all other routers and build the routing table for each router. A Dijkstra's algorithm could be applied to find shortest path tree. Finally, your program should be able to output the routing table of any router, and output the optimal path between any two selected routers.
