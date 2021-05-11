#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>

#define MAC_SIZE 6

struct interfaces{
		
		int mtu;
		char name[IFNAMSIZ];
		char hwaddr[MAC_SIZE];
	};

 void Get_all_interfaces_names(struct interfaces *ifc, short int total_interfaces);
 short int Print_info (struct interfaces *ifc, short int total_interfaces, int argc, char *argv[]);
 short int Get_info (struct interfaces *ifc, short int total_interfaces, int argc);

char path[100], interfaces;




int main(int argc, char *argv[])
{
	FILE *fp,*fp2;
	 
	short int  total_interfaces ,pos,i=0;
	
	
	fp=popen("ls /sys/class/net","r"); //to get names of the network interfaces present in the system
	
	if (fp==NULL)
	 perror("couldnt get interface names\t");   
	
	fread(path,sizeof(char),100,fp);   
	
	
	fp2= popen("ls /sys/class/net | wc -l","r");  //to get total count of the network interfaces present in the system 
	 
	if (fp2==NULL)
	 perror("couldnt get total interface count\t");
	
	fread(&interfaces,sizeof(char),1,fp2);

	
	 total_interfaces= interfaces - 48; // converting char to int
	 	 
	 struct interfaces ifc[total_interfaces];
	 
	 
	 Get_info (ifc, total_interfaces, argc);
	 Print_info(ifc, total_interfaces, argc, argv);
	 

	pclose (fp);
	pclose (fp2);
	
	return 0;
}


short int Get_info (struct interfaces *ifc, short int total_interfaces, int argc)
{
	struct ifreq ifr [total_interfaces];
	int fd, i=0;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
 
   if (fd < 0)
   {
     perror("EXITING the program...!!!couldnt open socket\t");
     return 1;  
   }
    
   total_interfaces=total_interfaces-1; //as array index starts from 0
   Get_all_interfaces_names(ifc, total_interfaces);
   
   for (total_interfaces; total_interfaces >= 0; total_interfaces--)
   {
   	 strncpy(ifr[total_interfaces].ifr_name,ifc[total_interfaces].name,sizeof(ifc[total_interfaces].name));
   	 
       if (ioctl(fd, SIOCGIFHWADDR, &ifr[total_interfaces])<0)
       {
          perror("EXITING the program...!!!couldnt get hardware address\t");
          printf("--->%s\n", ifr[total_interfaces].ifr_name);
          return 1;
       }
 
       if (ioctl(fd, SIOCGIFMTU, &ifr[total_interfaces])<0)
       {          
          perror("EXITING the program...!!!couldnt get MTU\t");
          return 1;
       }  
     
       ifc[total_interfaces].mtu=ifr[total_interfaces].ifr_mtu;
       
       for(i=0; i<MAC_SIZE; i++)
       ifc[total_interfaces].hwaddr[i]=ifr[total_interfaces].ifr_hwaddr.sa_data[i];
       
   }
      
   close(fd);

}




short int Print_info (struct interfaces *ifc, short int total_interfaces, int argc, char *argv[])
{
	
 total_interfaces=total_interfaces-1;
 if (argc<=1)
 {	
   for (total_interfaces; total_interfaces >= 0; total_interfaces--)
   {
        
    printf("mtu of %s=%d\t hwdaddrs=%02X:%02X:%02X:%02x:%02x:%02x\n",ifc[total_interfaces].name,ifc[total_interfaces].mtu,
    (unsigned char)ifc[total_interfaces].hwaddr[0], (unsigned char)ifc[total_interfaces].hwaddr[1], (unsigned char)ifc[total_interfaces].hwaddr[2],
    (unsigned char)ifc[total_interfaces].hwaddr[3], ifc[total_interfaces].hwaddr[4], (unsigned char)ifc[total_interfaces].hwaddr[5]);
   } 
 }
 
 else 
 {
 	
   //while(ifr[--total_interfaces].ifr_name != argv[1])  
   while (strcmp(ifc[total_interfaces].name, argv[1]))
   {     
      total_interfaces--;
      //printf("name=%s, argv[1]=%s\n",ifr[total_interfaces].ifr_name, argv[1]);
      continue;
   }
   
   printf("mtu of %s=%d\t hwdaddrs=%02X:%02X:%02X:%02x:%02x:%02x\n",ifc[total_interfaces].name,ifc[total_interfaces].mtu,
    (unsigned char)ifc[total_interfaces].hwaddr[0], (unsigned char)ifc[total_interfaces].hwaddr[1], (unsigned char)ifc[total_interfaces].hwaddr[2],
    (unsigned char)ifc[total_interfaces].hwaddr[3], (unsigned char)ifc[total_interfaces].hwaddr[4], (unsigned char)ifc[total_interfaces].hwaddr[5]);
   }

}
 

 
 
 void Get_all_interfaces_names(struct interfaces *ifc, short int total_interfaces)
 {
 	short int i=0;
 	for (total_interfaces; total_interfaces >= 0; total_interfaces--)
	{ 
	 short int pos=0;
	  while(path[i] != '\n')
	  { 	 
			 ifc[total_interfaces].name[pos++] = path[i];
			 i++;
			 if(path[i] == '\n') 
			 {
			 	   ifc[total_interfaces].name[pos] = '\0';
				 	i++;
				 	break; 
			 }
		}
	 }
  }


