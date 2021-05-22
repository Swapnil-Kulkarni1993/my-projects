#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>

#define MAC_SIZE 6
#define MAX_INTERFACES 5



struct interfaces{
		
unsigned char name[IFNAMSIZ];
unsigned char hwaddr[MAC_SIZE];
int mtu;

};

static void get_all_interfaces_names(struct interfaces *ifc, short int count, unsigned char *interface_names);
static int get_interfaces_info(struct interfaces *ifc, struct ifreq *ifr, unsigned char *port_name, int count, int fd);
static void print_info(struct interfaces *ifc, unsigned char *port_name, int count);
static int interface_info(unsigned char *port_name);


int main(int argc, char *argv[])
{
 	unsigned char port_name[IFNAMSIZ];
 	int retval;
 
 	if(argc > 1) {  strncpy(port_name, argv[1], IFNAMSIZ); }
 	else { strncpy(port_name, "no_port", IFNAMSIZ); }

 	retval = interface_info(port_name);
 	
 	if(retval) { printf("EXITING main() function....!!!!!!\n"); exit(EXIT_FAILURE); }
 	
 	return 0;
}

/**************************************************************************************************************/

 static int interface_info(unsigned char *port_name)
{
	
	struct interfaces ifc[MAX_INTERFACES]; struct ifreq ifr[MAX_INTERFACES];
 	int fd, count, goterr=0, retval=0;
	unsigned char interface_count, interface_names[100];
	FILE *fp, *fp2;
	
	fp=popen("ls /sys/class/net", "r"); //to get names of the network interfaces present in the system
	if(fp == NULL) { perror("couldnt get interface names\t"); goterr=1; goto _close1; }  
	 
	fread(interface_names, sizeof(char), 100, fp);  
	if(ferror(fp)) { printf("Error in reading interface names..!!!\n"); goterr=1; goto _close1; }

	fp2 = popen("ls /sys/class/net | wc -l", "r");  //to get total count of the network interfaces present in the system 
	if(fp2 == NULL) { perror("couldnt get total interface count\t"); goterr=1; goto _close2; }
	
	fread(&interface_count, sizeof(char), 1, fp2);
	if(ferror(fp2)) { printf("Error in getting total interface count..!!!\n"); goterr=1; goto _close2; }

   count = interface_count - 48; // converting char to int
   count = count-1; //as array index starts from 0

	fd = socket(AF_INET, SOCK_DGRAM, 0); if(fd < 0) { perror("EXITING the program..!!couldnt open socket\t"); goterr=1; goto _close3; }
   
   get_all_interfaces_names(ifc, count, interface_names);
   
   retval = get_interfaces_info(ifc, ifr, port_name, count, fd);
   
   if(retval) { printf("Error in getting interface info...!!!\n"); goterr=1; goto _close3; }
   
   print_info(ifc, port_name, count);
   
_close3:      
   close(fd);
_close2:   
   pclose(fp2);
_close1:   
   pclose(fp);
      
   if(goterr) { return 1; } 
   else { return 0; } 

}

/**************************************************************************************************************/
 
 static int get_interfaces_info(struct interfaces *ifc, struct ifreq *ifr, unsigned char *port_name, int count, int fd)
 {
 	 int result;
 	 
    result = strcmp(port_name, "no_port");
	 if(result != 0) { count = 0; strncpy(ifc[count].name, port_name, IFNAMSIZ); }
	
	 for(count; count >= 0; count--)
	 {
	    strncpy(ifr[count].ifr_name, ifc[count].name, IFNAMSIZ);
	   	 
	    if(ioctl(fd, SIOCGIFHWADDR, &ifr[count])<0) { perror("EXITING the program..!!couldnt get hardware address\t"); return 1; }
		 memcpy(ifc[count].hwaddr, ifr[count].ifr_hwaddr.sa_data, MAC_SIZE);   		
	   		
	    if (ioctl(fd, SIOCGIFMTU, &ifr[count])<0) { perror("EXITING the program..!!couldnt get MTU\t"); return 1; }  
	    ifc[count].mtu=ifr[count].ifr_mtu;   
	 }
    
    return 0;
 }

/**************************************************************************************************************/

static void print_info(struct interfaces *ifc, unsigned char *port_name, int count)
{
   int result;	
   
	result = strcmp(port_name, "no_port");
	if(result != 0) { count = 0; strncpy(ifc[count].name, port_name, IFNAMSIZ); }

   printf("\tNAME\t\tMTU\t\tHWADDR\n\n");   
  
   for(count; count >= 0; count--)
   {   
      printf("\t%s\t\t%d\t\t%02X:%02X:%02X:%02x:%02x:%02x\n",ifc[count].name, ifc[count].mtu,
             ifc[count].hwaddr[0], ifc[count].hwaddr[1], ifc[count].hwaddr[2],
             ifc[count].hwaddr[3], ifc[count].hwaddr[4], ifc[count].hwaddr[5]);
   } 

}
 
/**************************************************************************************************************/

 static void get_all_interfaces_names(struct interfaces *ifc, short int count, unsigned char *interface_names)
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


