#include "headers.h"

TraceSimulator::TraceSimulator(char *trace_f_name, unsigned int *p_args) {
    this->addrs = new unsigned long long[600000];
    this->is_read = new bool[600000];
    traceFileParse(trace_f_name);

    this->max_dir_size = CACHE_SIZE / 0x8;
    this->dir_size = 0;
    
    // The first entry is just a flag, doesn't contain a entry info
    this->dir_head = new LinkEntry;
    this->dir_head->next = this->dir_head;
    this->dir_head->prev = this->dir_head;

    // Initialize hit num
    this->nb_hit = 0;

    // Blocksize
    switch (p_args[0]) {
        case SMALL:
            this->bs = 8;
            break;
        case MID:
            this->bs = 32;
            break;
        case BIG:
            this->bs = 64;
            break;
        default:
            printf("Invalid block size!\n");
            exit(-1);
    }

    // Replacement policy
    switch (p_args[1]) {
        case LRU:
            this->r = new LRU_Replacer();
            break;
        case RANDOM:
            this->r = new RANDOM_Replacer();
            break;
        case BINARY_TREE:
            this->r = new BINARY_TREE_Replacer();
            break;
        default:
            printf("Invalid Replacement Policy!\n");
            exit(-1);        
    }

    // Write policy
    switch (p_args[2]) {
        case ALLOC_THROUGH:
            this->w = new ALLOC_THROUGH_Writer();
            break;
        case ALLOC_BACK:
            this->w = new ALLOC_BACK_Writer();
            break;
        case UNALLOC_THROUGH:
            this->w = new UNALLOC_THROUGH_Writer();
            break;
        case UNALLOC_BACK:
            this->w = new UNALLOC_BACK_Writer();
            break;
        default:
            printf("Invalid Write Policy!\n");
            exit(-1);          
    }

    // Origanization
    switch (p_args[3]) {
        case FULL_ASS:
            this->cache_line_num = CACHE_SIZE / this->bs;
            this->cache = new unsigned char *[(int)this->cache_line_num];
            this->meta_size = 1;
            this->way_num = 1;
            this->offset_bit_width = 0;
            this->entry_size = this->meta_size + this->bs;
            for (int i = 0; i < this->cache_line_num; ++i) this->cache[i] = new unsigned char[(int)this->entry_size];
            break;
        case ONE_WAY:
            this->cache_line_num = CACHE_SIZE / this->bs;
            this->cache = new unsigned char *[(int)this->cache_line_num];
            // Add 1 byte for valid bit and dirty bit, doesn't need a tag
            this->way_num = 1;
            this->offset_bit_width = 0;
            this->index_bit_width = ceil(log(this->cache_line_num) / log(2));
            this->tag_bit_width = 64 - this->offset_bit_width - this->index_bit_width;
            this->meta_size = ceil((2 + 64 - log(cache_line_num * this->way_num) / log(2)) / 8);
            this->entry_size = this->meta_size + this->bs;
            for (int i = 0; i < this->cache_line_num; ++i) this->cache[i] = new unsigned char[(int)this->entry_size];
            break;
        case FOUR_WAY:
            this->cache_line_num = CACHE_SIZE / (this->bs * 4);
            this->cache = new unsigned char *[(int)this->cache_line_num];
            this->way_num = 4;
            this->offset_bit_width = 2;
            this->index_bit_width = ceil(log(this->cache_line_num) / log(2));
            this->tag_bit_width = 64 - this->offset_bit_width - this->index_bit_width;
            this->meta_size = ceil((2 + 64 - log(cache_line_num * this->way_num) / log(2)) / 8);
            this->entry_size = this->meta_size + this->bs;
            for (int i = 0; i < this->cache_line_num; ++i) this->cache[i] = new unsigned char[(int)(this->entry_size * this->way_num)];
            break;
        case EIGHT_WAY:
            this->cache_line_num = CACHE_SIZE / (this->bs * 8);
            this->cache = new unsigned char *[(int)this->cache_line_num];
            this->way_num = 8;
            this->offset_bit_width = 3;
            this->index_bit_width = ceil(log(this->cache_line_num) / log(2));
            this->tag_bit_width = 64 - this->offset_bit_width - this->index_bit_width;
            this->meta_size = ceil((2 + 64 - log(cache_line_num * this->way_num) / log(2)) / 8);
            this->entry_size = this->meta_size + this->bs;
            for (int i = 0; i < this->cache_line_num; ++i) this->cache[i] = new unsigned char[(int)(this->entry_size * this->way_num)];
            break;
        default:
            printf("Invalid Organization!");
            exit(-1);
    }

    this->r->ts = this;
    this->r->init();
    this->w->ts = this;
}

int TraceSimulator::traceFileParse(char *fname) {
    printf("h2\n");
    FILE *fp;
    fp = fopen(fname, "r");
    if (fp == NULL) return -1; 
    int cnt = 0;
    char tmp = 0;
    char read = 'r';
    unsigned long long tr = 0;
    printf("h3\n");
    while (fscanf(fp, "%c", &tmp) != EOF) {
        if (tmp == read) {
            this->is_read[cnt] = true;
            // printf("r ");
        }
        fscanf(fp, "%llx", &this->addrs[cnt]);
        fscanf(fp, "%llx", tr);
        // printf("%llx\n", this->addrs[cnt]);
        cnt++;
    }
    
    printf("%d\n", cnt);
    this->nb_commands = cnt;
    // Check if correctly read
    printf("%llx\n", this->addrs[0]);
    fclose(fp);
    return 0;
}

int TraceSimulator::doCommands() {
    printf("h4\n");
    printf("%d %d %d\n", this->way_num, this->index_bit_width, this->offset_bit_width);
    for (int i = 0; i < this->nb_commands; ++i) {
        // printf("%d\n", i);
        if (this->meta_size != 1) {
            // printf("h5\n");
            uint curIndex = 0;
            ulong curTag = 0;
            setBits(curIndex, this->addrs[i], (uint)0, (uint)this->offset_bit_width, (uint)this->index_bit_width);
            setBits(curTag, this->addrs[i], (uint)0, (uint)(this->offset_bit_width + this->index_bit_width), this->tag_bit_width);
            // printf("curIndex: %d\n", curIndex);
            // printf("curTag: %llx\n", curTag);
            uint cnt = 0;
            ulong tmpl = 0;
            uchar *tmpc = this->cache[curIndex];
            bool hit = false;
            while (cnt < this->way_num) {
                tmpl = 0;
                if (tmpc[0] & 1) {
                    setBits(tmpl, tmpc, 0, 2, this->tag_bit_width);
                    // printf("%llx\n", tmpl);
                    if (curTag == tmpl) {
                        // printf("halo\n");
                        hit = true;
                        ++(this->nb_hit);
                        if (this->is_read[i]) {
                            this->r->doUpdate(curIndex, cnt); // Update the LRU stack
                        } else {
                            this->w->doWrite(curIndex, cnt);  // Do write
                        }
                        break;
                    }
                }
                tmpc += this->entry_size;
                ++cnt;
            }
            if (!hit) {
                if (this->is_read[i]) {
                    // printf("h6\n");
                    this->r->doReplace(curIndex, curTag); // Replacement including an update operation
                    this->r->doUpdate(curIndex, 0);
                } else {
                    // printf("h7\n");
                    this->w->doMissingWrite(curIndex, curTag); // Do missing write
                }
            }
        } else {
            // Full-Associated case
            uint cnt = 0;
            bool hit = false;
            while (cnt < this->dir_size) {
                LinkEntry *cur = this->dir_head->next;
                ulong curMM = 0;
                setBits(curMM, cur->entry_content, 0 , 0, 64);
                if (curMM == this->addrs[i]) {
                    ++(this->nb_hit);
                    deleteEntry(cur);
                    insertEntry(cur, this->dir_head->prev, this->dir_head);
                    uint curIndex = 0;
                    setBits(curIndex, cur->entry_content + 8, 0, 0, 14);
                    this->cache[curIndex][0] |= 0x1;
                    this->cache[curIndex][0] |= 0x2;
                    hit = true;
                    break;
                } 
            }
            if (!hit) {
                if (this->dir_size < this->max_dir_size) {
                    // Insert into the dir
                    LinkEntry *cur = new LinkEntry;
                    insertEntry(cur, this->dir_head->prev, this->dir_head);
                    uint curIndex = 8 * (this->dir_size);
                    this->cache[curIndex][0] |= 0x1;
                    this->cache[curIndex][0] &= ~0x2;
                    setBits(cur->entry_content, this->addrs[i], 0 , 0, 64);
                    setBits(cur->entry_content + 8, curIndex, 0, 0, 14);
                    ++(this->dir_size);  
                } else {
                    // Replace
                    LinkEntry *cur = this->dir_head->next;
                    deleteEntry(cur);
                    insertEntry(cur, this->dir_head->prev, this->dir_head);
                    setBits(cur->entry_content, this->addrs[i], 0 , 0, 64);
                    uint curIndex = 0;
                    setBits(curIndex, cur->entry_content + 8, 0, 0, 14);
                    this->cache[curIndex][0] |= 0x1;
                    this->cache[curIndex][0] &= ~0x2;
                }
            }
        }
    }

    printf("The hiting rate is: %lf", ((double)this->nb_hit) / this->nb_commands);
    return 0;
}

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