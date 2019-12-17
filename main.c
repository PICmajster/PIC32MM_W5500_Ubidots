/*****************************************************************************
  FileName:        main.c
  Processor:       PIC32MM0256GPM064
  Compiler:        XC32 ver 2.30
  IDE :            MPLABX-IDE 5.25
  Created by:      http://strefapic.blogspot.com
 ******************************************************************************/
/*----------------------------------------------------------------------------*/
/* Wiznet W5500 Ethernet Module connect                                       */
/*----------------------------------------------------------------------------*/
// ** Sample application for PIC32MM **
//    
//    W5500      --->    MCU PIC32MM
//     RST                     -->  RC14
//     MOSI                    -->  RA7 (MISO) 
//     MISO                    -->  RA8 (MOSI) 
//     SCK                     -->  RA10 
//     CS                      -->  RC11
//     INT                     -->  RD2  (not used))
/*----------------------------------------------------------------------------*/
/* UART for Printf() function                                                 */
/*----------------------------------------------------------------------------*/ 
//    Required connections for UART:
//    
//    CP210x Module     ---> MCU PIC32MM
//    TX                     --> RA6 (not used))
//    RX                     --> RC12 (TX)
//******************************************************************************
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/spi3.h"
#include "mcc_generated_files/pin_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "delay.h"
#include "wizchip_conf.h"
#include "w5500.h"
#include "socket.h"


#define Wizchip_Reset PORTCbits.RC14

// Socket & Port number definition
#define SOCK_ID_TCP       0
#define SOCK_ID_UDP       1
#define PORT_TCP          5000
#define PORT_UDP          10001
#define DATA_BUF_SIZE     1024

wiz_NetInfo pnetinfo;
uint8_t gDATABUF[DATA_BUF_SIZE]; 
uint8_t tmp;
uint8_t licznik = 1 ;
volatile uint8_t SoftTimer1 , SoftTimer2 ;

/*Call back function for W5500 SPI - Theses used as parametr of reg_wizchip_spi_cbfunc()
 Should be implemented by WIZCHIP users because host is dependent it*/
uint8_t CB_SpiRead(void);
void CB_ChipSelect(void);
void CB_ChipDeselect(void);
void CB_SpiWrite(uint8_t wb);

void TCP_HTTP(void);
void W5500_Init(void);
void network_init(void);


/*definition of network parameters*/
 wiz_NetInfo gWIZNETINFO =
{
  .mac = {0xba, 0x5a, 0x4d, 0xb4, 0xc3, 0xe0},    // Source Mac Address 
  .ip = {192, 168,  8, 220},                     // Source IP Address
  .sn = {255, 255, 255, 0},                      // Subnet Mask
  .gw = {192, 168,  8, 1},                       // Gateway IP Address
  .dns = {192, 168, 8, 1},                     // DNS server IP Address
  .dhcp = NETINFO_STATIC
 
 };


 
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    printf("\r\n***** W5500_TEST_START *****\r\n");
    W5500_Init();
    printf("\r\n***** W5500 successfully connect to Router *****\r\n"); // must be connect cable W5500 to Router
    
    TMR1_Start();
    
    while (1)
    {
       TCP_HTTP();                       
    }
}


/*Call back function for W5500 SPI*/

void CB_ChipSelect(void) // Call back function for WIZCHIP select.
{
    CS_SetLow();
}

void CB_ChipDeselect(void) // Call back function for WIZCHIP deselect.
{
    CS_SetHigh();
}

uint8_t CB_SpiRead(void) // Callback function to read byte usig SPI.
{
    return SPI3_Exchange8bit(0xFF);
}

void CB_SpiWrite(uint8_t wb) // Callback function to write byte usig SPI.
{
    SPI3_Exchange8bit(wb);
    
}


void TCP_HTTP(void)
{
    
uint8_t POST_Ubidots[256] ; //buffor for HTTP POST Request
uint8_t IP_ubidots[] = {169,55,61,243}; //IP adress Ubidots
int32_t ret;
uint16_t size = 0, sentsize = 0;

memset(POST_Ubidots, 0, sizeof(POST_Ubidots)); //clear buffor POST_Ubidots

/*TCP GET Request*/
uint8_t GET_Ubidots[] = {"Dom/1.0|LV|BBFF-dlkCOCzYMfLY9AV5wgxgsDrP2brXYZ|pic32:temperaturasalon|end"};
 /*TCP POST Request*/
sprintf(POST_Ubidots,"Dom/1.0|POST|BBFF-dlkCOCzYMfLY9AV5wgxgsDrP2brXYZ|pic32=>temperaturasalon:%d|end",licznik);
   
    // Get status of socket
    switch(getSn_SR(SOCK_ID_TCP))
    {
        // Connection established
        case SOCK_ESTABLISHED :
        {
            
            // Check interrupt: connection with peer is successful
            if(getSn_IR(SOCK_ID_TCP) & Sn_IR_CON)
            {
                // Clear corresponding bit
                setSn_IR(SOCK_ID_TCP,Sn_IR_CON);
            }

            // Get received data size
            if((size = getSn_RX_RSR(SOCK_ID_TCP)) > 0)
            {
                printf("\r\n-------TCP Packet Received-------\r\n");
                // Cut data to size of data buffer
                if(size > DATA_BUF_SIZE)
                {
                    size = DATA_BUF_SIZE;
                }

                // Get received data
                ret = recv(SOCK_ID_TCP, gDATABUF, size);

                // Check for error
                if(ret <= 0)
                {
                    return;
                }
                printf("\r\n");
                UART1_WriteBuffer(gDATABUF, size); //show the data of the buffer gDATABUF
                printf("\r\n");
                memset(gDATABUF, 0, sizeof(gDATABUF)); //clear buffer gDATABUF
                                             
              if(ret < 0)
                    {
                        close(SOCK_ID_TCP);
                        return;
                    }
                   
            }
             
            if(!SoftTimer1) {
              SoftTimer1 = 50 ; /*TMR1 x SoftTimer1 = 100ms x 50 = 5000 ms*/
              /*POST Request*/
              printf("\r\n");
              UART1_WriteBuffer(POST_Ubidots, strlen(POST_Ubidots)); //show on UART the send buffer POST_Ubidots
              ret = send(SOCK_ID_TCP, POST_Ubidots, strlen(POST_Ubidots)); // send data to socket TCP
              licznik++ ;
              if(licznik > 10) licznik = 1;
              
              /*GET Request*/
              printf("\r\n");
              UART1_WriteBuffer(GET_Ubidots, strlen(GET_Ubidots)); //show on UART the send buffer GET_Ubidots
              ret = send(SOCK_ID_TCP, GET_Ubidots, strlen(GET_Ubidots)); // send data to socket TCP
              }
      
            break;
        }

        // Socket received the disconnect-request (FIN packet) from the connected peer
        case SOCK_CLOSE_WAIT :
        {
            // Disconnect socket
            if((ret = disconnect(SOCK_ID_TCP)) != SOCK_OK)
            {
                return;
            }

            break;
        }

        // Socket is opened with TCP mode
        case SOCK_INIT :
        {
            
          // if( (ret = connect(SOCK_ID_TCP,IP_ubidots, 80)) == SOCK_OK) //for HTTP
             if( (ret = connect(SOCK_ID_TCP,IP_ubidots, 9012)) == SOCK_OK) //for TCP/UDP
            {
                   return;
            }
              
            break;
        }

        // Socket is released
        case SOCK_CLOSED:
        {
            // Open TCP socket
            if((ret = socket(SOCK_ID_TCP, Sn_MR_TCP, PORT_TCP, 0x00)) != SOCK_ID_TCP)
            {
                printf("\r\n---------socket is not opened---------\r\n");
                return;
            }
           printf("\r\n------socket is no. %d opened success------\r\n", ret);
           break;
        }

        default:
        {
            break;
        }
    }
}


/* Initialize modules */
 void W5500_Init(void)
{
    // Set Tx and Rx buffer size to 2KB
    uint8_t txsize[8] = {2,2,2,2,2,2,2,2};
    uint8_t rxsize[8] = {2,2,2,2,2,2,2,2};
       
    CB_ChipDeselect();  // Deselect module

    // Reset module
    Wizchip_Reset = 0;
    delayMs(1);
    Wizchip_Reset = 1;
    delayMs(1);

     /* Registration call back */
     //_WIZCHIP_IO_MODE_ = _WIZCHIP_IO_MODE_SPI_VDM_
    reg_wizchip_cs_cbfunc(CB_ChipSelect, CB_ChipDeselect);

    /* SPI Read & Write callback function */
    reg_wizchip_spi_cbfunc(CB_SpiRead, CB_SpiWrite);
     
    /* WIZCHIP SOCKET Buffer initialize */
       wizchip_init(txsize,rxsize);
    
    /* PHY link status check */
    do
    {
       if(!(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp)))
          printf("\r\nUnknown PHY Link status.\r\n");
       delayMs(500);
    }  while(tmp == PHY_LINK_OFF); 
    printf("\r\nPHY Link status OK.\r\n");
    
    /*Intialize the network information to be used in WIZCHIP*/
    network_init();
}

void network_init(void)
{
  uint8_t tmpstr[6];
  ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO); // Write Network Settings to W5500 registers
        
  // Display Network Information 
  ctlnetwork(CN_GET_NETINFO, (void*)&pnetinfo);    // Read registers value , W5500 Network Settings    
  ctlwizchip(CW_GET_ID,(void*)tmpstr); //GET ID WIZNET like this : W5500
  /*Send the data to the Uart*/
  printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
  printf("\r\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", pnetinfo.mac[0],pnetinfo.mac[1],pnetinfo.mac[2],
		   pnetinfo.mac[3],pnetinfo.mac[4],pnetinfo.mac[5]);
  printf("SIP: %d.%d.%d.%d\r\n", pnetinfo.ip[0],pnetinfo.ip[1],pnetinfo.ip[2],pnetinfo.ip[3]);
  printf("GAR: %d.%d.%d.%d\r\n", pnetinfo.gw[0],pnetinfo.gw[1],pnetinfo.gw[2],pnetinfo.gw[3]);
  printf("SUB: %d.%d.%d.%d\r\n", pnetinfo.sn[0],pnetinfo.sn[1],pnetinfo.sn[2],pnetinfo.sn[3]);
  printf("DNS: %d.%d.%d.%d\r\n", pnetinfo.dns[0],pnetinfo.dns[1],pnetinfo.dns[2],pnetinfo.dns[3]);
  printf("======================\r\n");

}


 
