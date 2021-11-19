#include <iostream>
#include "SharedObject.h"
#include "thread.h"
#include <string>
#include "Semaphore.h"


struct MyShared{
	int sharedDelay;
	int sharedThread;
	int sharedReport;
	bool sharedRun;
};
class WriterThread : public Thread {
	public:
		int delay;
		int threadNum;
		int reportNum = 1;
		bool run;
		
		WriterThread(int in, int threadNum):Thread(8*1000){
			this->threadNum = threadNum;
			this->delay = in;
		}
		virtual long ThreadMain(void) override{
			
			//declare shared memory var so this thread can access it
			Shared<MyShared> sharedMemory ("sharedMemory");
			Semaphore writerSem("writerSemaphore");
			Semaphore readerSem("readerSemaphore");
			// Creates the reader and writer Semaphores

			while(true) {
				writerSem.Wait(); 	// Locks resources so that there are no multiples
				sharedMemory->sharedThread = threadNum;
				sharedMemory->sharedReport = reportNum;
				sharedMemory->sharedDelay = delay;
				//write to shared memory

				reportNum ++;	// increases report count by 1

				writerSem.Signal();		// resources unlocked for read/write operations
				readerSem.Signal();		// Signals semaphore to start reading
				sleep(delay);			// Sleep for delay duration
				 
				if(flag) {	//Exit loop to end the thread
					break;
				}
			}
			return 1;
		}	
};
int main(void) {
	Semaphore writerSem("writerSemaphore", 1, true);
	Semaphore readerSem("readerSemaphore", 0, true);	
	// Creates writer and reader with initial values of 1 and 0, and status true and false, respectively
	
	std::string uInput;
	std::string uDelay;
	int numThreads = 1;
	
	std::cout << "I am a Writer" << std::endl;
	
	WriterThread * thread;
	Shared<MyShared> shared("sharedMemory", true);
	
	while(true) {
		std::cout << "Press (Y) to create Writer Thread, else press (N)."<< std::endl;
		getline(std::cin,uInput);
		if(userInput == "Y") {
			std::cout << "Enter delay time:"<< std::endl;
			getline(std::cin,uDelay);
			int delay = atoi(uDelay.c_str()); // Converts the entered string into an int
			shared->sharedRun = true;
		
			thread = new WriterThread(delay,numThreads);
			numThreads++; // Increase count of threads
		}
		else if (uInput == "N") {
			if(numThreads -1 !=0){
				shared->sharedRun = false;
				break;
			} else {
				break;
			}	// When pressing N it will break
		} else {
			std::cout << "Press either (Y) for yes or (N) for no." << std::endl;
			// For invlid options (not Y or N)
		}
	}
	if(numThreads-1 != 0) {
		thread->run = true;
		delete thread; 
	}
	return 0;	
}

