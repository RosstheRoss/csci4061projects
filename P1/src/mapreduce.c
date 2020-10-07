#include "mapreduce.h"

int main(int argc, char *argv[]) {
	
	if(argc < 4) {
		printf("Less number of arguments.\n");
		printf("./mapreduce #mappers #reducers inputFile\n");
		exit(0);
	}

	// ###### DO NOT REMOVE ######
	int nMappers 	= strtol(argv[1], NULL, 10);
	int nReducers 	= strtol(argv[2], NULL, 10);
	if (nMappers < nReducers) {
		printf("Number of mappers must be greater than or equal to the number of reducers.\n");
		exit(-1);
	}
	char *inputFile = argv[3];

	// ###### DO NOT REMOVE ######
	bookeepingCode();

	// ###### DO NOT REMOVE ######
	pid_t pid = fork();
	if(pid == 0){
		//send chunks of data to the mappers in RR fashion
		sendChunkData(inputFile, nMappers);
		exit(0);
	} else if (pid == -1) {
		printf("Fork failed!\n");
		return 1;
	}
	sleep(1);


	pid_t mapperPid;
	for (int i = 1; i <= nMappers; i++) {
		mapperPid = fork();
		if (mapperPid == 0) {
			char numMap[10];
			sprintf(numMap, "%d", i);
			execl("./mapper", "./mapper", &numMap, NULL);
			printf("Mapper execution failed! Make sure \"mapper\" exists in the directory.\n");
			return 2;
		} else if (mapperPid == -1) {
			printf("Fork failed!\n");
			return 1;
		}
	}

    for (int i = 0; i <= nMappers; i++) {
		int waitError = wait(NULL);
		if (waitError == -1)
		{
			printf("Wait failed!\n");
			return 5;
		}
	}
	// ###### DO NOT REMOVE ######
    // shuffle sends the word.txt files generated by mapper 
    // to reducer based on a hash function
	pid = fork();
	if(pid == 0){
		shuffle(nMappers, nReducers);
		exit(0);
	} else if (pid == -1) {
		printf("Fork failed!\n");
		return 1;
	}
	sleep(1);


	pid_t reducerPid;
	for (int i = 1; i <= nReducers; i++) {
		reducerPid = fork();
		if (reducerPid == 0) {
			char numRed[10];
			sprintf(numRed, "%d", i);
			execl("./reducer", "./reducer", &numRed, NULL);
			printf("Reducer execution failed! Make sure \"reducer\" exists in the directory.\n");
			return 2;
		} else if (reducerPid == -1) {
			printf("Fork failed!\n");
			return 1;
		}
	}

	for (int i = 0; i <= nReducers; i++) {
		int waitError = wait(NULL);
		if (waitError == -1) {
			printf("Wait failed!\n");
			return 5;
		}
	}

	return 0;
}