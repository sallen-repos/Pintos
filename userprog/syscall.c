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
void syscall_handler (struct intr_frame *f)// UNUSED)
{
  int sysCode = *(int*)f->esp;

  char *fileName =  *(char**)(f->esp + 4);
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

      printf("EXEC_NAME=%s\n", fileName );
      break;

    case SYS_OPEN: ;
    printf ("SYS_OPEN (%d)\n", sysCode);
      //char *fileName = (char**)(f->esp + 4);
      // open((char**)(f->esp + 4));
      struct file *currentFile = filesys_open(fileName);
      struct thread *currentThread = thread_current();
      int fileDescriptor = 0;

      if (currentFile == NULL) {
        fileDescriptor = -1;
      }
      f->eax = fileDescriptor;
      break;

    case SYS_WRITE : ;
      //inode_write_at(file->inode, buffer, size, file->pos)
      //file->pos += bytes_written;
      //f-eax = bytes_written;

      //  printf ("SYS_WRITE (%d)\n", sysCode);
      //  int fileDescriptor =   *((int*)f->esp + 1);
      //  void *buffer = (void*)(*((int*)f->esp + 2));
        //unsigned size = *((unsigned *)f->esp + 3);

        //f->eax = write(fileDescriptor, buffer, size)
      break;

    case SYS_SEEK : ;
      break;

    case SYS_REMOVE : ;
      //const char *fileName =  *(char**)(f->esp + 4);
        printf ("SYS_REMOVE (%s)\n", fileName);
      bool isRemoved = filesys_remove(fileName);
      f->eax = isRemoved;

      break;

    case SYS_CREATE : ;
      const char *createFileName =  *(char**)(f->esp + 4);
      bool iscreated;

      //lock_acquire(&filesysLock);
      iscreated = filesys_create(createFileName,f->esp + 8);
      //lock_release(&filesysLock);

      f->eax = iscreated;
      break;



    default:
    printf ("Syscode=%d\n", sysCode); //DEBUG
    printf("! Syscall default!\n"); //DEBUG
    break;
  }
  thread_exit();
}
