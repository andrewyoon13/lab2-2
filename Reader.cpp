#include <iostream>
#include "SharedObject.h"
#include "Semaphore.h"

struct MyShared { 
	int sharedDelay;
	int sharedThread;
	int sharedReport;
	bool sharedRun;
}

int main(void) {
	std::cout << "I am a reader" << std::endl;
	Semaphore reader ("readerSemaphore");
	Shared<MyShared> sharedMemory("sharedMemory");	// Access the shared memory, will display data if in use, else no

	while (true) {
		if (sharedMemory -> sharedRun == true) {
			reader.Wait();
			std::cout << "Thread ID: " << sharedMemory -> sharedThread << "Report ID: " << sharedMemory -> sharedReport << "Delay: " << sharedMemory -> sharedDelay <<std::endl;
		} else {
			break;
		}
	}
}
