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

static struct lock fileSysLock;



static void syscall_handler (struct intr_frame *);
void sysExit(int);
int open(void*);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void syscall_handler (struct intr_frame *f)// UNUSED)
{
  //printf ("system call!\n");
  int sysCode = *(int*)f->esp;

  //int sysCode = 6; //DEBUG FORCED ON 3!! DEBUG

  switch(sysCode) {
    case SYS_HALT :
      printf ("SYS_HALT (%d)\n", sysCode); //DEBUG
      shutdown_power_off();
      break;

    case SYS_EXIT : ;
      int status = *(int*)f->esp + 4;
      struct thread *cur = thread_current();
      cur->thread_exit_code = status;
      thread_exit();
      printf ("SYS_EXIT (%d)\n", sysCode); //DEBUG
      break;

    case SYS_EXEC :
      printf ("SYS_EXEC (%d)\n", sysCode); //DEBUG
      char *execFileName =  *(char**)(f->esp + 4);
      printf("EXEC_NAME=%s\n", execFileName );
      break;

    case SYS_OPEN: ;
    printf ("SYS_OPEN (%d)\n", sysCode);
      //char *fileName = (char**)(f->esp + 4);
      f->eax = open((char**)(f->esp + 4));

      //printf ("FileName (%s)\n", fileName);
      break;

    case SYS_WRITE : ;
      printf ("SYS_WRITE (%d)\n", sysCode);
      int fileDescriptor =   *((int*)f->esp + 1);
      void *buffer = (void*)(*((int*)f->esp + 2));
      unsigned size = *((unsigned *)f->esp + 3);

      //f->eax = write(fileDescriptor, buffer, size)
      break;

    default:
    printf ("Syscode=%d\n", sysCode); //DEBUG
    printf("! Syscall default!\n"); //DEBUG
    break;
  }
  thread_exit();
}

int open (void *esp)
{
  const char *fileName =  *(char**)(esp);
  esp += sizeof (char *);

  printf ("FileName= (%s)\n", fileName);
  struct file* file = filesys_open(fileName);
  struct thread *cur = thread_current ();
  int fileDescriptor;

  if (file == NULL){
      fileDescriptor = -1;
      return fileDescriptor;
  }
}
