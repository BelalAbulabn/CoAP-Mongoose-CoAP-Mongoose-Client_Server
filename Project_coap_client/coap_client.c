/*
 * coap_client.c
 *      Author: Bashar
 *              Belal Abulabn
 *              
 */
#include "coap_client.h"

//*****************************************************************************
// Required by lwIP library to support any host-related timer functions.
// This function is called in "lwIPServiceTimers()" from the "lwiplib.c" utility
// "lwIPServiceTimers()" is called in "lwIPEthernetIntHandler()" from the "lwiplib.c" utility
// "lwIPEthernetIntHandler()" is registered in the interrupt vector table (in file "..._startup_ccs.c")
//*****************************************************************************
#define IP_LINK_DOWN (0xffffffffU)              // No Link
#define IP_LINK_UP (0)                          // DHCP Process
void lwIPHostTimerHandler(void){
    static uint32_t ui32OldIPAddress = IP_LINK_DOWN;
    // Get current IP address
    g_ui32IPAddress = lwIPLocalIPAddrGet();
    // Check if IP address has changed
    if (g_ui32IPAddress != ui32OldIPAddress) {
        switch (g_ui32IPAddress) {
            // Indicate there is no link
            case IP_LINK_DOWN: {
                UARTprintf("Link down.\r\n");
                LEDWrite(CLP_D2, 0);
                g_IPAddressSet = false;
                break;
            }

            // No IP address DHCP is running
            case IP_LINK_UP: {
                UARTprintf("Link up.\r\n");
                LEDWrite(CLP_D2, 0);
                g_IPAddressSet = false;
                break;
            }

            default: {
                UARTprintf("IP address: %s\r\n", ipaddr_ntoa((const ip_addr_t *) &g_ui32IPAddress));
                LEDWrite(CLP_D2, CLP_D2);
                g_IPAddressSet = true;
                break;
            }
        }
        ui32OldIPAddress = g_ui32IPAddress;
        io_display(g_ui32IPAddress);
    }
}

//*****************************************************************************
// The interrupt handler for the SysTick interrupt.
//*****************************************************************************
void SysTickIntHandler(void){

    lwIPTimer(SYSTICKMS);   // Call the lwIP timer handler.
}


//*****************************************************************************
// Mongoose coap handler
//*****************************************************************************
static void coap_handler(struct mg_connection *nc, int ev, void *p){

struct mg_coap_message  *cm = (struct mg_coap_message *) p;      // CoAP message
    switch (ev){
        // Event connect to server
        case MG_EV_CONNECT:
            UARTprintf("---------------------------------------\n");
            UARTprintf("MG_EV_CONNECT invoked \n");
            UARTprintf("---------------------------------------\n");
            //********************************************
            /*
            struct mg_coap_message cm;
              uint32_t res;

              memset(&cm, 0, sizeof(cm));
              cm.msg_id = 1;
              cm.msg_type = MG_COAP_MSG_CON;
              printf("Sending CON...\n");
              res = mg_coap_send_message(nc, &cm);
              if (res == 0) {
                printf("Sent CON with msg_id = %d\n", cm.msg_id);
              } else {
                printf("Error: %d\n", res);
                s_time_to_exit = 1;
              }
              */
              //*******************************************
            break;
        case MG_EV_SEND:

            UARTprintf("---------------------------------------\n");
            UARTprintf("MG_EV_SEND invoked \n");
            UARTprintf("---------------------------------------\n");
            break;
        case MG_EV_RECV:
            UARTprintf("---------------------------------------\n");
            UARTprintf("MG_EV_RECV invoked \n");
            UARTprintf("---------------------------------------\n");
            break;

        case MG_EV_COAP_ACK:{
            char payload[cm->payload.len+1];
            memcpy(payload,cm->payload.p,cm->payload.len);
            payload[cm->payload.len] = '\0';
            UARTprintf("---------------------------------------\n");
            UARTprintf("ACK message ID    : %d\n",cm->msg_id);
            UARTprintf("    payload length: %d \n",cm->payload.len);
            UARTprintf("    payload       : %s\n",payload);
            UARTprintf("---------------------------------------\n");
            if (cm->msg_id >=1000){
                struct mg_coap_message cm;
                uint32_t res;
                if(strcmp(payload,"on") == 0){      //button is pushed
                    memset(&cm, 0, sizeof(cm));
                    cm.msg_id = rand()%1000;
                    cm.msg_type = MG_COAP_MSG_CON;
                    cm.code_class = MG_COAP_CODECLASS_REQUEST;
                    cm.code_detail = MG_COAP_CODEDETAIL_REQUEST_PUT;
                    cm.payload.p = "on";
                    cm.payload.len = strlen("on");
                    mg_coap_add_option(&cm, URI_PATH, "light", (strlen("light")));
                    printf("Sending...\n");
                    res = mg_coap_send_message(nc, &cm);
                    if (res == 0) {
                        printf("Sent CON PUT with msg_id = %d\n", cm.msg_id);
                    } else {
                        printf("Error: %d\n", res);
                        s_time_to_exit = 1;
                    }
                    while (!s_time_to_exit)
                    {
                        mg_mgr_poll(&mgr, 10);
                    }
                    s_time_to_exit=0;
                }else if (strcmp(payload,"off") == 0){                             ////button is not pushed
                    memset(&cm, 0, sizeof(cm));
                    cm.msg_id = rand()%1000;
                    cm.msg_type = MG_COAP_MSG_CON;
                    cm.code_class = MG_COAP_CODECLASS_REQUEST;
                    cm.code_detail = MG_COAP_CODEDETAIL_REQUEST_PUT;
                    cm.payload.p = "off";
                    cm.payload.len = strlen("off");
                    mg_coap_add_option(&cm, URI_PATH, "light", (strlen("light")));
                    printf("Sending...\n");
                    res = mg_coap_send_message(nc, &cm);
                    if (res == 0) {
                        printf("Sent CON PUT with msg_id = %d\n", cm.msg_id);
                    } else {
                        printf("Error: %d\n", res);
                        s_time_to_exit = 1;
                    }
                    while (!s_time_to_exit)
                    {
                        mg_mgr_poll(&mgr, 10);
                    }
                    s_time_to_exit=0;
                }

            }
            s_time_to_exit = 1;
            break;
        }
    }
}

// Necessary Mongoose functions
int gettimeofday(struct timeval *tv, void *tzvp){
    tv->tv_sec = time(NULL);
    tv->tv_usec = 0;
    return 0;
}

void mg_lwip_mgr_schedule_poll(struct mg_mgr *mgrS){
}

//*****************************************************************************
// Main function
// Coap client
//*****************************************************************************
int main(void) {
    uint32_t u0, u1;
    uint8_t mac[6];
    struct mg_connection    *nc;
    //SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                           SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                           120000000);
    // Configure the device pins.
    PinoutSet(true, false);

    // Configure uart debug port
    UARTStdioConfig(0, 115200, ui32SysClock);

    // Clear the terminal and print a banner.
    UARTprintf("\033[2J\033[H");
    UARTprintf("Simple CoAP Client - Mongoose\n\n");
    CFAF128128B0145T_init(2, ui32SysClock, 20000000);
    CFAF128128B0145T_clear(CFAF128128B0145T_color_black);

    // Convert the 24/24 split MAC address from NV ram into a 32/16 split
    // MAC address needed to program the hardware registers, then program
    // the MAC address into the Ethernet Controller registers.
    mac[0] = ((u0 >> 0) & 0xff);
    mac[1] = ((u0 >> 8) & 0xff);
    mac[2] = ((u0 >> 16) & 0xff);
    mac[3] = ((u1 >> 0) & 0xff);
    mac[4] = ((u1 >> 8) & 0xff);
    mac[5] = ((u1 >> 16) & 0xff);

    // Initialize the lwIP library, using DHCP.
    lwIPInit(ui32SysClock, mac, 0, 0, 0, IPADDR_USE_DHCP);

    // Configure SysTick for a periodic interrupt.
    MAP_SysTickPeriodSet(ui32SysClock / SYSTICKHZ);
    MAP_SysTickEnable();
    MAP_SysTickIntEnable();

    UARTprintf("Waiting for IP...\n");

    while(!g_IPAddressSet)
        {
            // Wait for DHCP process to finish
        }

    mg_mgr_init(&mgr, 0);
    nc = mg_connect(&mgr, s_default_address, coap_handler);
    mg_set_protocol_coap(nc);
    UARTprintf("check!\n");
    mg_mgr_poll(&mgr, 2);


    //****************
    //Programm start
    //****************
    printf("Welcome to our CoAP Client:\n");
    int run = -1;
    while(run != 5){
        printf("\n\n\n\n\n\n\n");
        printf(" _______________________________________________________\n");
        printf("|0.discovery\t1.get request\t2.put request\t5.end  |\n");
        printf("|______________________________________________________|\n");
        fflush(stdin);
        scanf("%d",&run);

        switch(run){

            case 0 :
                discovery_req(nc);

                break;
            case 1 :
                get_req(nc);

                break;
            case 2:
                put_req(nc);

                break;

            case 5:
                printf("Bye Bye:\n");
                run = 5;
                break;
       }

    }
}
//*****************************************
//Discovery Funtion
//*****************************************
void discovery_req(struct mg_connection *nc){
    struct mg_coap_message cm;
    uint32_t res;

    printf("composing DISCOVERY Messegse...\n");
    memset(&cm, 0, sizeof(cm));
    cm.msg_id = rand()%1000;
    cm.msg_type = MG_COAP_MSG_CON ;
    cm.code_class = MG_COAP_CODECLASS_REQUEST;
    cm.code_detail = MG_COAP_CODEDETAIL_REQUEST_GET;
    mg_coap_add_option(&cm, URI_PATH, ".well-known/core", (strlen(".well-known/core")));
    printf("Sending...\n");
    res = mg_coap_send_message(nc, &cm);
    if (res == 0) {
        printf("Sent CON DISCOVERY with msg_id = %d\n", cm.msg_id);
    } else {
        printf("Error: %d\n", res);
        s_time_to_exit = 1;
    }
    while (!s_time_to_exit)
    {
        mg_mgr_poll(&mgr, 10);
    }
    s_time_to_exit=0;
}

//*****************************************
//Get Funtion
//*****************************************
void get_req(struct mg_connection *nc){
    struct mg_coap_message cm;
    uint32_t res;
    char i_resource[20];
    char i_payload[20];

    printf("composing GET Messegse...\n");
    printf("please type your desiered endpoint:\n");
    fflush(stdin);
    scanf("%s",i_resource);
    memset(&cm, 0, sizeof(cm));
    if(strcmp(i_resource,"button") == 0){       //hier markieren wir die get anfragen die zu der ressource "button" gehen mir ein id bereich [1000-2000]
        cm.msg_id = 1000+rand()%1000;           //so k�nnen wir im handler darauf spiziell reagieren um mit der  button zustand die light resource zu steuern!
    }else{                                      //eine Alternative dazu w�re mit tokens zu markieren! diese Varieante gefiel mir besser !
        cm.msg_id = rand()%1000;
    }
    cm.msg_type = MG_COAP_MSG_CON;
    cm.code_class = MG_COAP_CODECLASS_REQUEST;
    cm.code_detail = MG_COAP_CODEDETAIL_REQUEST_GET;
    mg_coap_add_option(&cm, URI_PATH, i_resource, (strlen(i_resource)));
    printf("Sending...\n");
    res = mg_coap_send_message(nc, &cm);
    if (res == 0) {
        printf("Sent CON GET with msg_id = %d\n", cm.msg_id);
    } else {
        printf("Error: %d\n", res);
        s_time_to_exit = 1;
    }
    while (!s_time_to_exit)
    {
        mg_mgr_poll(&mgr, 10);
    }
    s_time_to_exit=0;
}

//*****************************************
//Put Funtion
//*****************************************
void put_req(struct mg_connection *nc){
    struct mg_coap_message cm;
    uint32_t res;
    char i_resource[20];
    char i_payload[20];
    printf("composing PUT Messegse...\n");
    printf("please type your desiered endpoint:\n");
    fflush(stdin);
    scanf("%s",i_resource);
    printf("please type your payload\n");
    fflush(stdin);
    scanf("%s",i_payload);

    memset(&cm, 0, sizeof(cm));
    cm.msg_id = rand()%1000;
    cm.msg_type = MG_COAP_MSG_CON;
    cm.code_class = MG_COAP_CODECLASS_REQUEST;
    cm.code_detail = MG_COAP_CODEDETAIL_REQUEST_PUT;
    cm.payload.p = i_payload;
    cm.payload.len = strlen(i_payload);
    mg_coap_add_option(&cm, URI_PATH, i_resource, (strlen(i_resource)));
    printf("Sending...\n");
    res = mg_coap_send_message(nc, &cm);
    if (res == 0) {
        printf("Sent CON PUT with msg_id = %d\n", cm.msg_id);
    } else {
        printf("Error: %d\n", res);
        s_time_to_exit = 1;
    }
    while (!s_time_to_exit)
    {
        mg_mgr_poll(&mgr, 10);
    }
    s_time_to_exit=0;
}

//*****************************************
//Herr B�ck's sch�ne io_display() funktion
//*****************************************
void io_display(uint32_t localIP)
{
int     l;
char    localStr[200];


    // Titel                          123456789012345678901
    CFAF128128B0145T_text(0, 10, "      CoAP Client    ", CFAF128128B0145T_color_white, CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(0, 20, "_____________________", CFAF128128B0145T_color_white, CFAF128128B0145T_color_black, 1, 1);


    // IP info
    sprintf(&localStr[0]," %d.%d.%d.%d", localIP & 0xff, (localIP >> 8) & 0xff, (localIP >> 16) & 0xff, (localIP >> 24) & 0xff);
    l = strlen(&localStr[0]);
    memset(&localStr[l], ' ', 21-l);    // fill with SPACEs
    localStr[21] = '\0';                // terminate string

    switch(localIP){
        case 0xFFFFFFFF:
            //                            123456789012345678901
            CFAF128128B0145T_text(0, 30, "Waiting for LINK ... ", CFAF128128B0145T_color_white, CFAF128128B0145T_color_black, 1, 1);
            break;

        case 0:
            //                            123456789012345678901
            CFAF128128B0145T_text(0, 30, "Waiting for IP ...   ", CFAF128128B0145T_color_white, CFAF128128B0145T_color_black, 1, 1);
            break;

        default:
            //                            123456789012345678901
            CFAF128128B0145T_text(0, 30, "IP-Address:        ", CFAF128128B0145T_color_white, CFAF128128B0145T_color_black, 1, 1);
            CFAF128128B0145T_text(0, 40, localStr, CFAF128128B0145T_color_white, CFAF128128B0145T_color_black, 1, 1);
            break;
    }
    //sprintf(s_default_address,"%s",s_default_address[6]);
    CFAF128128B0145T_text(0, 50, "Server: ", CFAF128128B0145T_color_white, CFAF128128B0145T_color_black, 1, 1);
    CFAF128128B0145T_text(0, 60, s_default_address+6, CFAF128128B0145T_color_white, CFAF128128B0145T_color_black, 1, 1);


    return;
}
