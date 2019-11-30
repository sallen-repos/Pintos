/* halt.c

   Simple program to test whether running a user program works.

   Just invokes a system call that shuts down the OS. */
#include <stdio.h>
#include <syscall.h>

int
main (void)
{
  halt ();
  /* not reached */
  //return EXIT_SUCCESS;
}
