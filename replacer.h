#ifndef REPLACER_H
#define REPLACER_H

class TraceSimulator;

class Replacer { 

    public:
        TraceSimulator *ts;
        unsigned char *trees;
        unsigned char **LRU_qs; // The head represents the least recently used entry
        unsigned int LRU_length;

        Replacer() {}
        virtual int init() {}
        virtual int doReplace(uint index, ulong tag) {}
        virtual int doUpdate(uint index, uint pos) {}
        ~Replacer() {
            if (trees != NULL) delete []trees;
            if (LRU_qs != NULL) {
                for (int i = 0; i < this->LRU_length; ++i) delete []LRU_qs[i];
                delete []LRU_qs;
            }
        }
};

class LRU_Replacer: public Replacer {

    public:
        LRU_Replacer() {}
        int init();
        int doReplace(uint index, ulong tag); // Index of the cache line
        int doUpdate(uint index, uint pos);
        ~LRU_Replacer() {}
};

class RANDOM_Replacer: public Replacer {
    public:
        RANDOM_Replacer() {}
        int init();
        int doReplace(uint index, ulong tag);
        int doUpdate(uint index, uint pos);
        ~RANDOM_Replacer() {}
};

class BINARY_TREE_Replacer: public Replacer {

    public:
        BINARY_TREE_Replacer() {}
        int init();
        int doReplace(uint index, ulong tag);
        int doUpdate(uint index, uint pos);
        ~BINARY_TREE_Replacer() {}
};

#endif