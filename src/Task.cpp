//
// Created by liben on 2021/7/23.
//

#include "Task.h"

#include <chrono>


Task::Task(int what) : Task(what, 0) {}

Task::Task(int what, long uptimeMillis) : m_what(what) {
    setWhen(uptimeMillis);
    task = nullptr;
}

void Task::setWhen(long uptimeMillis) {
    when = std::chrono::system_clock::now() + std::chrono::milliseconds(uptimeMillis);
}

void Task::setFunction(std::function<void()> &&f) {
    this->task = f;
}

Task::~Task() {

}

Task &Task::operator=(const Task &msg) {
    this->m_what = msg.m_what;
    this->when = msg.when;
    this->task = msg.task;

    return *this;
}