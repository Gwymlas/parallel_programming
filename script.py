import numpy as np


def read_file(path):
    matrix = []
    with open(path, 'r') as f:
        for line in f.readlines():
            row_string = line.split()
            row = list(map(int, row_string))
            matrix.append(row)
    return matrix


def multiplication_matrix(first_matrix, second_matrix):
    count_row = len(first_matrix)
    count_columns = len(second_matrix[0])
    matrix = []
    for i in range(count_row):
        matrix.append([0]*count_columns)

    for i in range(count_row):
        for j in range(count_columns):
            for k in range(len(first_matrix[i])):
                matrix[i][j] += first_matrix[i][k] * second_matrix[k][j]
    return matrix


def using_numpy():
    a = np.array(read_file("/Users/gwymlas/Desktop/1.txt"))
    b = np.array(read_file("/Users/gwymlas/Desktop/2.txt"))

    result = a.dot(b)
    print("Multiplication matrix using numpy")
    print(result)

    c_matrix = np.array(read_file("/Users/gwymlas/Desktop/result.txt"))
    print("Multiplication matrix using c++")
    print(c_matrix)

    print("Checking multiplication")
    print(np.array_equal(c_matrix, result))


def main():
    using_numpy()
    # a = read_file("/Users/gwymlas/Desktop/1.txt")
    # b = read_file("/Users/gwymlas/Desktop/2.txt")

    # c = multiplication_matrix(a, b)


if __name__ == "__main__":
    main()
