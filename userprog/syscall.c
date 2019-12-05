#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "../threads/interrupt.h"
#include "../threads/thread.h"
#include "../threads/init.h"
#include "../filesys/directory.h"
#include "../filesys/file.h"
#include "../filesys/free-map.h"
#include "../filesys/inode.h"
#include "../filesys/directory.h"

static struct lock filesysLock;


static void syscall_handler (struct intr_frame *);
void sysExit(int);
int open(void*);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}
//syscall handler takes interrupt frame as a pointer on the stack
void syscall_handler (struct intr_frame *f)
{
  int sysCode = *(int*)f->esp;

  char *fileName =  *(char**)(f->esp + 4);
  switch(sysCode) {

    case SYS_HALT : //Halt system call
      //printf ("SYS_HALT (%d)\n", sysCode); //DEBUG
      shutdown_power_off(); //Powers off the system
      break;

    case SYS_EXIT : ;
      int status = *(int*)f->esp + 4; //Gets exit status
      //printf ("Status: (%d)\n", status); //DEBUG
      struct thread *cur = thread_current(); //Get current thread pointer
      cur->thread_exit_code = status; //Set exit code in thread struct
      thread_exit();

      break;

    case SYS_OPEN: ;
      //printf ("SYS_OPEN (%d)\n", sysCode); //DEBUG
      struct file *currentFile = filesys_open(fileName); //Get file pointer
      struct thread *currentThread = thread_current(); //Get current thread pointer
      int fileDescriptor = 0;

      if (currentFile == NULL) { //If file exists
        fileDescriptor = -1;
      }
      f->eax = fileDescriptor; //Set file open status
      break;

    case SYS_REMOVE : ;
      //printf ("SYS_REMOVE (%s)\n", fileName); //DEBUG
      bool isRemoved = filesys_remove(fileName); //Call file remove()
      f->eax = isRemoved; //Set removed status
      break;

    case SYS_CREATE : ;
      bool iscreated = filesys_create(fileName, f->esp + 8); //Create file
      f->eax = iscreated; //Set created status
      break;

    default:
    printf ("Syscode=%d\n", sysCode); //DEBUG
    printf("! Syscall default!\n"); //DEBUG
    break;
  }
  //thread_exit(); //DEBUG
}
