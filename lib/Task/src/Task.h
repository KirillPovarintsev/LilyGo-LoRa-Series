#ifndef TASK_H
#define TASK_H

#include <Arduino.h>

class Task
{
public:
    Task(std::function<void()> action, int period = 0) : _period(period), _action(action) {}

public:
    void loop()
    {
        if (_started && millis() - _lastExecute > _period)
        {
            _action();

            _lastExecute = millis();
        }
    }

    void start() { _started = true; }
    void stop() { _started = false; }
    void toggle() { _started = !_started; }
    bool isStarted() { return _started; }

protected:
    bool _started = false;
    int _period = 0;
    std::function<void()> _action;
    int _lastExecute = 0;
};

#endif