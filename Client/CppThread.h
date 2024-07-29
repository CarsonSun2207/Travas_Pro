#ifndef CPPTHREAD_H
#define CPPTHREAD_H

#include <thread>

// CppThread class to provide a simple interface for managing threads
class CppThread {
public:
    // Method to start the thread and run the `run` method
    inline void start() {
        uthread = std::thread(&CppThread::run, this);
    }

    // Method to join the thread (wait for it to finish execution)
    inline void join() {
        uthread.join();
    }

protected:
    // Pure virtual function to be overridden by derived classes
    virtual void run() = 0;

private:
    std::thread uthread;  // Thread object to manage the thread
};

#endif // CPPTHREAD_H
