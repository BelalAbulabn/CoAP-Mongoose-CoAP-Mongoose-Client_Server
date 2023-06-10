/*
 * coap_client.c
 *      Author: Bashar
 *              Belal Abulabn
 *              
 */
#include <CFAF128128B0145T/CFAF128128B0145T.h>      // Display
// Mongoose   library
#include "third_party/mongoose.h"

#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

// utils libraries
#include "utils/lwiplib.h"


//drivers library
#include "drivers/pinout.h"

// CoAP Message Code according to RFC 7252@ https://tools.ietf.org/html/rfc7252#section-12.1.1
#define MG_COAP_CODEDETAIL_REQUEST_GET 1
#define MG_COAP_CODEDETAIL_REQUEST_POST 2
#define MG_COAP_CODEDETAIL_REQUEST_PUT 3
#define MG_COAP_CODEDETAIL_REQUEST_DELETE 4

// Option Numbers according to RFC 7252@ https://tools.ietf.org/html/rfc7252#section-12.2
#define URI_PATH 11
#define CONTENT_FORMAT 12

// Defines for setting up the system clock.
#define SYSTICKHZ               100
#define SYSTICKMS               (1000 / SYSTICKHZ)

// Mongoose connection variables
struct mg_mgr mgr;
struct mg_connection *nc;
static int s_time_to_exit = 0;
static char *s_default_address = "udp://192.168.0.102:5683";



// System clock frequency
uint32_t  ui32SysClock;

// IP address variables
uint32_t g_ui32IPAddress;

// check if IP is set
static volatile bool g_IPAddressSet = false;



