#ifndef HEADER_H
#define HEADER_h

#include "trace_simulator.h"

class Replacer { 

    public:
        TraceSimulator *ts;

        Replacer();
        int init();
        int doReplace(uint index, ulong tag);
        int doUpdate(uint index, uint pos);
        ~Replacer();
};

class LRU_Replacer: public Replacer {
    
    unsigned char **LRU_qs; // The head represents the least recently used entry

    public:
        LRU_Replacer();
        int init();
        int doReplace(uint index, ulong tag); // Index of the cache line
        int doUpdate(uint index, uint pos);
        ~LRU_Replacer();
};

class RANDOM_Replacer: public Replacer {
    public:
        RANDOM_Replacer();
        int init();
        int doReplace(uint index, ulong tag);
        int doUpdate(uint index, uint pos);
        ~RANDOM_Replacer();
};

class BINARY_TREE_Replacer: public Replacer {

    unsigned char *trees;

    public:
        BINARY_TREE_Replacer();
        int init();
        int doReplace(uint index, ulong tag);
        int doUpdate(uint index, uint pos);
        ~BINARY_TREE_Replacer();
};

#endif