#pragma once

class Thread {
public:
	Thread(Thread *childThread);
    Thread();
    ~Thread();
    virtual void run() {};
    void start();
    Thread *childThread;
//	Thread *childThread;
protected:
    void *state;
};
