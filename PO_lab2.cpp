//Find the number of elements greater than 20, as well as the largest such number.

#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

static const int threads_num = 2;
static const int arr_size = 2000;
int arr[arr_size];

void fill_array() {
    for (int i = 0; i < arr_size; i++) {
        arr[i] = rand() % 30 + 1;
    }
}

void solution_without_parallelization(int& number, int& largest) {
    int counter = 0;
    int max = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i] > 20) {
            counter++;
            if (arr[i] > max)
                max = arr[i];
        }
    }
    number = counter;
    largest = max;
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

    return 0;
}
