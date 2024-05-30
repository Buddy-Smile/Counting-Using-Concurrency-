#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void countUp() {
    for (int i = 0; i <= 20; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "Count Up: " << i << std::endl;
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();
}

void countDown() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return ready; });
    for (int i = 20; i >= 0; --i) {
        std::cout << "Count Down: " << i << std::endl;
    }
}

int main() {
    std::thread t1(countUp);
    std::thread t2(countDown);

    t1.join();
    t2.join();

    return 0;
}