#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>


 void Get_all_interfaces_info(struct ifreq *ifr, short int total_interfaces);
 short int Print_info (struct ifreq *ifr, int fd, int argc, char *argv[], int total_interfaces);
 short int Get_info (struct ifreq *ifr, short int total_interfaces, int argc, char *argv[]);

char path[100], interfaces;


int main(int argc, char *argv[])
{


FILE *fp2,*fp;
 
short int  total_interfaces ,pos,i=0;

fp=popen("ls /sys/class/net","r");
fp2= popen("ls /sys/class/net | wc -l","r");

if (fp==NULL)
 perror("couldnt get interface names--->");
 
 if (fp2==NULL)
 perror("couldnt get total interface count---->");

 
 fread(path,sizeof(char),100,fp);
 fread(&interfaces,sizeof(char),1,fp2);
 total_interfaces= interfaces - 48;
 struct ifreq ifr[total_interfaces];

 //printf("content from the fread is ---->\n%s\ninterfaces=%d\n",path, total_interfaces);
 

 Get_info (ifr, total_interfaces, argc, argv);
 
 //Get_interface_names (ifr, total_interfaces);

pclose (fp);
pclose (fp2);

	return 0;
}


short int Print_info (struct ifreq *ifr, int fd, int argc, char *argv[], int total_interfaces)
{

 if (argc<=1)
 {
   for (--total_interfaces; total_interfaces >= 0; --total_interfaces)
   {
       if (ioctl(fd, SIOCGIFHWADDR, &ifr[total_interfaces])<0)
       {
          perror("EXITING the program...!!!couldnt get hardware address-->");
          return 1;
       }
 
       if (ioctl(fd, SIOCGIFMTU, &ifr[total_interfaces])<0)
       {          
          perror("EXITING the program...!!!couldnt get MTU--->");
          return 1;
       }  
        
       printf("mtu of %s=%d\t hwdaddrs=%X:%X:%X:%x:%x:%x\n",ifr[total_interfaces].ifr_name,ifr[total_interfaces].ifr_mtu,
       ifr[total_interfaces].ifr_hwaddr.sa_data[0],ifr[total_interfaces].ifr_hwaddr.sa_data[1],ifr[total_interfaces].ifr_hwaddr.sa_data[2],
       ifr[total_interfaces].ifr_hwaddr.sa_data[3],ifr[total_interfaces].ifr_hwaddr.sa_data[4],ifr[total_interfaces].ifr_hwaddr.sa_data[5]);
   } 
 
 }
 else 
 {
 	total_interfaces=0;
 	strncpy(ifr[total_interfaces].ifr_name,  argv[1], sizeof(argv[1]));
 	
 	if (ioctl(fd, SIOCGIFHWADDR, &ifr[total_interfaces])<0)
 	{
      perror("EXITING the program...!!!couldnt get hardware address-->");
      return 1;
   }
   if (ioctl(fd, SIOCGIFMTU, &ifr[total_interfaces])<0)
   {   
      perror("EXITING the program...!!!couldnt get MTU--->");
      return 1;
   }   
      
   printf("mtu of %s=%d\t hwdaddrs=%.2X:%.2X:%X:%x:%x:%x\n",ifr[total_interfaces].ifr_name,ifr[total_interfaces].ifr_mtu,
   ifr[total_interfaces].ifr_hwaddr.sa_data[0],ifr[total_interfaces].ifr_hwaddr.sa_data[1],ifr[total_interfaces].ifr_hwaddr.sa_data[2],
   ifr[total_interfaces].ifr_hwaddr.sa_data[3],ifr[total_interfaces].ifr_hwaddr.sa_data[4],ifr[total_interfaces].ifr_hwaddr.sa_data[5]);
}

}
 
 
 short int Get_info (struct ifreq *ifr, short int total_interfaces, int argc, char *argv[])
{
	int fd;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
 
   if (fd < 0)
   {
     perror("EXITING the program...!!!couldnt open socket--->");
     return 1;  
   }
    
   Get_all_interfaces_info (ifr, total_interfaces);
  
   if(Print_info(ifr, fd, argc, argv, total_interfaces))
   {
     return 1;
   }
     
   close(fd);

}
 
 void Get_all_interfaces_info(struct ifreq *ifr, short int total_interfaces)
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
			 	   //Get_info(ifr[total_interfaces].ifr_name, fd);
				 	//printf("ifr=%s\n",ifr[total_interfaces].ifr_name);
				 	i++;
				 	break; 
			 }
		}
	 }
  }


