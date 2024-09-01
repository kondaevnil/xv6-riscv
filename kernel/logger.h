enum logtype {SYSCALL, SWITCHING, INTERRUPT, EXEC};

struct interval {
    uint start, end;
    int inf;
};

struct logger {
    struct spinlock lck;
    struct interval syscall;
    struct interval switching;
    struct interval interrupt;
    struct interval exec;
};