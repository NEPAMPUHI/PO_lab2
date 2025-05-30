//Find the number of elements greater than 20, as well as the largest such number.

#include <iostream>
#include <chrono>
#include <mutex>
#include <vector>

using namespace std;
using namespace chrono;

static const int threads_num = 2;
static const unsigned long long arr_size = 20000;
int arr[arr_size];

void fill_array() {
    for (int i = 0; i < arr_size; i++) {
        arr[i] = rand() % 30 + 1;
    }
}

void solution_without_parallelization(int& number, int& largest) {
    number = 0;
    largest = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i] > 20) {
            number++;
            if (arr[i] > largest)
                largest = arr[i];
        }
    }
}

void process_section_with_mutex(int start,  int end, mutex &mtx, int &number, int &largest) {
    int local_count = 0;
    int local_max = 0;
    for (int i = start; i < end; i++) {
        if (arr[i] > 20) {
            local_count++;
            if (arr[i] > local_max)
                local_max = arr[i];
        }
    }
    lock_guard<mutex> lock(mtx);
    number += local_count;
    if (local_max > largest)
        largest = local_max;
}

void solution_with_mutex(int& number, int& largest) {
    number = 0;
    largest = 0;
    mutex mtx;
    vector<thread> threads;
    int block_size = arr_size / threads_num;

    for (int i = 0; i < threads_num; i++) {
        int start = i * block_size;
        int end = (i == threads_num - 1) ? arr_size : start + block_size;
        threads.emplace_back(process_section_with_mutex, start, end, ref(mtx), ref(number), ref(largest));
    }

    for (auto& thr : threads) {
        thr.join();
    }  
}

bool is_correct(int n, int l) {
    int number;
    int largest;
    solution_without_parallelization(number, largest);
    return number == n && largest == l;
}

void print_results(const string& solution_type, double duration, int n, int l) {
    cout << solution_type << ": " << duration << " seconds" << endl;
    cout << "The number = " << n << ", the largest = " << l << endl;
    cout << "Matrix is " << (is_correct(n, l) ? "" : "NOT ") << "correct" << endl << endl;
}

int main() {
    srand(123456789);
    fill_array();

    int number;
    int largest;

    auto begin = high_resolution_clock::now();
    solution_without_parallelization(number, largest);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - begin).count() * 1e-9;
    print_results("Solution without parallelization", duration, number, largest);

    begin = high_resolution_clock::now();
    solution_with_mutex(number, largest);
    end = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(end - begin).count() * 1e-9;
    print_results("Solution with mutex", duration, number, largest);

    return 0;
}
