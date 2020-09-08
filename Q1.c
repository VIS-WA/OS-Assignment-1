
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/stat.h>
#include <string.h>

void reverse(char* buf,int x)
{
  int l=x/sizeof(char), i=0;
  char *b, *e, tmp;
  b= buf;
  e= buf+l-1;
  for(i=0;i<l/2;i++)
  {
    tmp= *e;
    *e= *b;
    *b=tmp;
    b++;e--;
  }  
}

int isDir(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

int main(int argc, char *argv[])
{

  int fdin, fdout, sz, sc=sizeof(char);
  long long  net=0, written=0;
  char BUFFER[1000000];
  char str[100];
  char str1[10000];
  if( argc < 2 ){
    sprintf(str, "Please provide input file\n");
    write(1,str,strlen(str));
    return -1;
  }

  else if( argc > 2 ){
    sprintf(str,"Too many Arguments!\n");
    write(1,str,strlen(str));
    return -1;
  }

  
  int check = isDir("./Assignment");
  if(!check){
    check = mkdir("./Assignment",0777);
    if (!check){ 
      sprintf(str,"Assignment Directory doesent exist,hence created\n");
      write(1,str,strlen(str));
    } 
    else{
      sprintf(str,"Unable to create directory\n");
      write(1,str,strlen(str));
    }

  }
  int i=strlen(argv[1]);
  for(; argv[1][i]!='/' && i!=-1;i--);
  sprintf(str,"%s",argv[1]+i+1);
  
  fdin = open(str, O_RDONLY);
  sprintf(str1, "./Assignment/%s",str);
  fdout= open(str1, O_WRONLY | O_CREAT | O_TRUNC , 0644);
  
  
  
  if (fdin < 0){
    perror("Input file Error");
    exit(1);
  }

  if (fdout < 0){
    perror("Output file Error");
    exit(1);
  }
 
  net = lseek(fdin,0,SEEK_END);
  long long cur = net-1000000;

  while(cur > 0){
    lseek(fdin,cur,SEEK_SET);
    sz = read(fdin, BUFFER,1000000);
    reverse(BUFFER, sz);
    write(fdout, BUFFER, sz/sc);
    written += sz/sc;
    sprintf(str, "\rwriting...%d%% [ %d/%d ] done", (written*100/net), written, net);
    write(1,str,strlen(str));
    cur = lseek(fdin,0,SEEK_CUR)-2000000;
  }
  if (cur<0){
    lseek(fdin,0,SEEK_SET);
    sz = read(fdin, BUFFER,1000000+cur);
    reverse(BUFFER, sz);
    write(fdout, BUFFER, sz/sc);
    written += sz/sc;
    sprintf(str, "\rwriting...%d%% [ %d/%d ] done", (written*100/net), written,net);
    write(1,str,strlen(str));
  }
  close(fdin);
  close(fdout);
}
