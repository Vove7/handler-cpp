//
// Created by liben on 2021/7/23.
//

#include <chrono>
#include <algorithm>
#include "Handler.h"
#include "Task.h"

Handler::Handler() : stopped(false) {
    looper = std::thread([this]() {
        for (;;) {
            Task msg(0);
            {
                std::unique_lock<std::mutex> lock(this->queue_mutex);
                if (this->msg_Q.empty()) {
                    this->condition.wait(lock, [this] {
                        return this->stopped || !this->msg_Q.empty();
                    });
                } else {
                    this->condition.wait_until(lock, this->msg_Q.back().when, [this] {
                        return this->stopped || !this->msg_Q.empty();
                    });
                }
                if (stopped) {
                    return;
                }
                msg = msg_Q.back();
                if (msg.when <= std::chrono::system_clock::now()) {
                    msg_Q.pop_back();
                } else {
                    auto dur = msg.when - std::chrono::system_clock::now();
                    condition.wait_for(lock, dur);
                    continue;
                }
            }
            this->dispatchMessage(msg);
        }
    });
}

Handler::~Handler() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stopped = true;
    }
    condition.notify_all();
    looper.join();
    msg_Q.clear();
}

int Handler::post(std::function<void()> &&f) {
    return postDelayed(std::forward<std::function<void()>>(f), 0);
}

int Handler::postDelayed(std::function<void()> &&f, long delay) {
    if (stopped || f == nullptr || delay < 0) {
        return 0;
    }
    std::unique_lock<std::mutex> lock(queue_mutex);
    Task msg(genWhat());
    msg.when += std::chrono::milliseconds(delay);
    msg.setFunction(std::forward<std::function<void()>>(f));
    msg_Q.push_back(msg);
    std::sort(msg_Q.begin(), msg_Q.end(), std::greater<Task>());
    condition.notify_one();
    return msg.m_what;
}

void Handler::cancelTask(int taskId) {
    if (taskId < 0)
        return;
    std::unique_lock<std::mutex> lock(queue_mutex);

    auto i = find(msg_Q.begin(), msg_Q.end(), taskId);
    if (i != msg_Q.end()) {
        msg_Q.erase(i);
    }
}

void Handler::cancelAllTask() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    msg_Q.clear();
    condition.notify_one();
}

void Handler::quit() {
    cancelAllTask();
    std::unique_lock<std::mutex> lock(queue_mutex);
    stopped = true;
    condition.notify_one();
}


bool Handler::isQuiting() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    return stopped;
}

void Handler::dispatchMessage(Task &msg) {
    if (msg.task != nullptr) {
        msg.task();
    }
}
