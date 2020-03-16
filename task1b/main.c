#include "util.h"
#include <errno.h>

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_CREATE 8
#define SYS_LSEEK 19

#define O_RDRW 2
#define O_CREATE 64


#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define EOF -1

int debugSys;
int debug;
int fromFile;
int toFile;
int inputFile = STDIN;
int outputFile = STDOUT;
char * inputFileName = "STDIN";
char * outputFileName = "STDOUT";


int myOpen(const char *fileName, int flag);
int myClose(int fileDescriptor);
int myStderr(char * str);
int myWriteChar(char * str);
int mySeek(int fileDescriptor, long int offset, int whence);
int myRead(char * buf);
int myCreate(char * fileName);

int printCurPos(int fileDescriptor);
void prPar(int n, char * name);
void prStr(char * str);

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

int foo(int argc, char **argv)
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
            debugSys = 1;
            debug1 = 1;
            /*
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
    
    /*while(status != 0) */
    while (ch != '\n')
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
    return foo(argc, argv);
    /*
  int i;
  char * str;
  
  str = itoa(argc);
  system_call(SYS_WRITE,STDOUT, str,strlen(str));
  system_call(SYS_WRITE,STDOUT," arguments \n", 12);
  for (i = 0 ; i < argc ; i++)
    {
      system_call(SYS_WRITE,STDOUT,"argv[", 5);
	  str = itoa(i);
      system_call(SYS_WRITE,STDOUT,str,strlen(str));
      system_call(SYS_WRITE,STDOUT,"] = ",4);
      system_call(SYS_WRITE,STDOUT,argv[i],strlen(argv[i]));
      system_call(SYS_WRITE,STDOUT,"\n",1);
    }*/
    return 0;
}
