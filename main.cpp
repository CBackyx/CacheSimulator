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

        srand(0);

        TraceSimulator &ts = *new TraceSimulator(argv[1], parse_args);
        ts.doCommands();
        delete &ts;

        // parse_args[0] = 8;
        // parse_args[1] = 0;
        // parse_args[2] = 1;
        // parse_args[3] = 8;

        // string files[] = {"astar.trace", "bzip2.trace", "mcf.trace", "perlbench.trace"};
        // string tail = "_log";
        // for (int i = 0; i < 4; ++i) {
        //     char curfile[20];
        //     strcpy(curfile, files[i].c_str());
        //     TraceSimulator *ts = new TraceSimulator(curfile, parse_args);
        //     FILE *fp;
        //     fp = fopen((files[i] + tail).c_str(), "w");
        //     printf(curfile);
        //     ts->cur_file = fp;
        //     ts->doCommands();
        //     fclose(fp);
        //     delete ts;
        // }
        // string str_LRU = "LRU_";

    }
    
    return 0;
}