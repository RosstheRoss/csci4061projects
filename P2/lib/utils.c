#include "utils.h"

int openQueue() {
	int id = msgget(ftok("4061 Project 2 SS", 'S'), 0666 | IPC_CREAT);
	if (id < 0) {
		perror("Cannot open queue.\n");
		return -1;
	}
	return id;
}
void closeQueue() {
	int msgid = msgget(ftok("4061 Project 2 SS", 'S'), 0666);
	if (msgid < 0) {
		perror("Cannot open queue. It may already exist.\n");
		exit(-1);
	}
	msgctl(msgid, IPC_RMID, NULL);
}

char *getChunkData(int mapperID) {
	printf("GETTING CHUNK DATA\n");
	//Message
	struct msgBuffer message;
	//Queue ID
	int mid = openQueue();
	msgrcv(mid, &message, sizeof(message.msgText), mapperID, 0);
	if (strncmp("END", message.msgText, 3)) {
		return NULL;
	}
	char* value = message.msgText;
	return value;
}

// sends chunks of size 1024 to the mappers in RR fashion
void sendChunkData(char *inputFile, int nMappers) {
	printf("SENDING CHUNK DATA\n");
	struct msgBuffer message;
	// open message queue
	int msgid = openQueue();
	int map = 1;
	// message.msgText = 1;
	int fd = open(inputFile, O_RDONLY);
	// construct chunks of 1024 bytes
	memset(message.msgText, '\0', MSGSIZE);
	while(read(fd, message.msgText, chunkSize) != 0) {
		// printf("%s\n", message.msgText);
		/*  Go to the end of the chunk, check if final character 
		    is a space if character is a space, do nothing
		    else cut off before that word and put back file.
			
			Maybe copy message.msgText into a new array, do the
			backwards iteration, and then replace message.msgText
			with the temp array? Or something?       */
			
		// TODO! help 

		// int i = 1023;
		// while(message.msgText[i] != ' ') {
		// 	message.msgText 
		// }

		message.msgType = map++;
		//THIS IS DEBUG, NOT ACTUALLY FUNCTIONAL (like at all)
		msgsnd(msgid, &message, map, 0);
		if (map > nMappers)
			map = 1;
	}

	for (int i = 1; i < nMappers; i++) {
		struct msgBuffer END = {i, "END"};
		msgsnd(msgid, &END, MSGSIZE, 0);
	}

}

// hash function to divide the list of word.txt files across reducers
//http://www.cse.yorku.ca/~oz/hash.html
int hashFunction(char* Qkey, int reducers){
	unsigned long hash = 0;
    int c;

    while ((c = *Qkey++)!='\0')
        hash = c + (hash << 6) + (hash << 16) - hash;

    return (hash % reducers);
}

int getInterData(char *Qkey, int reducerID) {
	struct msgBuffer message;
	//make sure it work.
	int id = openQueue();
	msgrcv(id, &message, chunkSize, reducerID, 0);
	Qkey = message.msgText;
	return strncmp("END", message.msgText, 3);
	// if (strncmp("END", message.msgText, 3))
	// {
	// 	return 0;
	// } else {
	// 	return 1;
	// }
}

void shuffle(int nMappers, int nReducers) {
	struct msgBuffer message;
	//Once again, MAKE SURE THIS WORKS PROPERLY!
	int id = openQueue();
	for (int i = 1; i <= nMappers; i++) {
		//Extra for loop traversing directory
		//TODO: Actually traverse directory
			//message.msgType = hashFunction(/* SOMETHING ,*/ nReducers);
			msgsnd(id, &message, chunkSize, 0);
	}

}

// check if the character is valid for a word
int validChar(char c){
	return (tolower(c) >= 'a' && tolower(c) <='z') ||
					(c >= '0' && c <= '9');
}

char *getWord(char *chunk, int *i){
	char *buffer = (char *)malloc(sizeof(char) * chunkSize);
	memset(buffer, '\0', chunkSize);
	int j = 0;
	while((*i) < strlen(chunk)) {
		// read a single word at a time from chunk
		// printf("%d\n", i);
		if (chunk[(*i)] == '\n' || chunk[(*i)] == ' ' || !validChar(chunk[(*i)]) || chunk[(*i)] == 0x0) {
			buffer[j] = '\0';
			if(strlen(buffer) > 0){
				(*i)++;
				return buffer;
			}
			j = 0;
			(*i)++;
			continue;
		}
		buffer[j] = chunk[(*i)];
		j++;
		(*i)++;
	}
	if(strlen(buffer) > 0)
		return buffer;
	return NULL;
}

void createOutputDir(){
	mkdir("output", ACCESSPERMS);
	mkdir("output/MapOut", ACCESSPERMS);
	mkdir("output/ReduceOut", ACCESSPERMS);
}

char *createMapDir(int mapperID){
	char *dirName = (char *) malloc(sizeof(char) * 100);
	memset(dirName, '\0', 100);
	sprintf(dirName, "output/MapOut/Map_%d", mapperID);
	mkdir(dirName, ACCESSPERMS);
	return dirName;
}

void removeOutputDir(){
	pid_t pid = fork();
	if(pid == 0){
		char *argv[] = {"rm", "-rf", "output", NULL};
		if (execvp(*argv, argv) < 0) {
			printf("ERROR: exec failed\n");
			exit(1);
		}
		exit(0);
	} else{
		wait(NULL);
	}
}

void bookeepingCode(){
	removeOutputDir();
	sleep(1);
	createOutputDir();
}
