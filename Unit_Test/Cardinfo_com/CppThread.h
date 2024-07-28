// CppThread.h
#ifndef CPPTHREAD_H
#define CPPTHREAD_H

#include <thread>

// CppThread class provides a simple interface to start and join a thread.
// Users of this class should inherit from it and implement the run method,
// which contains the code that will be executed in the thread.
class CppThread {
public:
    // Starts the thread by invoking the run method in a new thread.
    inline void start() {
        uthread = std::thread(&CppThread::run, this);
    }

    // Joins the thread, blocking the calling thread until the thread finishes execution.
    inline void join() {
        uthread.join();
    }

protected:
    // Pure virtual function that must be overridden by derived classes.
    // This function contains the code to be executed in the thread.
    virtual void run() = 0;

private:
    std::thread uthread; // Internal thread object
};

#endif // CPPTHREAD_H
