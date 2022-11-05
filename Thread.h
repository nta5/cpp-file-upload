#pragma once

class Thread {
public:
	Thread(Thread *childThread);
	~Thread();
    virtual void run() {}
    void start();
    Thread *childThread;
private:
	void *state;
//	Thread *childThread;
};
