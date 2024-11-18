import numpy as np
import sympy as sp
import random


def generate_random_scalar_matrix(height, width, rank, seed):
    
    """Refer https://math.stackexchange.com/questions/757671/generate-some-random-matrix-with-given-rank"""
    min_after_zero_in_matrix = 0
    max_in_matrix = 2
    if rank > min(width, height):
        print("ERRORRRRRRRRR")
        return
    P = np.zeros(shape=(height, width), dtype=int)
    for i in range(rank):
        P[i][i] = 1
    #print(P)
    U = np.random.randint(min_after_zero_in_matrix, max_in_matrix+1, size=(height, width))
    while np.linalg.matrix_rank(U) == 0:  # Generating invertible matrix (ie. rank>0)
        U = np.random.randint(min_after_zero_in_matrix, max_in_matrix+1, size=(height, width))
    #print(U)
    V = np.random.randint(min_after_zero_in_matrix, max_in_matrix+1, size=(height, width))
    while np.linalg.matrix_rank(V) == 0:  # Generating invertible matrix (ie. rank>0)
        V = np.random.randint(min_after_zero_in_matrix, max_in_matrix+1, size=(height, width))
    #print(V)
    matrix = np.dot(U, P)
    matrix = np.dot(matrix, V)
    #print(matrix)
    #print(np.linalg.matrix_rank(matrix))
    return matrix.tolist()
    #return matrix


def generate_random_matrix(m, n, matrix, seed):
    
    num_vars = m * n
    symbol_pool = sp.symbols(f"x0:{num_vars}")
    index = 0

    symbolic_matrix = [row[:] for row in matrix]

    for i in range(m):
        for j in range(n):
            if random.random() < 0.05:
                symbolic_matrix[i][j] = symbol_pool[index]
                index += 1

    return sp.Matrix(symbolic_matrix)


def increase_independence(A, A_symbolic, seed):
    Vr = set(range(m))
    Vc = set(range(n))
    Ac = set()
    print("\nAc = ")
    print(Ac)
    print("\nVr = ")
    print(Vr)
    print("\nVc = ")
    print(Vc)
    og_matrix = A
    og_rank = np.linalg.matrix_rank(og_matrix)
    avd_rows = avoidable_rows(A)
    avd_cols = avoidable_cols(A)
    print("\nAvd rows = ")
    print(avd_rows)
    print("\nAvd cols = ")
    print(avd_cols)
    col_iterate = Vc - avd_cols
    print("\ncol_iterate = ")
    print(col_iterate)
    for i in col_iterate:
        if avoidable_rows(np.delete(A, i, axis=1)) == avd_rows:
            Ac.add(i)
    #print("Ac:", Ac)
    print("\nAc = ")
    print(Ac)
    cols = Vc - Ac
    #print("rows", avd_rows)
    #print("cols", cols)

    for i in avd_rows:
        for j in cols:
            if isinstance(A_symbolic[i, j], sp.Symbol):
                print ("\n")
                print("i j : ", i, j)
                for value in range(1, m + 2):
                    temp = og_matrix.copy()
                    temp[i, j] = value
                    print(temp)
                    print("\nrank new = ", np.linalg.matrix_rank(temp))
                    print("\nog rank ", og_rank)
                    if greater_independence(og_matrix,temp):
                        print("independence increased")
                        return temp
                print("Value not found")

    #print("now cols")
    # for i in cols:
    #     for j in range(0, n):
    #         if isinstance(A_symbolic[j, i], sp.Symbol):
    #             for value in range(1, m + 2):
    #                 temp = og_matrix.copy()
    #                 temp[j, i] = value
    #                 if np.linalg.matrix_rank(temp) > og_rank:
    #                     #print("independence increased")
    #                     return temp
    #                 elif np.linalg.matrix_rank(temp) == og_rank and avoidable_rows(temp) < avoidable_rows(og_matrix):
    #                     #print("independence increased")
    #                     return temp

    #print("No increase in independence")
    return A


def avoidable_rows(matrix):
    og_rank = np.linalg.matrix_rank(matrix)
    avoidable_rows = set()
    for i in range(0, m):
        curr_matrix = np.delete(matrix, i, axis=0)
        curr_rank = np.linalg.matrix_rank(curr_matrix)
        if curr_rank == og_rank:
            avoidable_rows.add(i)
    return avoidable_rows


def avoidable_cols(matrix):
    og_rank = np.linalg.matrix_rank(matrix)
    avoidable_cols = set()
    for i in range(0, n):
        curr_matrix = np.delete(matrix, i, axis=1)
        curr_rank = np.linalg.matrix_rank(curr_matrix)
        if curr_rank == og_rank:
            avoidable_cols.add(i)
    return avoidable_cols


def greater_independence(A1, A2):
    if np.linalg.matrix_rank(A1) < np.linalg.matrix_rank(A2):
        return True
    if avoidable_rows(A1) < avoidable_rows(A2) and np.linalg.matrix_rank(A1) == np.linalg.matrix_rank(A2):
        return True
    
    return False


def randomized_algorithm(m, n, matrix, pass_num, seed):
    
    max_rank = 0
    for i in range(pass_num):
        print(i)
        temp_matrix = matrix.subs({s: random.randint(1, 10) for s in matrix.free_symbols})
        temp_matrix = np.array(temp_matrix, dtype=float)
        #print("Pass ", i)
        #print(temp_matrix)
        rank = np.linalg.matrix_rank(temp_matrix)
        #print("Rank= ", rank)
        if max_rank < rank:
            max_rank = rank
        if max_rank == min(m, n):
            break
    return max_rank


def deterministic_algorithm(matrix, symbolic_matrix, seed):
    passes = 1
    new_matrix = increase_independence(matrix, symbolic_matrix, seed)
    print(new_matrix)
    while greater_independence(matrix, new_matrix):
        matrix = new_matrix
        new_matrix = increase_independence(matrix, symbolic_matrix, seed)
        passes += 1
        print(new_matrix)
        print("\n\n----------------------------------------------------\n\n")
    return np.linalg.matrix_rank(np.array(new_matrix, dtype=float)), passes


num_tests = 1  # Num test cases
m = 15  # rows
n = 15  # columns
seed = 90

random.seed(seed)
np.random.seed(seed)

# Open a file to save the results
with open("matrix_rank_results.txt", "w") as f:
    for test_num in range(1, num_tests + 1):
        # matrix = [
        #     [1, 2, 3],
        #     [4, 5, 6],
        #     [7, 8, 9]
        # ]

        matrix = generate_random_scalar_matrix(m, n, 2, seed)
        symbolic_matrix = generate_random_matrix(m, n, matrix, seed)
        matrix = np.array(matrix, dtype=float)

        # Write symbolic matrix to file
        f.write(f"Test #{test_num}\nSymbolic Matrix:\n")
        f.write(str(symbolic_matrix) + "\n")
        f.write("Variables = " + str(len(symbolic_matrix.free_symbols)) + "\n\n")

        randomized_rank = randomized_algorithm(m, n, symbolic_matrix, 10, seed)
        f.write(f"Rank(Randomized) = {randomized_rank}\n\n")

        f.write("Initial Substitution:\n")
        f.write(str(matrix) + "\n")

        deterministic_rank, passes = deterministic_algorithm(matrix, symbolic_matrix, seed)
        f.write(f"Rank(Deterministic) = {deterministic_rank}\n")
        f.write(f"Passes: {passes}\n")

        if randomized_rank == deterministic_rank:
            f.write("\nRanks SAME\n")
        else:
            f.write("Ranks DIFFERENT\n")
        f.write("\n----------------------------------------------------------------------------\n\n")

print("Results saved to 'matrix_rank_results.txt'")
