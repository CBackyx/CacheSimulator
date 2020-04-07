#include "headers.h"

int LRU_Replacer::init() {
    if (this->ts->way_num == 8) {
        this->LRU_qs = new unsigned char *[this->ts->cache_line_num];
        for (int i = 0; i < this->ts->cache_line_num; ++i) {
            this->LRU_qs[i] = new unsigned char[3];
            for (int j = 0; j < 8; ++j) setBits(LRU_qs[i], (uint)j, (uint)j*3, (uint)0, (uint)3);
        }
    } else if (this->ts->way_num == 4) {
        this->LRU_qs = new unsigned char *[this->ts->cache_line_num];
        for (int i = 0; i < this->ts->cache_line_num; ++i) {
            this->LRU_qs[i] = new unsigned char[1];
            for (int j = 0; j < 4; ++j) setBits(LRU_qs[i], (uint)j, (uint)j*2, (uint)0, (uint)2);
        }
    } else {
        this->LRU_qs = NULL;
    }
    return 0;
}

int LRU_Replacer::doReplace(uint index, ulong tag) {
    if (this->ts->way_num == 8) {
        uint pos = LRU_qs[index][0] & 0x7;
        uchar *curc = (this->ts->cache)[index] + pos * this->ts->entry_size;
        setBits(curc, tag, 2, 0, this->ts->tag_bit_width);
        curc[0] |= 0x1; // Set valid
        curc[0] &= ~(0x2); // Set clean
    } else if (this->ts->way_num == 4) {
        uint pos = LRU_qs[index][0] & 0x3;
        uchar *curc = (this->ts->cache)[index] + pos * this->ts->entry_size;
        setBits(curc, tag, 2, 0, this->ts->tag_bit_width);
        curc[0] |= 0x1; // Set valid
        curc[0] &= ~(0x2); // Set clean
    } else {

    }

    return 0;
}

int LRU_Replacer::doUpdate(uint index, uint pos) {
    if (this->ts->way_num == 8) {
        uint curOrder[8];
        for (int i = 0; i < 8; ++i) {
            curOrder[i] = 0;
            setBits(curOrder[i], this->LRU_qs[index], 0, i*3, 3);
        }
        setBits(this->LRU_qs[index], curOrder[pos], 0, 0, 3);
        uint cnt = 1;
        for (int i = 0; i < 8; ++i) {
            if (i != pos) {
                setBits(this->LRU_qs[index], curOrder[pos], 3*cnt, 0, 3);
                ++cnt;
            }
        }
    } else if (this->ts->way_num == 4) {
        uint curOrder[4];
        for (int i = 0; i < 4; ++i) {
            curOrder[i] = 0;
            setBits(curOrder[i], this->LRU_qs[index], 0, i*2, 2);
        }
        setBits(this->LRU_qs[index], curOrder[pos], 0, 0, 2);
        uint cnt = 1;
        for (int i = 0; i < 4; ++i) {
            if (i != pos) {
                setBits(this->LRU_qs[index], curOrder[pos], 2*cnt, 0, 2);
                ++cnt;
            }
        }
    } else {

    }

    return 0;
}

int BINARY_TREE_Replacer::init() {
    if (this->ts->way_num == 8 || this->ts->way_num == 4) {
        this->trees = new unsigned char [this->ts->cache_line_num];
    } else {
        this->trees = NULL;
    }

    return 0;
}

