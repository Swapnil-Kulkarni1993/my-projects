#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>

char path[100], interfaces;


int main(int argc, char *argv[])
{


FILE *fp2,*fp;
 
short int  total_interfaces ,pos,i=0;

fp=popen("ls /sys/class/net","r");
fp2= popen("ls /sys/class/net | wc -l","r") ;

if (fp==NULL)
 perror("couldnt get interface names--->");
 
 if (fp2==NULL)
 perror("couldnt get total interface count---->");

 
 fread(path,sizeof(char),100,fp);
 fread(&interfaces,sizeof(char),1,fp2);
 total_interfaces= interfaces - 48;
 struct ifreq ifr[total_interfaces];

 printf("content from the fread is ---->\n%s\ninterfaces=%d\n",path, total_interfaces);
 

 //print_info (ifr, total_interfaces);
 
 Get_interface_names (ifr, total_interfaces);



	
}

/*
void Get_info (struct ifreq *ifr)
{

if (ioctl(fd, SIOCGIFHWADDR, &ifr)<0)
 perror("couldnt get hardware address-->");
 
 if (ioctl(fd, SIOCGIFMTU, &ifr)<0)
 perror("couldnt get hardware address--->");


}

void print_info (struct ifreq *ifr, short int total_interfaces)
{
  Get_interface_names(ifr, total_interfaces);
  

}

void open_socket ()
{

 int fd;
 fd = socket(AF_INET, SOCK_DGRAM, 0);
 
 if (fd <0)
 perror("couldnt open socket--->");
 
 }
 */
 
 void Get_interface_names(struct ifreq *ifr, short int total_interfaces)
 {
 	short int i=0;
 	for (--total_interfaces; total_interfaces >= 0; --total_interfaces)
	{ 
	  int pos=0;
	  while(path[i] != '\n')
	  { 	 
			 ifr[total_interfaces].ifr_name[pos++] = path[i];
			 i++;
			 if(path[i] == '\n') 
			 {
				 	printf("ifr=%s\n",ifr[total_interfaces].ifr_name);
				 	i++;
				 	break; 
			 }
		}
	}
}


