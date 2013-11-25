#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

char* getRam(unsigned long size){

    char* str = malloc(sizeof(char)*10);
    if(size >= 1024){
       size/=1024;
    if(size >= 1024){ 
      size /=1024;
      if(size >= 1024){
         size/=1024;
         sprintf(str,"%ldGb",size);
      }
      else sprintf(str,"%ldMb",size);
    }
    else sprintf(str,"%ldKb",size);
  }
  else  sprintf(str,"%ldB",size);
  return str;
}

int main (){
 
  printf("\n\nHello User World!\n");
  int days, hours, mins;
  struct sysinfo sys_info;

  if(sysinfo(&sys_info) != 0)
    perror("Can't get sysinfo!");

  days = sys_info.uptime / 86400;
  hours = (sys_info.uptime / 3600) - (days * 24);
  mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);

  printf("Uptime: %ddays, %dhours, %dminutes, %ldseconds\n",
         days, hours, mins, sys_info.uptime % 60);
  
  printf("Total RAM: %s\n", getRam(sys_info.totalram));
  printf("Free  RAM: %s\n", getRam(sys_info.freeram));
  printf("Process count: %d\n", sys_info.procs);
  printf("Page size: %ldByte\n", sysconf(_SC_PAGESIZE));
  return 0;
}
