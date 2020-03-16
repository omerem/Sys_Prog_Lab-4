#include "util.h"
#include <errno.h>

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_CREATE 8
#define SYS_LSEEK 19
#define SYS_GETDENTS 141

#define O_RDONLY 0
#define O_RDRW 2
#define O_CREATE 64


#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define EOF -1

#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12


int debugSys = 0;
int debug=0;
int fromFile=0;
int toFile=0;
int inputFile = STDIN;
int outputFile = STDOUT;
char * inputFileName = "STDIN";
char * outputFileName = "STDOUT";
char * prefix = "";
int prefixLength = 0;
int usePrefix = 0;

int myOpen(const char *fileName, int flag);
int myGetdents(int fd, char * buffer, int size);
int myOpen2(const char *fileName);
int myClose(int fileDescriptor);
int myStderr(char * str);
int myWriteChar(char * str);
int myWrite(int file, char * str);
int mySeek(int fileDescriptor, long int offset, int whence);
int myRead(char * buf);
int myCreate(char * fileName);
int ptintTypeName(char d_type);

int printCurPos(int fileDescriptor);
void prPar(int n, char * name);
void prStr(char * str);
int task1b(int argc, char **argv);
int task2a(int argc, char **argv);

struct linux_dirent {
    unsigned long  d_ino;     /* Inode number */
    unsigned long  d_off;     /* Offset to next linux_dirent */
    unsigned short d_reclen;  /* Length of this linux_dirent */
    char           d_name[];  /* Filename (null-terminated) */
                /* length is actually (d_reclen - 2 -
                offsetof(struct linux_dirent, d_name)) */
           } ent;


int task2b(int argc, char **argv)
{
    char  buffer[8192];
    int fd = myOpen2(".");
    int read = myGetdents(fd, buffer, 8192);
    char d_type;
    char * name;
    int i;
    int cmp;
  for (i=0; i<read;)
  {
      struct linux_dirent * dir =(struct linux_dirent *)(buffer + i);
      name = dir->d_name;
      cmp = strncmp (name, prefix, prefixLength);
      if (cmp == 0)
      {
        myWrite(STDOUT, name);
        myWrite(STDOUT, ": ");
        d_type = *(buffer + i + dir->d_reclen - 1);
        ptintTypeName(d_type);
        myWrite(STDOUT, "\n");
      }
      i+=dir->d_reclen;
  }
      
    return 0;
}

int ptintTypeName(char d_type)
{
    char * str;
    if (d_type == DT_REG) {str = "regular";}
    else if (d_type == DT_DIR) {str =  "directory";}
    else if (d_type == DT_FIFO) {str = "FIFO";}
    else if (d_type == DT_SOCK) {str = "socket";}
    else if (d_type == DT_LNK) {str = "symlink";}
    else if (d_type == DT_BLK) {str = "block dev";}
    else if (d_type == DT_CHR) {str = "char dev";}
    else {str =  "???";}
    
    int status = myWrite(STDOUT, str);
    return status;
    
}



int myRead(char * buf)
{
    char * str;
    int status = system_call(SYS_READ,inputFile,buf,1);
    if(status < 0)
    {
        str = itoa(status);
        
        system_call(SYS_WRITE,STDERR,"Problem Reading From File. status = ", 36);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
        system_call(SYS_EXIT);
    }
    if (debugSys == 1)
    {
        system_call(SYS_WRITE,STDERR, "system call #3 (SYS_READ) ", 26);
        system_call(SYS_WRITE,STDERR, "Return code: ", 13);
        str = itoa(status);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
    }
    return status;
}
int myGetdents(int fd, char * buffer, int size)
{
    char * str;
    int status = system_call(SYS_GETDENTS, fd, buffer, 8192);
    if(status < 0)
    {
        str = itoa(status);
        
        system_call(SYS_WRITE,STDERR,"Problem GETDENTS. status = ", 27);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
        system_call(SYS_EXIT, 0x55);
    }
    if (debugSys == 1)
    {
        system_call(SYS_WRITE,STDERR, "system call #141 (SYS_GETDENTS) ", 32);
        system_call(SYS_WRITE,STDERR, "Return code: ", 13);
        str = itoa(status);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
    }
    return status;
}
int myOpen2(const char *fileName)
{
    char * str;
    int fileDescriptor = system_call(SYS_OPEN,fileName,0,0);

    if (fileDescriptor < 0)
    {
        str = itoa(fileDescriptor);
        
        system_call(SYS_WRITE,STDERR,"Problem Opening File. status = ", 31);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
        system_call(SYS_EXIT);
    }
    
    if (debugSys == 1)
    {
        system_call(SYS_WRITE,STDERR, "system call #5 (SYS_OPEN) ", 26);
        
        system_call(SYS_WRITE,STDERR, "Return code: ", 13);
        str = itoa(fileDescriptor);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
    }
    return fileDescriptor;
}
int myOpen(const char *fileName, int flag)
{
    char * str;
    int fileDescriptor = system_call(SYS_OPEN,fileName,flag,0777);

    if (fileDescriptor < 0)
    {
        str = itoa(fileDescriptor);
        
        system_call(SYS_WRITE,STDERR,"Problem Opening File. status = ", 31);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
        system_call(SYS_EXIT);
    }
    
    if (debugSys == 1)
    {
        system_call(SYS_WRITE,STDERR, "system call #5 (SYS_OPEN) ", 26);
        
        system_call(SYS_WRITE,STDERR, "Return code: ", 13);
        str = itoa(fileDescriptor);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
    }
    return fileDescriptor;
}
int myClose(int fileDescriptor)
{
    char * str;
    int status = system_call(SYS_CLOSE,fileDescriptor);
    if (status < 0)
    {
        system_call(SYS_WRITE,STDERR,"Problem Closing File\n", 21);
        system_call(SYS_EXIT);
    }
    
    if (debugSys == 1)
    {
        system_call(SYS_WRITE,STDERR, "system call #6 (SYS_CLOSE) ", 27);
        
        system_call(SYS_WRITE,STDERR, "Return code: ", 13);
        str = itoa(status);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
    }
    return status;
}
int myStderr(char * str)
{
    char * str0;
    int status = system_call(SYS_WRITE,STDERR,str, strlen(str));
    if (status == -1)
    {
        system_call(SYS_WRITE,STDERR,"Problem Writing to STDERR\n", 26);
        system_call(SYS_EXIT);
    }
    if (debugSys == 1)
    {
        system_call(SYS_WRITE,STDERR, "system call #4 (SYS_WRITE) ", 27);
        system_call(SYS_WRITE,STDERR, "Return code: ", 13);
        str0 = itoa(status);
        system_call(SYS_WRITE,STDERR, str0, strlen(str0));
        system_call(SYS_WRITE,STDERR, "\n", 1);
    }
    return status;
}
int myWrite(int file, char * str)
{
    char * str0;
    int status = system_call(SYS_WRITE, file, str, strlen(str));
    if (status == -1)
    {
        system_call(SYS_WRITE,STDERR,"Problem Writing\n", 16);
        system_call(SYS_EXIT);
    }
    if (debugSys == 1)
    {
        system_call(SYS_WRITE,STDERR, "system call #4 (SYS_WRITE) ", 27);
        system_call(SYS_WRITE,STDERR, "Return code: ", 13);
        str0 = itoa(status);
        system_call(SYS_WRITE,STDERR, str0, strlen(str0));
        system_call(SYS_WRITE,STDERR, "\n", 1);
    }
    return status;
    
}
/* PRINTS ONLY A CHAR */
int myWriteChar(char * str)
{
    char *str0;
    
    int status = system_call(SYS_WRITE,outputFile,str, 1);
    
        
    if (status < 0)
    {
        system_call(SYS_WRITE,STDERR,"Problem Writing to ", 19);
        system_call(SYS_WRITE, STDERR, outputFileName, strlen(outputFileName));
        system_call(SYS_WRITE,STDERR,"\n", 1);
        system_call(SYS_EXIT);
    }
    if (debugSys == 1)
    {
        system_call(SYS_WRITE,STDERR, "system call #4 (SYS_WRITE) ", 27);
        system_call(SYS_WRITE,STDERR, "Return code: ", 13);
        str0 = itoa(status);
        system_call(SYS_WRITE,STDERR, str0, strlen(str0));
        system_call(SYS_WRITE,STDERR, "\n", 1);
        
    }
    return status;
}
int mySeek(int fileDescriptor, long int offset, int whence)
{
    char * str;
    int status = system_call(SYS_LSEEK,fileDescriptor,offset, whence);
    if (status < 0)
    {
        system_call(SYS_WRITE,STDERR,"Problem Seeking File\n", 21);
        system_call(SYS_EXIT);
    }
    if (debugSys == 1)
    {
        system_call(SYS_WRITE,STDERR, "system call #19 (SYS_LSEEK) ", 28);
        system_call(SYS_WRITE,STDERR, "Return code: ", 13);
        str = itoa(fileDescriptor);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
    }
    return status;
}
int myCreate(char * fileName)
{
    char * str;
    int fileDescriptor = system_call(SYS_OPEN,fileName,65,0777);
    
    if (fileDescriptor < 0)
    {
        str = itoa(fileDescriptor);
        
        system_call(SYS_WRITE,STDERR,"Problem Opening File. status = ", 31);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
        system_call(SYS_EXIT);
    }
    
    if (debugSys == 1)
    {
        system_call(SYS_WRITE,STDERR, "system call #5 (SYS_OPEN) ", 26);
        
        system_call(SYS_WRITE,STDERR, "Return code: ", 13);
        str = itoa(fileDescriptor);
        system_call(SYS_WRITE,STDERR, str, strlen(str));
        system_call(SYS_WRITE,STDERR, "\n", 1);
    }
    return fileDescriptor;
}


int printCurPos(int fileDescriptor)
{
    int cur = mySeek(fileDescriptor, 0, SEEK_CUR);
    return cur;
}

void prPar(int n, char * name)
{
    char * str = itoa(n);
    system_call(SYS_WRITE,STDERR,name,strlen(name));
    system_call(SYS_WRITE,STDERR," = ",3);
    system_call(SYS_WRITE,STDERR,str,strlen(str));
    system_call(SYS_WRITE,STDERR,"\n",1);
}

void prStr(char * str)
{
    system_call(SYS_WRITE,STDERR,str, strlen(str));
    system_call(SYS_WRITE,STDERR,"\n", 1);
}

int task1b(int argc, char **argv)
{
    debugSys = 0;
    int debug1 = 0;
    debug = 0;
    fromFile = 0;
    toFile = 0;
    int i;
    int status;
    char * str;
    for(i=1; i<argc; i++)
    {
        if(strncmp(argv[i],"-D",2)==0)
        {
            debug = 1;
            /*
            debugSYS = 1;
            debug1 = 1;
            */
        }
        if(strncmp(argv[i],"-i",2)==0)
        {
            fromFile = 1;
            inputFileName = argv[i]+2;
            inputFile = myOpen(inputFileName, O_RDRW);
        }
        if(strncmp(argv[i],"-o",2)==0)
        {
            toFile = 1;
            outputFileName = argv[i]+2;
            outputFile = myCreate(outputFileName);
        }        
    }
    if(debug == 1)
    {
        myStderr("Input: ");
        myStderr(inputFileName);
        myStderr(".\nOutput: ");
        myStderr(outputFileName);
        myStderr("\n");
    }
    
    char ch;
    status = myRead(&ch);
    while(status != 0)
    {
        if(debug1 == 1)
        {
            str = itoa(ch);
            myStderr(str);
        }
        if ((65 <= ch) && (ch <= 90))
        {
            ch+=32;
        }
        
        myWriteChar(&ch);
        if(debug1 == 1)
        {
            str = itoa(ch);
            myStderr(str);
        }
        
        status = myRead(&ch);
    }
    
    if(fromFile == 1)
    {
        myClose(inputFile);
    }
    
    return 0;
}

    
int main (int argc , char* argv[], char* envp[])
{
    
    int i=0;
    for(i=1; i<argc; i++)
    {
        if(strncmp(argv[i],"-D",2)==0)
        {
            debug = 1;
            debugSys = 1;
            debug1 = 1;
//             /*
            */
        }
        if(strncmp(argv[i],"-i",2)==0)
        {
            fromFile = 1;
            inputFileName = argv[i]+2;
            inputFile = myOpen(inputFileName, O_RDRW);
        }
        if(strncmp(argv[i],"-o",2)==0)
        {
            toFile = 1;
            outputFileName = argv[i]+2;
            outputFile = myCreate(outputFileName);
        }                
        if(strncmp(argv[i],"-p",2)==0)
        {
            usePrefix = 1;
            prefix = argv[i]+2;
            prefixLength = strlen(prefix);
        }        
    }
    
    
    
    
    
    return task2b(argc, argv);
    
    
    
    return 0;
}










