//
// Created by liben on 2021/7/23.
//

#ifndef SCOREREADER_HANDLER_H
#define SCOREREADER_HANDLER_H


#include <chrono>
#include <map>
#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>
#include <set>

#include "Task.h"


class Handler {
private:
    void dispatchMessage(Task &msg);

public:
    Handler();

    virtual ~Handler();

    int post(std::function<void()> &&f);

    int postDelayed(std::function<void()> &&f, long delay);

    void cancelTask(int taskId);

    void cancelAllTask();

    void quit();

    bool isQuiting();

private:
    std::vector<Task> msg_Q;

    std::mutex queue_mutex;
    std::mutex task_id_mutex;
    std::condition_variable condition;
    std::thread looper;
    bool stopped;

    int g_taskId = 10;

    int genWhat() {
        std::unique_lock<std::mutex> lock(this->task_id_mutex);
        if (g_taskId > 0x0fffffff) {
            g_taskId = 10;
        }
        return g_taskId++;
    }
};

#endif //SCOREREADER_HANDLER_H
