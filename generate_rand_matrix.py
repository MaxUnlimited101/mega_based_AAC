import random

def generate_random_matrix(size):
    return [[random.randint(1, 9) for _ in range(size)] for _ in range(size)]

with open("input.txt", "w") as f:
    G_size = 80
    random_matrix = generate_random_matrix(G_size)
    f.write(f"{G_size}\n")
    for row in random_matrix:
        f.write(" ".join(map(str, row)) + "\n")
    
    H_size = 100
    random_matrix = generate_random_matrix(H_size)
    f.write(f"{H_size}\n")
    for row in random_matrix:
        f.write(" ".join(map(str, row)) + "\n")
    
    