enum proc_state { SUNUSED, SUSED, SSLEEPING, SRUNNABLE, SRUNNING, SZOMBIE };

struct procinfo {
    int ppid;
    enum proc_state state;
    char name[16];
};