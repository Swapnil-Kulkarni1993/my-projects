#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>

#define MAC_SIZE 6

struct interfaces{
		
		unsigned char name[IFNAMSIZ];
		unsigned char hwaddr[MAC_SIZE];
		int mtu;
	};

void get_all_interfaces_names(struct interfaces *ifc, short int count, unsigned char *interface_names);
short int print_info (struct interfaces *ifc, unsigned char *port_name, int count);
short int get_info (unsigned char *port_name);





int main(int argc, char *argv[])
{
	
	 unsigned char port_name [IFNAMSIZ];
	 
	 
	 if(argc > 1) {  strncpy(port_name, argv[1], IFNAMSIZ); }
	 else { strncpy(port_name, "no_port", IFNAMSIZ); }

	 get_info(port_name );
	 //print_info(port_name);

	return 0;
}


short int get_info (unsigned char *port_name)
{
	
	int fd, i=0,count, count1;
	unsigned char interface_count, interface_names[100];
	FILE *fp,*fp2;
	
	
	fp=popen("ls /sys/class/net","r"); //to get names of the network interfaces present in the system
	
	if(fp==NULL) { perror("couldnt get interface names\t"); }  
	 
	fread(interface_names,sizeof(char),100,fp);  
	
	
	fp2= popen("ls /sys/class/net | wc -l","r");  //to get total count of the network interfaces present in the system 
	 
	if(fp2==NULL) { perror("couldnt get total interface count\t"); }
	
	fread(&interface_count,sizeof(char),1,fp2);

   count1 = count = interface_count - 48; // converting char to int
	struct interfaces ifc[count]; struct ifreq ifr[count];
	 
	fd = socket(AF_INET, SOCK_DGRAM, 0);  if (fd < 0) { perror("EXITING the program...!!!couldnt open socket\t"); return 1; }
    
   count=count-1; //as array index starts from 0
   get_all_interfaces_names(ifc, count, interface_names);
    

   if(!(strcmp(port_name, "no_port")))
   {
	   for (count; count >= 0; count--)
	   {
	   	 strncpy(ifr[count].ifr_name,ifc[count].name, IFNAMSIZ);
	   	 
	       if (ioctl(fd, SIOCGIFHWADDR, &ifr[count])<0) { perror("EXITING the program...!!!couldnt get hardware address\t"); return 1; }
			 memcpy(ifc[count].hwaddr, ifr[count].ifr_hwaddr.sa_data, MAC_SIZE);   		
	   		
	       if (ioctl(fd, SIOCGIFMTU, &ifr[count])<0) { perror("EXITING the program...!!!couldnt get MTU\t"); return 1; }  
	       ifc[count].mtu=ifr[count].ifr_mtu;
	       
	   }
   } 
   
   else 
   {
   	count = 0;
   	strncpy(ifr[count].ifr_name, port_name, IFNAMSIZ);
   
   	 if (ioctl(fd, SIOCGIFHWADDR, &ifr[count])<0) { perror("EXITING the program...!!!couldnt get hardware address\t"); return 1; }
		 memcpy(ifc[count].hwaddr, ifr[count].ifr_hwaddr.sa_data, MAC_SIZE);   		
	   		
	    if (ioctl(fd, SIOCGIFMTU, &ifr[count])<0) { perror("EXITING the program...!!!couldnt get MTU\t"); return 1; }  
	    ifc[count].mtu=ifr[count].ifr_mtu;
   
   }
   
    print_info(ifc, port_name, interface_count-48);
      
   pclose(fp);
	pclose(fp2);   
   close(fd);

}




short int print_info (struct interfaces *ifc, unsigned char *port_name, int count)
{
	
 count=count-1;
 if(!(strcmp(port_name, "no_port")))
 {	
   for (count; count >= 0; count--)
   {
        
    printf("mtu of %s=%d\t hwdaddrs=%02X:%02X:%02X:%02x:%02x:%02x\n",ifc[count].name,ifc[count].mtu,
    ifc[count].hwaddr[0], ifc[count].hwaddr[1], ifc[count].hwaddr[2],
    ifc[count].hwaddr[3], ifc[count].hwaddr[4], ifc[count].hwaddr[5]);
   } 
 }
 
 else 
 {
 	   count=0;
	   strncpy(ifc[count].name, port_name, IFNAMSIZ);
	   
	   printf("mtu of %s=%d\t hwdaddrs=%02X:%02X:%02X:%02x:%02x:%02x\n",ifc[count].name,ifc[count].mtu,
	   ifc[count].hwaddr[0], ifc[count].hwaddr[1], ifc[count].hwaddr[2],
	   ifc[count].hwaddr[3], ifc[count].hwaddr[4], ifc[count].hwaddr[5]);
  }

}
 

 
 
 void get_all_interfaces_names(struct interfaces *ifc, short int count, unsigned char *interface_names)
 {
 	short int i=0;
 	for (count; count >= 0; count--)
	{ 
	 short int pos=0;
	  while(interface_names[i] != '\n')
	  { 	 
			 ifc[count].name[pos++] = interface_names[i];
			 i++;
			 if(interface_names[i] == '\n') 
			 {
			 	   ifc[count].name[pos] = '\0';
				 	i++;
				 	break; 
			 }
		}
	 }
  }


