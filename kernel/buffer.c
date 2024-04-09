#include "buffer.h"
#include "memlayout.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"
#include <stdarg.h>
#include <stddef.h>

struct kernel_logs buffer;

const uint buf_size = BUFPAGES * PHYSPAGESIZE;

void
bufferinit(void)
{
    initlock(&buffer.lock, "kernel logging");
    buffer.tail = buffer.head;
}

static void
writebyte(char byte)
{
    *buffer.tail++ = byte;

    if (buffer.tail - buffer.head == buf_size)
        buffer.tail = buffer.head;
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
        writebyte(buf[i]);
}

static void
printptr(uint64 x)
{
    int i;
    consputc('0');
    consputc('x');
    for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
        writebyte(digits[x >> (sizeof(uint64) * 8 - 4)]);
}

void
pr_msg(const char *fmt, ...)
{
    acquire(&buffer.lock);

    acquire(&tickslock);
    uint64 cur_ticks = ticks;
    release(&tickslock);

    writebyte('[');
    printint(cur_ticks, 10, 0);
    writebyte(']');
    writebyte(' ');

    va_list ap;
    int i, c;
    char *s;

    if (fmt == 0)
        panic("null fmt");

    va_start(ap, fmt);
    for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
        if(c != '%'){
            writebyte(c);
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
                    writebyte(*s);
                break;
            case '%':
                writebyte('%');
                break;
            default:
                // Print unknown % sequence to draw attention.
                writebyte('%');
                writebyte(c);
                break;
        }
    }
    va_end(ap);

    writebyte('\n');

    release(&buffer.lock);
}

int
copy_buffer(char *buf, uint size)
{
    char null = '\0';

    if (size <= 0)
        return -1;

    if (buf == NULL)
        return -2;

    uint cs = (size < buf_size ? size : buf_size) - 1;

    acquire(&buffer.lock);
    if (copyout(myproc()->pagetable, (uint64)(buf), buffer.head, cs) < 0)
    {
        release(&buffer.lock);
        return -3;
    }

    if (copyout(myproc()->pagetable, (uint64)(buf + cs), &null, 1) < 0)
    {
        release(&buffer.lock);
        return -3;
    }

    release(&buffer.lock);

    return cs + 1;
}
