# Exact Pattern Matching

Exact Pattern Matching using various algorithms such as [Knuth–Morris–Pratt](https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm), [Boyer-Moore](https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_string-search_algorithm), and [Rabin-Karp](https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm).

# Aim

Program given two sequences T and P, determines whether P occurs exactly
within T , and if it does, the locations of P in T.

# Installation

```bash
$ git clone https://github.com/salmoor/exact-pattern-matching.git
$ cd exact-pattern-matching
$ make
```

# How to Use

**Input:** Two strings T and P , where |T | ≥ |P |. Two strings should be in
[FASTA](https://en.wikipedia.org/wiki/FASTA_format) format.

After the Installation step, you should have executable named ***emp***.

You can run the program in the following way:

```bash
$ ./emp text.fa pattern.fa
```

**Output:** For each of the four algorithms, program displays

- Whether P is in T , and if it is, the location of P within T (1-based coordinate).
- Number of character comparisons performed.
- Run time in microseconds.
- Algorithm that performed the best.