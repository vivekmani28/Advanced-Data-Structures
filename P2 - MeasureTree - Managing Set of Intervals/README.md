# Project 2

## Measure Tree Implementation for managing set of Intervals


The goal of the program is to implement the measure tree as described in The paper(attached) by Gunno et al. The measure tree is a dynamic structure that maintains a system of intervals under insertion and deletion, and can answer the query: give the total length of the union of the current intervals.

The structure support the following operations

- m_tree_t * create_m_tree() creates an empty measure tree.
- void insert_interval(m_tree_t * tree, int a, int b) inserts the interval [a,b[.
- void delete_interval(m_tree_t * tree, int a, int b) deletes the interval [a,b[, if it exists.
- int query_length(m_tree_t * tree) returns the length of the union of all intervals in the current set.
- void destroy_m_tree(m_tree_t *tree) destroys the tree and releases the memory.

