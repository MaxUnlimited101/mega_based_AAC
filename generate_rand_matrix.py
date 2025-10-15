#!/usr/bin/env python3
import random

def generate_random_matrix(size):
    return [[random.randint(0, 9) for _ in range(size)] for _ in range(size)]

with open("input.txt", "w") as f:
    G_size = 5
    random_matrix = generate_random_matrix(G_size)
    f.write(f"{G_size}\n")
    for row in random_matrix:
        f.write(" ".join(map(str, row)) + "\n")
    
    H_size = 10
    random_matrix = generate_random_matrix(H_size)
    f.write(f"{H_size}\n")
    for row in random_matrix:
        f.write(" ".join(map(str, row)) + "\n")
    
    