#include "headers.h"

TraceSimulator::TraceSimulator(char *trace_f_name, unsigned int *p_args) {
    this->addrs = new unsigned long long[600000];
    this->is_read = new bool[600000];
    traceFileParse(trace_f_name);

    this->dir_size = 0;
    this->addr_len = 0;
    
    bool needDir = false;

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
            needDir = true;
            this->cache_line_num = CACHE_SIZE / this->bs;
            this->cache = new unsigned char *[(int)this->cache_line_num];
            this->meta_size = 1;
            this->way_num = 1;
            this->offset_bit_width = ceil(log(this->bs) / log(2));
            this->index_bit_width = ceil(log(this->cache_line_num) / log(2));
            this->tag_bit_width = 64 - this->offset_bit_width;
            this->dir_entry_content_size = ceil(log(this->tag_bit_width + this->index_bit_width) / log(2));
            this->entry_size = this->meta_size + this->bs;
            this->addr_len = this->bs * 8;
            for (int i = 0; i < this->cache_line_num; ++i) this->cache[i] = new unsigned char[(int)this->entry_size];
            break;
        case ONE_WAY:
            this->cache_line_num = CACHE_SIZE / this->bs;
            this->cache = new unsigned char *[(int)this->cache_line_num];
            // Add 1 byte for valid bit and dirty bit, doesn't need a tag
            this->way_num = 1;
            this->offset_bit_width = ceil(log(this->bs) / log(2));
            this->index_bit_width =ceil(log(this->cache_line_num) / log(2));
            this->tag_bit_width = 64 - this->offset_bit_width - this->index_bit_width;
            this->meta_size = ((2 + this->tag_bit_width) >> 3) + 1;
            this->entry_size = this->meta_size + this->bs;
            for (int i = 0; i < this->cache_line_num; ++i) this->cache[i] = new unsigned char[(int)this->entry_size];
            break;
        case FOUR_WAY:
            this->cache_line_num = CACHE_SIZE / (this->bs * 4);
            this->cache = new unsigned char *[(int)this->cache_line_num];
            this->way_num = 4;
            this->offset_bit_width = ceil(log(this->bs) / log(2));
            this->index_bit_width = ceil(log(this->cache_line_num) / log(2));
            this->tag_bit_width = 64 - this->offset_bit_width - this->index_bit_width;
            this->meta_size = ((2 + this->tag_bit_width) >> 3) + 1;
            this->entry_size = this->meta_size + this->bs;
            for (int i = 0; i < this->cache_line_num; ++i) this->cache[i] = new unsigned char[(int)(this->entry_size * this->way_num)];
            break;
        case EIGHT_WAY:
            this->cache_line_num = CACHE_SIZE / (this->bs * 8);
            this->cache = new unsigned char *[(int)this->cache_line_num];
            this->way_num = 8;
            this->offset_bit_width = ceil(log(this->bs) / log(2));
            printf("offset %d\n",this->offset_bit_width);
            this->index_bit_width = ceil(log(this->cache_line_num) / log(2));
            printf("index %d\n",this->index_bit_width);
            this->tag_bit_width = 64 - this->offset_bit_width - this->index_bit_width;
            printf("tag %d\n",this->tag_bit_width);
            this->meta_size = ((2 + this->tag_bit_width) >> 3) + 1;
            printf("meta %d\n",this->meta_size);
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
    this->max_dir_size = this->cache_line_num;

    if (needDir) {
        this->dir_resources = new LinkEntry[this->cache_line_num + 1];
        this->resources_now = 1;
        this->dir_head = 0;
        this->dir_resources[0].next = 0;
        this->dir_resources[0].prev = 0;
    }
    // The first entry is just a flag, doesn't contain a entry info

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
    // printf("h4\n");
    // printf("%d %d %d\n", this->way_num, this->index_bit_width, this->offset_bit_width);
    for (int i = 0; i < this->nb_commands; ++i) {
        // if ((i & 0xfff) == 0) printf("%d\n", i);
        if (this->meta_size != 1) {
            // printf("h5\n");
            uint curIndex = 0;
            ulong curTag = 0;
            setBits(curIndex, this->addrs[i], (uint)0, (uint)this->offset_bit_width, (uint)this->index_bit_width);
            setBits(curTag, this->addrs[i], (uint)0, (uint)(this->offset_bit_width + this->index_bit_width), this->tag_bit_width);
            // printf("curIndex: %d\n", curIndex);
            // printf("curTag: %llx\n", curTag);
            // if (curIndex == 0x0ff) {
            //     printf("%llx ", curTag);
            //     if (this->is_read[i]) printf("r\n");
            //     else printf("w\n");
            // }
            uint cnt = 0;
            ulong tmpl = 0;
            uchar *tmpc = this->cache[curIndex];
            // if (curIndex == 0x0ff) {
            //     printf("current LRU queue: ");
            //     uint curOrder[8];
            //     for (int k = 0; k < 8; ++k) {
            //         curOrder[k] = 0;
            //         setBits(curOrder[k], this->r->LRU_qs[curIndex], 0, k*3, 3);
            //         printf("%d ", curOrder[k]);
            //     }
            //     printf("\n");
            // }
            bool hit = false;
            while (cnt < this->way_num) {
                tmpl = 0;
                if (tmpc[0] & 1) {
                    setBits(tmpl, tmpc, 0, 2, this->tag_bit_width);
                    // if (curIndex == 0x0ff) printf("%d + %llx ", cnt , tmpl);
                    if (curTag == tmpl) {
                        // if (cnt != 0) printf("cnt: %d\n", cnt);
                        // printf("%llx\n", tmpl);
                        // printf("halo\n");
                        // if (curIndex == 0x0ff) {
                        //     printf("Hit!!!: way is %d", cnt);
                        //     printf("\n");
                        // }
                        hit = true;
                        ++(this->nb_hit);
                        //fprintf(this->cur_file, "Hit\n");
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

            // if (curIndex == 0xff) printf("\n");
            if (!hit) {
                // if (curIndex == 0x0ff) {
                //     printf("Miss : "); 
                // }
                //fprintf(this->cur_file, "Miss\n");
                if (this->is_read[i]) {
                    // printf("h6\n");
                    // Replacement including an update operation

                    uint replaced = this->r->doReplace(curIndex, curTag);
                    // if (curIndex == 0x0ff) printf("%d replaced\n", replaced);
                    this->r->doUpdate(curIndex, replaced);
                } else {
                    // printf("h7\n");
                    this->w->doMissingWrite(curIndex, curTag); // Do missing write
                }
            }
        } else {
            // Full-Associated case
            // printf("h5\n");
            uint cnt = 0;
            bool hit = false;
            LinkEntry *s = this->dir_resources;
            uint cur = s[dir_head].next;
            while (cnt < this->dir_size) {
                ulong curMM = 0;
                ulong curTag = 0;
                setBits(curTag, this->addrs[i], (uint)0, (uint)(this->offset_bit_width), this->tag_bit_width);
                setBits(curMM, s[cur].entry_content, 0 , 0, this->tag_bit_width);
                if (curMM == curTag) {
                    // printf("h7\n");
                    ++(this->nb_hit);
                    deleteEntry(s, cur);
                    insertEntry(s, cur, s[dir_head].prev, this->dir_head);
                    uint curIndex = 0;
                    setBits(curIndex, s[cur].entry_content, 0, this->tag_bit_width, this->index_bit_width);
                    this->cache[curIndex][0] |= 0x1;
                    if (this->is_read[i]) this->cache[curIndex][0] |= 0x2;
                    hit = true;
                    break;
                } 
                cur = s[cur].next;
                ++cnt;
            }
            if (!hit) {
                // printf("h6\n");
                if (this->dir_size < this->max_dir_size) {
                    // Insert into the dir
                    uint cur = (this->resources_now ++);
                    insertEntry(s, cur, s[dir_head].prev, this->dir_head);
                    uint curIndex = (this->dir_size);
                    this->cache[curIndex][0] |= 0x1;
                    this->cache[curIndex][0] &= ~0x2;
                    setBits(s[cur].entry_content, this->addrs[i], 0 , (uint)(this->offset_bit_width), this->tag_bit_width);
                    setBits(s[cur].entry_content, curIndex, (uint)this->tag_bit_width, 0, this->index_bit_width);
                    ++(this->dir_size);  
                } else {
                    // Replace
                    uint cur = s[dir_head].next;
                    deleteEntry(s, cur);
                    insertEntry(s, cur, s[dir_head].prev, this->dir_head);
                    setBits(s[cur].entry_content, this->addrs[i], 0 , (uint)(this->offset_bit_width), this->tag_bit_width);
                    uint curIndex = 0;
                    // printf("%d\n", curIndex);
                    setBits(curIndex, s[cur].entry_content, 0, (uint)this->tag_bit_width, this->index_bit_width);
                    this->cache[curIndex][0] |= 0x1;
                    this->cache[curIndex][0] &= ~0x2;
                }
            }
        }
    }

    printf("The Missing rate is: %lf", 1.0 - ((double)this->nb_hit) / this->nb_commands);
    return 0;
}

// void insertEntry(LinkEntry* cur, LinkEntry* p, LinkEntry* n) {
//     p->next = cur;
//     cur->prev = p;
//     cur->next = n;
//     n->prev = p;
// };

// void deleteEntry(LinkEntry* cur) {
//     cur->prev->next = cur->next;
//     cur->next->prev = cur->prev;
// }

void insertEntry(LinkEntry *s, unsigned int cur, unsigned int p, unsigned int n) {
    s[p].next = cur;
    s[cur].prev = p;
    s[cur].next = n;
    s[n].prev = p;
};

void deleteEntry(LinkEntry *s, unsigned int cur) {
    s[s[cur].prev].next = s[cur].next;
    s[s[cur].next].prev = s[cur].prev;
}