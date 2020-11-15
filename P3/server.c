#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include "util.h"
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

#define MAX_THREADS 100
#define MAX_queue_len 100
#define MAX_CE 100
#define INVALID -1
#define BUFF_SIZE 1024

/*
  THE CODE STRUCTURE GIVEN BELOW IS JUST A SUGGESTION. FEEL FREE TO MODIFY AS NEEDED
*/

// structs:
typedef struct request_queue {
   int fd;
   char *request;
} request_t;

typedef struct cache_entry {
    int len;
    char *request;
    char *content;
} cache_entry_t;

/* ******************** Dynamic Pool Code  [Extra Credit A] **********************/
// Extra Credit: This function implements the policy to change the worker thread pool dynamically
// depending on the number of requests
void * dynamic_pool_size_update(void *arg) {
  while(1) {
    // Run at regular intervals
    // Increase / decrease dynamically based on your policy
  }
}
/**********************************************************************************/

/* ************************ Cache Code [Extra Credit B] **************************/

// Function to check whether the given request is present in cache
int getCacheIndex(char *request){
  /// return the index if the request is present in the cache
}

// Function to add the request and its file content into the cache
void addIntoCache(char *mybuf, char *memory , int memory_size){
  // It should add the request at an index according to the cache replacement policy
  // Make sure to allocate/free memory when adding or replacing cache entries
}

// clear the memory allocated to the cache
void deleteCache(){
  // De-allocate/free the cache memory
}

// Function to initialize the cache
void initCache(){
  // Allocating memory and initializing the cache array
}

/**********************************************************************************/

/* ************************************ Utilities ********************************/
// Function to get the content type from the request
char* getContentType(char * mybuf) {
  // Should return the content type based on the file type in the request
  // (See Section 5 in Project description for more details)

  
}

// Function to open and read the file from the disk into the memory
// Add necessary arguments as needed
int readFromDisk(/*necessary arguments*/) {
    // Open and read the contents of file given the request
}

/**********************************************************************************/

// Function to receive the request from the client and add to the queue
void * dispatch(void *arg) {

  while (1) {

    // Accept client connection

    // Get request from the client

    // Add the request into the queue

   }
   return NULL;
}

/**********************************************************************************/

// Function to retrieve the request from the queue, process it and then return a result to the client
void * worker(void *arg) {

   while (1) {

    // Get the request from the queue

    // Get the data from the disk or the cache (extra credit B)

    // Log the request into the file and terminal

    // return the result
  }
  return NULL;
}

/**********************************************************************************/

static volatile sig_atomic_t exitFlag = 0;
int main(int argc, char **argv) {

  // Error check on number of arguments
  if(argc != 8){
    printf("usage: %s port path num_dispatcher num_workers dynamic_flag queue_length cache_size\n", argv[0]);
    return -1;
  }

  // Get the input args
  int port = argv[3];
  char* path = argv[4];
  int dispatchers = argv[5];
  int workers = argv[6];
  int dynFlag = argv[7];
  int qLen = argv[8];
  int cSiz = argv[9];
  // Perform error checks on the input arguments
  if (port < 1025 || port > 65535) {
    perror("Invalid port. Port must be greater than 1024 or less than 65536.\n");
    return -1;
  }
  if (dispatchers > MAX_THREADS || dispatchers < 1) {
    perror("Number of dispatchers is invalid. It must be greater than 0 or less than 101.\n");
    return -1;
  }
  if (workers > MAX_THREADS || workers < 1) {
    perror("Number of dispatchers is invalid. It must be greater than 0 or less than 101.\n");
    return -1;
  }
  if (qLen > MAX_queue_len || qLen <= 0) {
    perror("Queue length is invalid.\n");
    return -1;
  }
  // Change SIGINT action for grace termination
  struct sigaction act;
  sigset_t sigset;
  act.sa_handler = eggs;
  act.sa_flags = 0;
  if (sigemptyset(&act.sa_mask) == -1 ||
      sigaction(SIGINT, &act, NULL) == -1) {
        perror("SIGINT handler failed.\n");
        return -1;
  }
  // Open log file
  FILE* logfile = fopen("serverlog.txt", "a+");
  // Change the current working directory to server root directory
  if (chdir("testing") == -1) {
    perror("Could not find root webserver directory testing. Exiting\n");
    return -1;
  }
  // Initialize cache (extra credit B)

  // Start the server
  init(port);
  // Create dispatcher and worker threads (all threads should be detachable)
  pthread_t dThreads[dispatchers];
  pthread_create(&dThreads, NULL, dispatch, NULL); // DEBUG! figure out last arg

  pthread_t wThreads[workers];
  pthread_create(&wThreads, NULL, worker, NULL); //TODO: Worker arguments.

  // Create dynamic pool manager thread (extra credit A)

  //Server loop (RUNS FOREVER)
  while (1) {
    //TODO: Add something else?

    // Terminate server gracefully
    if (exitFlag){
      // Print the number of pending requests in the request queue
      // close log file
      fclose(logfile);
      // Remove cache (extra credit B)

      return 0;
    }
    
  }
  printf("This should never be printed.");
  return 42;
}
static void eggs(int signo) {
  exitFlag |= 1;
}

