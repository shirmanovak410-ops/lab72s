//
// Created by Ксения on 27.04.2026.
//
//вариант 4
#include <future>
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
std::mutex m;
std::condition_variable cv;
int completed_threads = 0;
void bubble_sort(std::vector<double>& arr, int start, int end,
                 const std::string& name, bool time_flag, bool block_flag) {
    for (int i = 0; i < end - start - 1; i++) {
        for (int j = start; j < end - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
    for (int i = start; i < end; i++) {
        if (time_flag) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        if (block_flag) {
            m.lock();
            std::cout << name << ": " << arr[i] << std::endl;
            m.unlock();
        } else {
            std::cout << name << ": " << arr[i] << std::endl;
        }
    }

    {
        std::lock_guard<std::mutex> lock(m);
        completed_threads++;
        cv.notify_one();
    }
}
bool sort_wrapper(std::vector<double>& arr, int start, int end,
                  const std::string& name, bool time_flag, bool block_flag) {
    bubble_sort(arr, start, end, name, time_flag, block_flag);
    return true;
}
int main() {
    const int SIZE = 20;
    std::vector<double> arr(SIZE);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(1.0, 100.0);

    for (int i = 0; i < SIZE; i++) {
        arr[i] = dis(gen);
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl << std::endl;
    int n = SIZE / 2;
    bool type_flag, time_flag, block_flag, main_stream;
    std::cout << "0 - future/async;  1 - threads" << std::endl;
    std::cin >> type_flag;
    std::cout << "0 - without delay;  1 - with delay" << std::endl;
    std::cin >> time_flag;
    std::cout << "0 - without mutex;  1 - with mutex" << std::endl;
    std::cin >> block_flag;
    std::cout << "0 - without waiting;  1 - with waiting" << std::endl;
    std::cin >> main_stream;
    completed_threads = 0;
    if (!type_flag) {
        std::cout << "\nfuture/async " << std::endl;
        auto th1 = std::async(std::launch::async, sort_wrapper, std::ref(arr), 0, n, "th1", time_flag, block_flag);
        auto th2 = std::async(std::launch::async, sort_wrapper, std::ref(arr), n, SIZE, "th2", time_flag, block_flag);
        if (main_stream) {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, []{ return completed_threads >= 2; });
        } else {
            th1.get();
            th2.get();
        }
        auto th3 = std::async(std::launch::async, sort_wrapper, std::ref(arr), 0, SIZE, "th3", time_flag, block_flag);
        if (main_stream) {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, []{ return completed_threads >= 3; });
        } else {
            th3.get();
        }
    }
    else {
        std::cout << "\nthread" << std::endl;
        std::thread th1(bubble_sort, std::ref(arr), 0, n, "th1", time_flag, block_flag);
        std::thread th2(bubble_sort, std::ref(arr), n, SIZE, "th2", time_flag, block_flag);
        if (main_stream) {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, []{ return completed_threads >= 2; });
        } else {
            th1.join();
            th2.join();
        }
        if (!main_stream) {
            th1.join();
            th2.join();
        }
        std::thread th3(bubble_sort, std::ref(arr), 0, SIZE, "th3", time_flag, block_flag);
        if (main_stream) {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, []{ return completed_threads >= 3; });
        } else {
            th3.join();
        }
        if (!main_stream) {
            th3.join();
        }
    }
    if (main_stream) {
    } else {
        std::cout << "done" << std::endl;
    }
    for (double v : arr) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    return 0;
}