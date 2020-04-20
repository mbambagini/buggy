/** \file xbee.cpp
 *
 *  \brief implementation of the ssWi channel using the XBee module
 *
 */

#include "xbee.hpp"

#include <sstream>

void XBeeModule::readResponse (char* msg)
{
    char c = 0;
    int i = 0;
    while (c!='\r') {
        if (serial_xbee.readable()) {
            c = serial_xbee.getc();
            msg[i++] = c;
        }
    }
}

bool XBeeModule::executeWithOk (const char* cmd)
{
    char msg[5];
    
    serial_xbee.printf("%s", cmd);
    //printf("command %s\r\n", cmd);
    readResponse(msg);
    //printf("response %s\r\n", msg);
    if (strncmp(msg, "OK\r", 3)!=0)
        return false;
        
    return true;
}


void XBeeModule::executeWithRes (const char* cmd, char* res)
{
    serial_xbee.printf("%s", cmd);
    readResponse(res);
}

bool XBeeModule::initSequence()
{
    // wait 1 second before starting
    ThisThread::sleep_for(1000);
    // send start command
    if (!executeWithOk("+++"))
        return false;
    // wait 1 more second after start command
    ThisThread::sleep_for(1000);
    
    return true;
}

XBeeModule::XBeeModule (PinName tx, PinName rx, int panID, int channel) : serial_xbee(tx, rx, 9600)
{
    status = false;
    // read address
    if (!_getLocalAddr())
        return;
    // set channel
    if (!_setChannel(channel))
        return;
    // set pan ID
    if (!_setPanID(panID))
        return;
    status = true;
}

bool XBeeModule::getDstAddress (XBeeAddress &addr)
{
    char tmp[10];
    std::stringstream s1, s2;
    uint32_t high, low;
    
    // start communication with transceiver
    if (!initSequence())
        return false;
    // read high address
    executeWithRes("ATDH \r", tmp);
    s1<<std::hex<<tmp;
    s1>>high;
    // read low address
    executeWithRes("ATDL \r", tmp);
    s2<<std::hex<<tmp;
    s2>>low;
    // terminate
    if (!executeWithOk("ATCN \r"))
        return false;
    addr.setAddress(low, high);
    
    return true;
}


bool XBeeModule::setDstAddress (XBeeAddress addr)
{
    char s[10];
    string low, high;
    
    // start communication with transceiver
    if (!initSequence())
        return false;
    // send high address
    sprintf(s, "ATDH%X \r", addr.getHighAddr());
    if (!executeWithOk(s))
        return false;
    // send low address
    sprintf(s, "ATDL%X \r", addr.getLowAddr());
    if (!executeWithOk(s))
        return false;
    // terminate
    if (!executeWithOk("ATCN \r"))
        return false;
        ThisThread::sleep_for(1000);

    return true;
}

bool XBeeModule::_getLocalAddr ()
{
    char tmp[10];
    uint32_t high, low;
    std::stringstream s1, s2;

    // start communication with transceiver
    if (!initSequence())
        return false;
    // read high address
    executeWithRes("ATSH \r", tmp);
    s1<<std::hex<<tmp;
    s1>>high;
    // read low address
    executeWithRes("ATSL \r", tmp);
    s2<<std::hex<<tmp;
    s2>>low;
    // terminate
    if (!executeWithOk("ATCN \r"))
        return false;
    // save address
    local = XBeeAddress(low, high);
    
    return true;
}


bool XBeeModule::_setChannel (int channel)
{
    char s[10];
    
    // start communication with transceiver
    if (!initSequence())
        return false;
    sprintf(s, "ATCH%d \r", channel);
    if (!executeWithOk(s))
        return false;
    if (!executeWithOk("ATCN \r"))
        return false;
    return true;
}

int XBeeModule::getChannel ()
{
    int channel;
    char s[10];
    
    // start communication with transceiver
    if (!initSequence())
        return false;
    executeWithRes("ATCH \r", s);
    channel = atoi(s);
    if (!executeWithOk("ATCN \r"))
        return -1;
    ThisThread::sleep_for(1000);
    return channel;
}


bool XBeeModule::_setPanID (int panID)
{
    char s[10];
    
    // start communication with transceiver
    if (!initSequence())
        return false;
    sprintf(s, "ATID%d \r", panID);
    if (!executeWithOk(s))
        return false;
    if (!executeWithOk("ATCN \r"))
        return false;
        
    return true;
}

int XBeeModule::getPanID ()
{
    int id;
    char s[10];
    
    // start communication with transceiver
    if (!initSequence())
        return false;
    executeWithRes("ATID \r", s);
    id = atoi(s);
    if (!executeWithOk("ATCN \r"))
        return -1;
    
    return id;
}



int XBeeModule::read (char* msg)
{
    int i = 0;
    while (serial_xbee.readable())
        msg[i++] = serial_xbee.getc();
    return i;
}



void XBeeModule::write (const char* msg, int n)
{
    for (int i=0; i<n; i++) {
        while(!serial_xbee.writeable());
        serial_xbee.putc(msg[i]);
    }
}
