#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "../threads/interrupt.h"
#include "../threads/thread.h"
#include "../threads/init.h"

static void syscall_handler (struct intr_frame *);
void sysExit(int);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void syscall_handler (struct intr_frame *f)// UNUSED)
{
  //printf ("system call!\n");
  int sysCode = *(int*)f->esp;

  switch(sysCode) {
    case SYS_HALT :
    printf ("SYS_HALT (%d)\n", sysCode); //debug


      shutdown_power_off();
      break;

    case SYS_EXIT :
      sysExit(*(int*)f->esp + 4);
      printf ("SYS_EXIT (%d)\n", sysCode); //debug
      thread_exit();
      break;

    case SYS_WRITE :
    //printf ("SYS_WRITE (%d)\n", sysCode); //debug
      break;

    default:
    printf ("Syscode=%d\n", sysCode); //debug
    printf("! Syscall default!"); //debug
      //thread_exit();
  }

  void sysExit(int status)
  {
    struct thread *cur = thread_current();
    cur->thread_exit_code = status;
    thread_exit();
  }

  //thread_exit ();
}
