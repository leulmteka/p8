#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "atomic.h"
#include "stdint.h"
#include "shared.h"
#include "vmm.h"
#include "blocking_lock.h"
#include "future.h"
#include "file.h"
#include "u8250.h"
#include "shared.h"

class Process {
	constexpr static int NSEM = 10;
	constexpr static int NCHILD = 10;
    constexpr static int NFILE = 10;

    File* files[NFILE]{};
	Semaphore* sems[NSEM]{};
	Future<uint32_t>* children[NCHILD]{};
	BlockingLock mutex{};

	int getChildIndex(int id);
	int getSemaphoreIndex(int id);
	int getFileIndex(int id);

    Atomic<uint32_t> ref_count {0};

public:
    Future<uint32_t>* output = new Future<uint32_t>();// { new Future<uint32_t>() };
    uint32_t *pd = gheith::make_pd();
    static Process* kernelProcess;

	Process(bool isInit);
	virtual ~Process();

	Process* fork(int& id);
    void clear_private();

	int newSemaphore(uint32_t init);

	Semaphore* getSemaphore(int id);

    File* getFile(int fd) {
        auto i = getFileIndex(fd);
        if (i < 0) {
            //return Shared<File>();
            return nullptr;
        }
        return files[fd];
    }

    int setFile(File* file) {
        for (auto i = 0; i<NFILE; i++) {
            auto f = files[i];
            if (f == nullptr) {
                files[i] = file;
                return i;
            }
        }
        return -1;
    }

    
	int close(int id);
	void exit(uint32_t v) {
		output->set(v);
	}
	int wait(int id, uint32_t* ptr);

	static void init(void);

    //friend class Shared<Process>;
    //friend class gheith::TCB;
};

#endif
