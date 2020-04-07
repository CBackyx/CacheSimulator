#ifndef TRACE_SIMULATOR
#define TRACE_SIMULATOR

#include "headers.h"

class TraceSimulator {
    
    public:

        Replacer *r;
        Writer *w;
        unsigned int bs;

        unsigned long long *addrs;
        unsigned int nb_commands;
        bool *is_read;

        unsigned char **cache; 
        
        LinkEntry *dir_head; // LRU Directory used for Full-Associated organization
        unsigned int dir_size;
        unsigned int max_dir_size; 

        unsigned int cache_line_num;
        unsigned int way_num;
        unsigned int meta_size;
        unsigned int entry_size;
        unsigned int offset_bit_width;
        unsigned int index_bit_width;
        unsigned int tag_bit_width;

        unsigned int nb_hit;

        TraceSimulator(char *trace_f_name, unsigned int *p_args);
        ~TraceSimulator();
        int traceFileParse(char *trace_f_name);
        int doCommands();
};

// This struct is only used for LRU Directory for Full-Associated origanization
struct LinkEntry {
    LinkEntry *prev;
    LinkEntry *next;
    unsigned char entry_content[10]; // Includes Main-Memory number, Cache number 
};

void insertEntry(LinkEntry* cur, LinkEntry* p, LinkEntry* n) {
    p->next = cur;
    cur->prev = p;
    cur->next = n;
    n->prev = p;
};

void deleteEntry(LinkEntry* cur) {
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
}

#endif
