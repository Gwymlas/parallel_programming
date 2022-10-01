#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>


std::vector<std::vector<double>> MatrixMultiplication(const std::vector<std::vector<double>>& first_matrix,
                                                      const std::vector<std::vector<double>>& second_matrix){
    // (n,m)*(m,k) = (n,k)
    // count rows = n
    // count columns = k
    if (first_matrix[0].size() != second_matrix.size()) throw std::exception();

    size_t count_rows = first_matrix.size();
    size_t count_columns = second_matrix[0].size();
    std::vector<std::vector<double>> result(count_rows);
    for (int i = 0; i < count_rows; ++i) {
        result[i].resize(count_columns);
    }

    size_t m = second_matrix.size();

    for (int i = 0; i < count_rows; ++i) {
        for (int j = 0; j < count_columns; ++j) {
            for (int k = 0; k < m; ++k) {
                result[i][j] += first_matrix[i][k] * second_matrix[k][j];
            }
        }
    }

    return result;
}


std::vector<std::vector<double>> CreateRandomMatrix(size_t count_rows, size_t count_columns) {
    std::vector<std::vector<double>> result(count_rows);
    for (auto& i: result) {
        i.resize(count_columns);
    }

    for (int i = 0; i < count_rows; ++i) {
        for (int j = 0; j < count_columns; ++j) {
            result[i][j] = rand() % 10;
        }
    }

    return result;
}


void CreateFile(const std::vector<std::vector<double>>& matrix, const char* path) {
    std::ofstream out;
    out.open(path);
    if (out.is_open()) {
        std::cout << "OK" << std::endl;
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[i].size(); ++j) {
                out << std::setw(10) << matrix[i][j];
            }
            out << std::endl;
        }
    }
    out.close();
}


void ReadFile(std::vector<std::vector<double>>& matrix, size_t count_rows, size_t count_columns, const char* path) {
    matrix.resize(count_rows);
    for (auto& elem: matrix) {
        elem.resize(count_columns);
    }
    std::ifstream in;
    in.open(path);
    if (in.is_open()) {
        std::cout << "OK" << std::endl;
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[i].size(); ++j) {
                in  >> matrix[i][j];
            }
        }
    }
    in.close();
}


void PrintMatrix(const std::vector<std::vector<double>>& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


int main() {
    srand(time(0));

    size_t n = 1000;
    size_t m = 1000;
    size_t k = 1000;


    auto first = CreateRandomMatrix(n, m);
    auto second = CreateRandomMatrix(m, k);

    CreateFile(first, "/Users/gwymlas/Desktop/1.txt");
    CreateFile(second, "/Users/gwymlas/Desktop/2.txt");

    //std::vector<std::vector<double>> first, second;

    //ReadFile(first, n, m, "/Users/gwymlas/Desktop/1.txt");
    //ReadFile(second, m, k, "/Users/gwymlas/Desktop/2.txt");

    clock_t start = clock();

    auto result = MatrixMultiplication(first, second);
    CreateFile(result, "/Users/gwymlas/Desktop/result.txt");

    clock_t end = clock();

    //PrintMatrix(result);

    std::cout << "Time of multiplication: " << (double) (end-start) / CLOCKS_PER_SEC << std::endl;
    std::cout << "First matrix: ("  << n << ", " << m << ")" << std::endl;
    std::cout << "Second matrix: (" << m << ", " << k << ")"<< std::endl;
    std::cout << "Result matrix: (" << n << ", " << k << ")" << std::endl;

    return 0;
}
