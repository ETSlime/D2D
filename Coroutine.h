#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include "SingletonBase.h"

class Coroutine
{
public:
    Coroutine(std::function<void(Coroutine&)> func) : func(func), state(0), running(true) {}
    Coroutine() : state(0), running(true) {};

    void operator()()
    {
        if (running) 
        {
            func(*this);
        }
    }

    void yield(float seconds) 
    {
        running = false;
        std::thread([this, seconds]() {
            std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
            running = true;
            state++;
            func(*this);  // Resume the coroutine after waiting
            // comment below to set complete by hand
            //if (state > 1) {
            //    completed = true;  // Mark as completed after finishing the last part
            //}
            }).detach();
    }

    int getState() const 
    {
        return state;
    }

    void setCallback(std::function<void(Coroutine&)> func)
    {
        this->func = func;
    }

    bool isCompleted() const 
    {
        return completed;
    }

    void setComplete()
    {
        completed = true;
    }

private:

    friend class SingletonBase<Coroutine>;

    std::function<void(Coroutine&)> func;
    std::atomic<int> state;  // Used to keep track of coroutine state
    std::atomic<bool> running;  // Used to control whether the coroutine should run
    std::atomic<bool> completed;  // Used to mark if the coroutine has completed
};

