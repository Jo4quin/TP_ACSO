/**
 * File: thread-pool.h
 * -------------------
 * This class defines the ThreadPool class, which accepts a collection
 * of thunks (which are zero-argument functions that don't return a value)
 * and schedules them in a FIFO manner to be executed by a constant number
 * of child threads that exist solely to invoke previously scheduled thunks.
 */

#ifndef _thread_pool_
#define _thread_pool_

#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <stdexcept>
#include <utility>
#include "Semaphore.h"

using namespace std;

struct WorkerEntry {
    thread workerThread;
    function<void()> task;
    Semaphore sem{0};
    mutex taskMutex;
};

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void schedule(const function<void(void)>& thunk);
    void wait();

private:
    void dispatcher();
    void worker(size_t id);

    vector<WorkerEntry> wts;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable_any cv;

    atomic<size_t> tasksScheduled{0};
    atomic<size_t> tasksCompleted{0};
    atomic<bool> done;
    atomic<bool> destroyed_{false};

    thread dispatcherThread;
};
#endif
