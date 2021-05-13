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



struct interfaces{
		
		int mtu;
		char name[IFNAMSIZ];
		char hwaddr[MAC_SIZE];
	};

 void Get_all_interfaces_names(struct interfaces *ifc, short int total_interfaces);
 short int Print_info (struct interfaces *ifc, short int total_interfaces, int argc, char *argv[]);
 short int Get_info (struct interfaces *ifc, short int total_interfaces, int argc);





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

	FILE *fp,*fp2;
	 
	short int  total_interfaces ,pos,i=0;
	
	
	fp=popen("ls /sys/class/net","r"); //to get names of the network interfaces present in the system
	
	if (fp==NULL)
	 perror("couldnt get interface names\t");   
	
	fread(path,sizeof(char),100,fp);   

	
	int fd, i=0,count, count1;
	unsigned char interface_count, interface_names[100];
	FILE *fp,*fp2;
	
	

	fp=popen("ls /sys/class/net","r"); //to get names of the network interfaces present in the system
	
	if(fp==NULL) { perror("couldnt get interface names\t"); }  
	 
	fread(interface_names,sizeof(char),100,fp);  
	
	
	fp2= popen("ls /sys/class/net | wc -l","r");  //to get total count of the network interfaces present in the system 
	 
	if(fp2==NULL) { perror("couldnt get total interface count\t"); }
	
	fread(&interface_count,sizeof(char),1,fp2);

	fread(&interfaces,sizeof(char),1,fp2);

	
	 total_interfaces= interfaces - 48; // converting char to int
	 	 
	 struct interfaces ifc[total_interfaces];
	 
	 
	 Get_info (ifc, total_interfaces, argc);
	 Print_info(ifc, total_interfaces, argc, argv);
	 

	pclose (fp);
	pclose (fp2);
	
	return 0;
}


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

 	   count=0;
	   strncpy(ifc[count].name, port_name, IFNAMSIZ);
	   
	   printf("mtu of %s=%d\t hwdaddrs=%02X:%02X:%02X:%02x:%02x:%02x\n",ifc[count].name,ifc[count].mtu,
	   ifc[count].hwaddr[0], ifc[count].hwaddr[1], ifc[count].hwaddr[2],
	   ifc[count].hwaddr[3], ifc[count].hwaddr[4], ifc[count].hwaddr[5]);
  }

 	
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
 

 
 

 void get_all_interfaces_names(struct interfaces *ifc, short int count, unsigned char *interface_names)
 {
 	short int i=0;
 	for (count; count >= 0; count--)

 void Get_all_interfaces_names(struct interfaces *ifc, short int total_interfaces)
 {
 	short int i=0;
 	for (total_interfaces; total_interfaces >= 0; total_interfaces--)

	{ 
	 short int pos=0;
	  while(interface_names[i] != '\n')
	  { 	 

			 ifc[count].name[pos++] = interface_names[i];

			 ifc[total_interfaces].name[pos++] = path[i];

			 i++;
			 if(interface_names[i] == '\n') 
			 {

			 	   ifc[count].name[pos] = '\0';

			 	   ifc[total_interfaces].name[pos] = '\0';

				 	i++;
				 	break; 
			 }
		}
	 }
  }


