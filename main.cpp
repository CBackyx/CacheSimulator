#include <iostream>
#include <cstdlib>

#include "headers.h"

using namespace std;

// The arguments are: tracefile name, block size, replacement policy, write policy, origanization  
int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Invalid argument number!\n");
        exit(-1);
    } else {
        unsigned int parse_args[4];
        sscanf(argv[2], "%ud", &parse_args[0]);
        sscanf(argv[3], "%ud", &parse_args[1]);
        sscanf(argv[4], "%ud", &parse_args[2]);
        sscanf(argv[5], "%ud", &parse_args[3]);
        TraceSimulator ts = *new TraceSimulator(argv[1], parse_args);
        ts.doCommands();
    }
    
    return 0;
}