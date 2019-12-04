#include <stdio.h>
#include "../../lib/user/syscall.h"

int
main (void)
{
    //printf("Hello, World\n");
    //halt();
    remove("insult");
    return EXIT_SUCCESS;
}
