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

