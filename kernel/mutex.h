struct mutex {
    struct sleeplock mutex_lock; // mutex logic lock
    struct spinlock lock; // mutex table access lock
    int ucount; // count of processes using mutex
};