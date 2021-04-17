#include <iostream>
#include<queue>
#include<chrono>
#include <windows.h>
#include<vector>
#include<string>
#include<fstream>
//try to build a template, but with windows h it's so terrible
// lets build it for a double

class BasicMatrix {
    std::vector<std::vector<double>> matrix_;
    int n_;
    int m_;
    void check_if_in_matrix(int i, int j) {
        if (i < 0 || j < 0 || i >= n_ || j >= m_) {
            throw std::runtime_error("Out of bound");
        }
    }
public:
    BasicMatrix() : matrix_(std::vector<std::vector<double>>(0)),n_(0),m_(0) {};
    BasicMatrix(int n, int m):n_(n),m_(m) {
        matrix_ = std::vector<std::vector<double>>(n);
        for (int i = 0; i < n; i++) {
            matrix_[i].resize(m);
        }
    };
    int get_n() {
        return n_;
    }
    int get_m() {
        return m_;
    }

    double get_value(int i, int j) {
        this->check_if_in_matrix(i, j);
        return this->matrix_[i][j];
    }
    void set_value(double value, int i, int j) {
        this->check_if_in_matrix(i, j);
        this->matrix_[i][j] = value;
    }
    BasicMatrix multiply(BasicMatrix& m2) {
        if (this->get_m() != m2.get_n()) {
            throw std::runtime_error("They are not conjugate");
        }
        BasicMatrix m3(this->get_n(), m2.get_m());
        for (int i = 0; i < this->get_n(); i++) {
            for (int j = 0; j < m2.get_m(); j++) {
                double curr_sum = 0;
                for (int k = 0; k < this->get_m(); k++) {
                    curr_sum += this->get_value(i, k) * m2.get_value(k, j);
                }
                m3.set_value(curr_sum, i, j);
            }
        }
        return m3;
    }
    std::vector<std::vector<BasicMatrix>> split_by_bloks(int rows_in_one, int cols_in_one) {
        int count_of_r = this->n_ / rows_in_one;
        int count_of_c = this->m_ / cols_in_one;
        std::vector<std::vector<BasicMatrix>> bloks(count_of_r);
        for (int i = 0; i < count_of_r; i++) {
            for (int j = 0; j < count_of_c; j++) {
                BasicMatrix tmp(rows_in_one, cols_in_one);
                for (int k = i * rows_in_one; k < (i + 1) * rows_in_one ; k++) {
                    for (int l = j * cols_in_one; l < (j + 1) * cols_in_one ; l++) {
                        tmp.set_value(this->get_value(k, l), k - i * rows_in_one, l - j * cols_in_one);
                    }
                }
                bloks[i].push_back(tmp);
            }
        }
        return bloks;
    }
    void print_matrix() {
        for (int i = 0; i < this->n_; i++) {
            for (int j = 0; j < this->m_; j++) {
                std::cout << this->get_value(i, j)<<" ";
            }
            std::cout << "\n";
        }
    }
};
// sorry for that but it was noway
BasicMatrix a(0, 0);
BasicMatrix b(0, 0);
BasicMatrix result(0, 0);
std::vector<std::vector<BasicMatrix>> blocks_a;
std::vector<std::vector<BasicMatrix>>blocks_b;
// thats for mutex, when we calculating col*row
std::vector<std::vector<HANDLE>> mutexes(0);
DWORD WINAPI thread_multiply_by_row_and_col(CONST LPVOID lpParam) {
    //row and column
    int* arr = (int*)lpParam;
    int r = arr[0];
    int c = arr[1];
    double value = 0;
    for (int i = 0; i < a.get_m(); i++) {
        value+= a.get_value(r, i) * b.get_value(i, c);
    }
    //by this way we don't worry about multithreading, cause every i and j is unique for writing
    result.set_value(value, arr[0], arr[1]);
    ExitThread(0);

}
// it's uncorrect ( in sense of math )
DWORD WINAPI thread_multiply_by_col_and_row(CONST LPVOID lpParam) {
    // col and row
    int* arr = (int*)lpParam;
    int c = arr[0];
    int r = arr[1];
    BasicMatrix tmp(a.get_n(), b.get_m());
    for (int i = 0; i < a.get_n(); i++) {
        for (int j = 0; j < b.get_m(); j++) {
            tmp.set_value(a.get_value(i, c) * b.get_value(r, j), i, j);
        }
    }
    // so we updating our matrix
    for (int i = 0; i < result.get_n(); i++) {
        for (int j = 0; j < result.get_m(); j++) {
            // muting 1 single value
            WaitForSingleObject(mutexes[i][j], INFINITE);
            result.set_value(tmp.get_value(i, j) + result.get_value(i, j), i, j);
            ReleaseMutex(mutexes[i][j]);
        }
    }
    ExitThread(0);

}

//so there was a lot of ways for solving
// I choose smth like a multiplication of block matrices in one block matrix and then write them block by block

DWORD WINAPI thread_multiply_by_block(CONST LPVOID lpParam) {
    // arr of indexes - row/column block multiply
    int* arr = (int*)lpParam;
    int r = arr[0];
    int c = arr[1];
    std::vector<BasicMatrix> blocks_b_t_tmp;
    for (int i = 0; i < blocks_b.size(); i++) {
        blocks_b_t_tmp.push_back(blocks_b[i][c]);
    }
    BasicMatrix res(blocks_a[r][0].get_n(), blocks_b_t_tmp[0].get_m());
    for (int i = 0; i < blocks_a[r].size(); i++) {
        BasicMatrix tmp = blocks_a[r][i].multiply(blocks_b_t_tmp[i]);
        for (int k = 0; k < tmp.get_n(); k++) {
            for (int l = 0; l < tmp.get_m(); l++) {
                res.set_value(tmp.get_value(k, l) + res.get_value(k, l), k, l);
            }
        }
    }
    //by multiplying this way we dont worry about data(in case of multithreading)
    // cause every block is unique for writing
    for (int i = 0; i < res.get_n(); i++) {
        for (int j = 0; j < res.get_m(); j++) {
            result.set_value(res.get_value(i, j), r * res.get_n()+ i, c * res.get_m() + j);
        }
    }
    ExitThread(0);
}

class ThreadMatrixMultiplier {
private:
    int number_of_threads_;
    void is_conjugate(int m1, int n2) {
        if (m1 != n2) {
            throw std::runtime_error("They are not conjugate");
        }
    }

public:
    ThreadMatrixMultiplier(int number_of_threads) {
        is_conjugate(a.get_m(), b.get_n());
        result = BasicMatrix(a.get_n(), b.get_m());
        this->number_of_threads_ = number_of_threads;
        mutexes = std::vector<std::vector<HANDLE>>(a.get_n());
        for (int i = 0; i < a.get_n(); i++) {
            mutexes[i].resize(b.get_m());
            for (int j = 0; j < b.get_m(); j++) {
                mutexes[i][j] = CreateMutex(NULL, FALSE, NULL);
            }
        }
    }
    BasicMatrix  multiply_by_row_and_column() {
        std::queue<HANDLE> threads;
        int count_of_threads = 0;
        std::queue<std::pair<int, int>> is_and_js;
        for (int i = 0; i < a.get_n(); i++) {
            for (int j = 0; j < b.get_m(); j++) {
                is_and_js.push({ i,j });
            }
        }
        auto begin = std::chrono::steady_clock::now();
        for (int i = 0; i < a.get_n() * b.get_m(); i++) {
            std::pair<int, int> tmp = is_and_js.front();
            is_and_js.pop();
            if (count_of_threads < number_of_threads_) {
                int* arr = new int[2];
                arr[0] = tmp.first;
                arr[1] = tmp.second;
                void* p = arr;
                threads.push(CreateThread(NULL, 0, &thread_multiply_by_row_and_col, p, 0, NULL));
                count_of_threads++;
            }
            else {
                WaitForSingleObject(threads.front(), INFINITE);
                threads.pop();
                int* arr = new int[2];
                arr[0] = tmp.first;
                arr[1] = tmp.second;
                void* p = arr;
                threads.push(CreateThread(NULL, 0, &thread_multiply_by_row_and_col, p, 0, NULL));
            }
       }
        HANDLE threads_arr[100];
        for (int i = 0; i < count_of_threads; i++) {
            threads_arr[i] = threads.front();
            threads.pop();
        }
        WaitForMultipleObjects(count_of_threads, threads_arr, TRUE, INFINITE);
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << "The time: " << elapsed_ms.count() << " ms\n";

        return result;
    }
    // sorry for duplicate but it only 1 time of it
    BasicMatrix multiply_by_column_and_row() {
        std::queue<HANDLE> threads;
        int count_of_threads = 0;
        std::vector<std::pair<int, int>> is_and_js;
        for (int i = 0; i < a.get_m(); i++) {
            for (int j = 0; j < b.get_n(); j++) {
                is_and_js.push_back({  i,j });
            }
        }
        auto begin = std::chrono::steady_clock::now();

        for (int i = 0; i < a.get_m() * b.get_n(); i++) {
            std::pair<int, int> tmp = is_and_js[i];
            if (count_of_threads < number_of_threads_) {
                int  * arr = new int[2];
                arr[0] = tmp.first;
                arr[1] = tmp.second;
                void* p = arr;
                threads.push(CreateThread(NULL, 0, &thread_multiply_by_col_and_row, p, 0, NULL));
                count_of_threads++;
            }
            else {
                WaitForSingleObject(threads.front(), INFINITE);
                threads.pop();
                int* arr = new int[2];
                arr[0] = tmp.first;
                arr[1] = tmp.second;
                void* p = arr;
                threads.push(CreateThread(NULL, 0, &thread_multiply_by_col_and_row, p, 0, NULL));
            }
        }
        HANDLE threads_arr[100];
        for (int i = 0; i < count_of_threads; i++) {
            threads_arr[i] = threads.front();
            threads.pop();
        }
        WaitForMultipleObjects(count_of_threads, threads_arr, TRUE, INFINITE);
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << "The time: " << elapsed_ms.count() << " ms\n";

        return result;
    }
    BasicMatrix multiply_by_block() {
        // so there can be any blocks, but let it be only:
        if ((a.get_n() % 2 == 0) && (a.get_m() % 2 == 0) && (b.get_n() % 2 == 0) && (b.get_m() % 2 == 0)) {
            // 2 x 2 blocks
            blocks_a = a.split_by_bloks(2,2);
            blocks_b = b.split_by_bloks(2,2);

        }
        else if ((a.get_n() % 2 == 0) && (a.get_m() % 3 == 0 )&& (b.get_n() % 3 == 0) && (b.get_m() % 2 == 0)) {
            // 2x3 and 3x2 

            blocks_a = a.split_by_bloks(2, 3);
            blocks_b = b.split_by_bloks(3, 2);

        }
        else if((a.get_n() % 3 == 0) && (a.get_m() % 2 == 0) && (b.get_n() % 2 == 0) && (b.get_m() % 3 == 0)){
            // 3x2 and 2 x 3

            blocks_a = a.split_by_bloks(3, 2);
            blocks_b = b.split_by_bloks(2, 3);
        }
        else if ((a.get_n() % 3 == 0) && (a.get_m() % 3 == 0) &&(b.get_n() % 3 == 0) && (b.get_m() % 3 == 0)) {
            // 3x3 and 3 x 3
            blocks_a = a.split_by_bloks(3, 3);
            blocks_b = b.split_by_bloks(3, 3);
        }
       
        std::vector<std::pair<int, int>> is_and_js;
        for (int i = 0; i < blocks_a.size(); i++) {
            for (int j = 0; j < blocks_b[0].size(); j++) {
                is_and_js.push_back({ i,j });
            }
        }
        std::queue<HANDLE> threads;
        int count_of_threads = 0;
        auto begin = std::chrono::steady_clock::now();
        for (int i = 0; i < is_and_js.size(); i++) {
            std::pair<int, int> tmp = is_and_js[i];
            if (count_of_threads < number_of_threads_) {
                int* arr = new int[2];
                arr[0] = tmp.first;
                arr[1] = tmp.second;
                void* p = arr;
                threads.push(CreateThread(NULL, 0, &thread_multiply_by_block, p, 0, NULL));
                count_of_threads++;
            }
            else {
                WaitForSingleObject(threads.front(), INFINITE);
                threads.pop();
                int* arr = new int[2];
                arr[0] = tmp.first;
                arr[1] = tmp.second;
                void* p = arr;
                threads.push(CreateThread(NULL, 0, &thread_multiply_by_block, p, 0, NULL));
            }
        }
        HANDLE threads_arr[100];
        for (int i = 0; i < count_of_threads; i++) {
            threads_arr[i] = threads.front();
            threads.pop();
        }
        WaitForMultipleObjects(count_of_threads, threads_arr, TRUE, INFINITE);
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << "The time: " << elapsed_ms.count() << " ms\n";

        return result;
    }
};
// format like - n,m, n*m values
std::pair<BasicMatrix,BasicMatrix> read_2_matrix(std::string filename = "input.txt") {
    std::ifstream in(filename);
    int n, m;
    in >> n>>m;
    BasicMatrix matrix1(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double tmp;
            in >> tmp;
            matrix1.set_value(tmp, i, j);
        }
    }
    in >> n >> m;
    BasicMatrix matrix2(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double tmp;
            in >> tmp;
            matrix2.set_value(tmp, i, j);
        }
    }
    return { matrix1,matrix2 };

}

int main(){
    auto tmp = read_2_matrix();
    a = tmp.first;
    b = tmp.second;
    std::cout << "a = \n";
    a.print_matrix();
    std::cout << "b = \n";
    b.print_matrix();
    std::cout << "basic multiply\n";
    auto begin = std::chrono::steady_clock::now();
    BasicMatrix c = a.multiply(b);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";
    std::cout << "result = \n";
    c.print_matrix();
    std::cout << "\n";
    int k_threads = 2;
    std::cout << "count_of_threads = " << k_threads << "\n";
    std::cout << "multiply by rows and columns\n";
    ThreadMatrixMultiplier  mm1( k_threads);
    c = mm1.multiply_by_row_and_column();
    std::cout << "result = \n";
    c.print_matrix();
    std::cout << "\n";
    std::cout << "multiply by columns and rows\n";
    ThreadMatrixMultiplier  mm2(k_threads);
    c = mm2.multiply_by_column_and_row();
    std::cout << "result = \n";
    c.print_matrix();
    std::cout << "\n";
    std::cout << "multiply by block\n";
    ThreadMatrixMultiplier  mm3(k_threads);
  
    c = mm3.multiply_by_block();
    std::cout << "result = \n";
    c.print_matrix();
    std::cout << "\n";
    return 0;
}

