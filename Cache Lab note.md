# Cache Lab note

## Knowledge Preparation

the usage of enum

#idndef #define #endif

```c++
char myarray[5] = {'-', '1', '2', '3', '\0'};
int i;
sscanf(myarray, "%d", &i);
```

继承的时候再父类前面要加访问控制符

switch的用法

```c++
    switch (p_args[0]) {
        case SMALL:
            this->bs = 8;
            break;
        case MID:
            this->bs = 16;
            break;
        case BIG:
            this->bs = 32;
            break;
        default:
            printf("Invalid block size!\n");
            exit(-1);
    }
```



## astar.trace

* LRU, ALLOC_BACK

|          | 8B   | 32B  | 64B  |
| -------- | ---- | ---- | ---- |
| 全相联   |      |      |      |
| 直接映射 |      |      |      |
| 4-way    |      |      |      |
| 8-way    |      |      |      |



* 8B， 8-way, ALLOC_BACK

| LRU  | Random | Binary Tree |
| ---- | ------ | ----------- |
|      |        |             |



* 8B,  8-way, LRU

| ALLOC_THROUGH | ALLOC_BACK | UNALLOC_THROUGH | UNALLOC_BACK |
| ------------- | ---------- | --------------- | ------------ |
|               |            |                 |              |





## bzip2.trace

* LRU, ALLOC_BACK

|          | 8B       | 32B      | 64B      |
| -------- | -------- | -------- | -------- |
| 全相联   |          |          |          |
| 直接映射 | 0.983915 | 0.987016 | 0.989315 |
| 4-way    | 0.982842 | 0.976349 | 0.972672 |
| 8-way    | 0.976194 | 0.966267 | 0.963118 |



* 8B， 8-way, ALLOC_BACK

| LRU      | Random   | Binary Tree |
| -------- | -------- | ----------- |
| 0.976194 | 0.996596 | 0.994113    |



* 8B,  8-way, LRU

| ALLOC_THROUGH | ALLOC_BACK | UNALLOC_THROUGH | UNALLOC_BACK |
| ------------- | ---------- | --------------- | ------------ |
| 0.976194      | 0.976194   | 0.820912        | 0.820912     |



## mcf.trace

* LRU, ALLOC_BACK

|          | 8B   | 32B  | 64B  |
| -------- | ---- | ---- | ---- |
| 全相联   |      |      |      |
| 直接映射 |      |      |      |
| 4-way    |      |      |      |
| 8-way    |      |      |      |



* 8B， 8-way, ALLOC_BACK

| LRU  | Random | Binary Tree |
| ---- | ------ | ----------- |
|      |        |             |



* 8B,  8-way, LRU

| ALLOC_THROUGH | ALLOC_BACK | UNALLOC_THROUGH | UNALLOC_BACK |
| ------------- | ---------- | --------------- | ------------ |
|               |            |                 |              |



## perlbench.trace

* LRU, ALLOC_BACK

|          | 8B   | 32B  | 64B  |
| -------- | ---- | ---- | ---- |
| 全相联   |      |      |      |
| 直接映射 |      |      |      |
| 4-way    |      |      |      |
| 8-way    |      |      |      |



* 8B， 8-way, ALLOC_BACK

| LRU  | Random | Binary Tree |
| ---- | ------ | ----------- |
|      |        |             |



* 8B,  8-way, LRU

| ALLOC_THROUGH | ALLOC_BACK | UNALLOC_THROUGH | UNALLOC_BACK |
| ------------- | ---------- | --------------- | ------------ |
|               |            |                 |              |

