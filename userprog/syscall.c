#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void syscall_handler (struct intr_frame *f)// UNUSED)
{
  printf ("system call!\n");
  int sysCode = *(int*)f->esp;
  printf ("f=%d\n", sysCode); //debug

  switch(sysCode) {
    case SYS_HALT :
      shutdown_power_off();
      break;

    case SYS_EXIT :
      thread_exit();
      break;

    case SYS_WRITE :

      break;

    default:
    prinf("!Error Syscall default!"); //debug
      thead_exit();
  }

  //thread_exit ();
}
