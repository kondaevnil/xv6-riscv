#include <stdarg.h>
#include "types.h"
#include "riscv.h"
#include "param.h"
#include "spinlock.h"
#include "defs.h"
#include "proc.h"

char msg_buf[BUFSIZE];
struct spinlock buf_lck;
uint64 head, tail;

uint64 nxt(uint64 p) {
    return (p + 1) % BUFSIZE;
}

uint64 tlnxt(void) {
    return (tail + 1) % BUFSIZE;
}

uint64 hdnxt(void) {
    return (head + 1) % BUFSIZE;
}

uint64 min(uint64 a, uint64 b) {
    return a > b ? a : b;
}

void
bufinit(void)
{
    head = 0;
    msg_buf[head] = '\n';
    tail++;
    initlock(&buf_lck, "buffer lock");
}

void
bufputc(char ch)
{
    if (tlnxt() == head) {
        head = hdnxt();
    }

    msg_buf[tail] = ch;
    tail = tlnxt();
}

static char digits[] = "0123456789abcdef";

static void
printint(int xx, int base, int sign)
{
    char buf[16];
    int i;
    uint x;

    if(sign && (sign = xx < 0))
        x = -xx;
    else
        x = xx;

    i = 0;
    do {
        buf[i++] = digits[x % base];
    } while((x /= base) != 0);

    if(sign)
        buf[i++] = '-';

    while(--i >= 0)
        bufputc(buf[i]);
}

static void
printptr(uint64 x)
{
    int i;
    bufputc('0');
    bufputc('x');
    for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
        bufputc(digits[x >> (sizeof(uint64) * 8 - 4)]);
}


void
pr_msg(const char *fmt, ...)
{
    acquire(&tickslock);
    uint t = ticks;
    release(&tickslock);

    va_list ap;
    int i, c;
    char *s;

    if (fmt == 0)
        panic("null fmt");

    acquire(&buf_lck);

    bufputc('[');
    printint(t, 10, 1);
    bufputc(']');
    bufputc(' ');

    va_start(ap, fmt);
    for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
        if(c != '%'){
            bufputc(c);
            continue;
        }
        c = fmt[++i] & 0xff;
        if(c == 0)
            break;
        switch(c){
            case 'd':
                printint(va_arg(ap, int), 10, 1);
                break;
            case 'x':
                printint(va_arg(ap, int), 16, 1);
                break;
            case 'p':
                printptr(va_arg(ap, uint64));
                break;
            case 's':
                if((s = va_arg(ap, char*)) == 0)
                s = "(null)";
                for(; *s; s++)
                    bufputc(*s);
                break;
            case '%':
                bufputc('%');
                break;
            default:
                bufputc('%');
                bufputc(c);
                break;
        }
    }
    va_end(ap);
    bufputc('\n');
    release(&buf_lck);
}

int
dmesg(char *dest, uint64 sz)
{
    int p;

    if (dest == 0) {
        return -1;
    }

    if (sz < 1)
        return -2;

    if (sz > BUFSIZE) {
        sz = BUFSIZE;
    } else {
        sz = sz - 1;
    }

    acquire(&buf_lck);

    p = head;

    while (msg_buf[p] != '\n') {
        p = nxt(p);
    }

    head = p;
    p = nxt(p);

    pagetable_t pt = myproc()->pagetable;

    uint64 fc = min(sz, BUFSIZE - head);
    uint64 sc = 0;

    if (head > tail) {
        if (copyout(pt, (uint64)dest, msg_buf + head, fc) != 0) {
            release(&buf_lck);
            return -3;
        }

        if (sz > fc) {
            sc = min(sz - fc, tail);
            if (copyout(pt, (uint64)dest, msg_buf, sc) != 0) {
                release(&buf_lck);
                return -3;
            }
        }
    } else {
        fc = min(tail - head, fc);
        if (copyout(pt, (uint64)dest, msg_buf + head, fc) != 0) {
            release(&buf_lck);
            return -3;
        }
    }

    release(&buf_lck);

    char end = '\0';
    if (copyout(pt, (uint64)(dest + fc + sc), &end, 1) != 0) {
        return -3;
    }

    return fc + sc + 1;
}
