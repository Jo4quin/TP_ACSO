/**
 * File: thread-pool.cc
 * --------------------
 * Presents the implementation of the ThreadPool class.
 */

#include "thread-pool.h"
using namespace std;

ThreadPool::ThreadPool(size_t numThreads)
    : wts(numThreads), done(false) {
    for(size_t i = 0; i < wts.size(); ++i){
        wts[i].workerThread = thread(&ThreadPool::worker, this, i);
    }
    dispatcherThread = thread(&ThreadPool::dispatcher, this);
}

ThreadPool::~ThreadPool(){
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]{
            return tasksCompleted.load() == tasksScheduled.load();
        });
    }
    done = true;
    cv.notify_all();
    for (auto& w : wts){
        w.sem.signal();
    }
    if(dispatcherThread.joinable()) dispatcherThread.join();
    for(auto& w : wts){
        if(w.workerThread.joinable()) w.workerThread.join();
    }
    destroyed_ = true;
}

void ThreadPool::schedule(const function<void(void)>& thunk){
    if(destroyed_) throw runtime_error("schedule() called after destruction");
    if(!thunk) throw invalid_argument("Cannot schedule nullptr task.");
    {
        lock_guard<mutex> lock(mtx);
        tasks.push(thunk);
        tasksScheduled++;
    }
    cv.notify_all();
}

void ThreadPool::wait(){
    if(destroyed_) return;
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [this]{
        return tasksCompleted.load() == tasksScheduled.load();
    });
}

void ThreadPool::dispatcher(){
    while(true){
        function<void()> task;
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]{
                return done || !tasks.empty();
            });
            if(done && tasks.empty()) break;
            task = move(tasks.front());
            tasks.pop();
        }
        bool assigned = false;
        while(!assigned){
            for(auto& w : wts){
                lock_guard<mutex> lk(w.taskMutex);
                if(!w.task){
                    w.task = move(task);
                    w.sem.signal();
                    assigned = true;
                    break;
                }
            }
            if(!assigned) this_thread::yield();
        }
    }
}

void ThreadPool::worker(size_t id){
    WorkerEntry& w = wts[id];
    while(true){
        w.sem.wait();
        {
            lock_guard<mutex> lk(w.taskMutex);
            if(done && !w.task) break;
            if(w.task){
                w.task();
                w.task = nullptr;
                tasksCompleted++;
                cv.notify_all();
            }
        }
    }
}