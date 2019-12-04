#include <stdio.h>
#include "../../lib/user/syscall.h"

int
main (void)
{
    //printf("Hello, World\n");
    //halt();
    create("myfile.txt",32);
    return EXIT_SUCCESS;
}
