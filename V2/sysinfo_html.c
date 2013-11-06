#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (){
 

  int days, hours, mins;
  struct sysinfo sys_info;

  if(sysinfo(&sys_info) != 0){
    perror("Can't get sysinfo!");
    return 0;
  }
  days = sys_info.uptime / 86400;
  hours = (sys_info.uptime / 3600) - (days * 24);
  mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);

  printf("<table>");
  printf("<tr><td>Uptime</td><td>%ddays, %dhours, %dminutes, %dseconds</tr>\n",
         days, hours, mins, (sys_info.uptime % 60));
  
  printf("<tr><td>Total Ram</td><td>%ldByte</td></tr>\n", sys_info.totalram);
  printf("<tr><td>Free RAM</td><td>%ldByte</td></tr>\n", sys_info.freeram);
  printf("<tr><td>Process count</td><td>%dByte</td></tr>\n", sys_info.procs);
  printf("<tr><td>Page size</td><td>%ldByte</td></tr>\n", sysconf(_SC_PAGESIZE));
  printf("</table>");
  return 0;
}
