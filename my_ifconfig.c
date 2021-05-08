#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>


 void Get_all_interfaces_names(struct ifreq *ifr, short int total_interfaces);
 short int Print_info (struct ifreq *ifr, int argc, char *argv[], int total_interfaces);
 short int Get_info (struct ifreq *ifr, short int total_interfaces, int argc, char *argv[]);

char path[100], interfaces;


int main(int argc, char *argv[])
{
	FILE *fp2,*fp;
	 
	short int  total_interfaces ,pos,i=0;
	
	fp=popen("ls /sys/class/net","r");
	fp2= popen("ls /sys/class/net | wc -l","r");
	
	if (fp==NULL)
	 perror("couldnt get interface names\t");
	 
	 if (fp2==NULL)
	 perror("couldnt get total interface count\t");
	
	 
	 fread(path,sizeof(char),100,fp);
	 fread(&interfaces,sizeof(char),1,fp2);
	 total_interfaces= interfaces - 48;
	 struct ifreq ifr[total_interfaces];
	
	 //printf("content from the fread is ---->\n%s\ninterfaces=%d\n",path, total_interfaces);
	 
	
	 Get_info (ifr, total_interfaces, argc, argv);
	 Print_info(ifr, argc, argv, total_interfaces);
	 
	 //Get_interface_names (ifr, total_interfaces);
	
	pclose (fp);
	pclose (fp2);
	
	return 0;
}


short int Get_info (struct ifreq *ifr, short int total_interfaces, int argc, char *argv[])
{
	int fd;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
 
   if (fd < 0)
   {
     perror("EXITING the program...!!!couldnt open socket\t");
     return 1;  
   }
    
   Get_all_interfaces_names(ifr, total_interfaces);
   
   for (--total_interfaces; total_interfaces >= 0; --total_interfaces)
   {
       if (ioctl(fd, SIOCGIFHWADDR, &ifr[total_interfaces])<0)
       {
          perror("EXITING the program...!!!couldnt get hardware address\t");
          return 1;
       }
 
       if (ioctl(fd, SIOCGIFMTU, &ifr[total_interfaces])<0)
       {          
          perror("EXITING the program...!!!couldnt get MTU\t");
          return 1;
       }  
     /*   
       printf("mtu of %s=%d\t hwdaddrs=%02X:%02X:%02X:%02x:%02x:%02x\n",ifr[total_interfaces].ifr_name,ifr[total_interfaces].ifr_mtu,
       ifr[total_interfaces].ifr_hwaddr.sa_data[0],ifr[total_interfaces].ifr_hwaddr.sa_data[1],ifr[total_interfaces].ifr_hwaddr.sa_data[2],
       (unsigned char)ifr[total_interfaces].ifr_hwaddr.sa_data[3],ifr[total_interfaces].ifr_hwaddr.sa_data[4],ifr[total_interfaces].ifr_hwaddr.sa_data[5]);
     */
   }
     
   close(fd);

}

short int Print_info (struct ifreq *ifr, int argc, char *argv[], int total_interfaces)
{
 
 if (argc<=1)
 {
   for (--total_interfaces; total_interfaces >= 0; --total_interfaces)
   {
        
       printf("mtu of %s=%d\t hwdaddrs=%02X:%02X:%02X:%02x:%02x:%02x\n",ifr[total_interfaces].ifr_name,ifr[total_interfaces].ifr_mtu,
       ifr[total_interfaces].ifr_hwaddr.sa_data[0],ifr[total_interfaces].ifr_hwaddr.sa_data[1],ifr[total_interfaces].ifr_hwaddr.sa_data[2],
       (unsigned char)ifr[total_interfaces].ifr_hwaddr.sa_data[3],ifr[total_interfaces].ifr_hwaddr.sa_data[4],ifr[total_interfaces].ifr_hwaddr.sa_data[5]);
   } 
 }
 
 else 
 {
 	
   //while(ifr[--total_interfaces].ifr_name != argv[1])  
   while (strcmp(ifr[--total_interfaces].ifr_name, argv[1]))
   {     
      //printf("name=%s, argv[1]=%s\n",ifr[total_interfaces].ifr_name, argv[1]);
      continue;
   }
   
   printf("mtu of %s=%d\t hwdaddrs=%.2X:%.2X:%X:%x:%x:%x\n",ifr[total_interfaces].ifr_name,ifr[total_interfaces].ifr_mtu,
   ifr[total_interfaces].ifr_hwaddr.sa_data[0],ifr[total_interfaces].ifr_hwaddr.sa_data[1],ifr[total_interfaces].ifr_hwaddr.sa_data[2],
   (unsigned char)ifr[total_interfaces].ifr_hwaddr.sa_data[3],ifr[total_interfaces].ifr_hwaddr.sa_data[4],ifr[total_interfaces].ifr_hwaddr.sa_data[5]);
}

}
 
 
 
 
 void Get_all_interfaces_names(struct ifreq *ifr, short int total_interfaces)
 {
 	short int i=0;
 	for (--total_interfaces; total_interfaces >= 0; --total_interfaces)
	{ 
	 short int pos=0;
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


