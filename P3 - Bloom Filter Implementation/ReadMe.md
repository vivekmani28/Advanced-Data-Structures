# Project 3

## Bloom Filter Implementation


This is a implementation for a structure that maintains a Bloom Filter for 2,000,000 strings with an error rate of 0.05%, using only 2Mbyte of memory. To achieve this, we have eight bit arrays, each of 2,000,000 bits(this is, 250,000 char). For each of these, we have a random hash function from the universal family(hi).

To insert a string s, we set the hi(S)-th bit to one in the i-th bit array, for i = 0,…,7. To query whether a string q is contained in the set, you check whether hi(q) is one in the i-th bit array, for all i.

The structure support the following operations
- bf_t * create_bf() creates a Bloom filter with the above specification.
- void insert_bf(bf_t *b, char *s) inserts the string *s into the Bloom filter *b.
- int is_element(bf_t *b, char *q) returns 1 if the string *q is accepted by the Bloom filter, and 0 else.
