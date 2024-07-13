// CppThread.h
#ifndef CPPTHREAD_H
#define CPPTHREAD_H

#include <thread>

class CppThread {
public:
    inline void start() {
        uthread = std::thread(&CppThread::run, this);
    }

    inline void join() {
        uthread.join();
    }

protected:
    virtual void run() = 0;

private:
    std::thread uthread;
};

#endif // CPPTHREAD_H
