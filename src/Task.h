//
// Created by liben on 2021/7/23.
//

#ifndef SCOREREADER_TASK_H
#define SCOREREADER_TASK_H

#include <chrono>
#include <functional>

class Task {
public:
    int m_what;
    typedef std::function<void()> Function;
    Function task;

    std::chrono::system_clock::time_point when;

public:

    Task(int what);

    Task(int what, long uptimeMillis);

    virtual ~Task();

    Task &operator=(const Task &msg);

    void setWhen(long uptimeMillis);

    void setFunction(std::function<void()> &&f);

    bool operator>(const Task &msg) const {
        return (this->when > msg.when);
    }

    bool operator<(const Task &msg) const {
        return (this->when < msg.when);
    }

    bool operator==(const Task &msg) const {
        return (this->m_what == msg.m_what) && (this->task != nullptr) && (msg.task != nullptr);
    }

    bool operator==(int what) const {
        return (this->m_what == what);
    }

private:

};

#endif //SCOREREADER_TASK_H
