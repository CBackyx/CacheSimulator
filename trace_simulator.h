#ifndef TRACE_SIMULATOR
#define TRACE_SIMULATOR

class Replacer;
class Writer;
struct LinkEntry;

// This struct is only used for LRU Directory for Full-Associated origanization
struct LinkEntry {
    unsigned int prev;
    unsigned int next;
    unsigned char entry_content[10]; // Includes Main-Memory number, Cache number 
};

class TraceSimulator {
    
    public:

        Replacer *r;
        Writer *w;
        unsigned int bs;
        FILE *cur_file;

        unsigned long long *addrs;
        unsigned int nb_commands;
        bool *is_read;

        unsigned char **cache; 
        
        LinkEntry *dir_resources;
        unsigned int resources_now;
        unsigned int dir_head; // LRU Directory used for Full-Associated organization
        unsigned int addr_len;
        unsigned int dir_entry_content_size;

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
        ~TraceSimulator() {
            if (addrs != NULL) delete []addrs;
            if (is_read != NULL) delete []is_read;
            if (cache != NULL) {
                for (int i = 0; i < this->cache_line_num; ++i) delete []cache[i];
                delete []cache;
            }
            if (dir_resources != NULL) delete []dir_resources;
        }

        int traceFileParse(char *trace_f_name);
        int doCommands();
};

void insertEntry(LinkEntry *s, unsigned int cur, unsigned int p, unsigned int n);
void deleteEntry(LinkEntry *s, unsigned int cur);

#endif
