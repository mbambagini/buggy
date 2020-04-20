/** \file ssWiPort.cpp
 *  \brief Implementation of the dual head port
 *
 */

#include "ssWiPort.hpp"


PortValue ssWiPort::getTXValue()
{
    PortValue tmp = valueTX;
    modified = false;
    return tmp;
}

void ssWiPort::setTXValue(PortValue tmp)
{
    valueTX = tmp;
    modified = true;
}

bool ssWiPort::isModified()
{
    return modified;
}

PortValue ssWiPort::getRXValue()
{
    return valueRX;
}

void ssWiPort::setRXValue(PortValue tmp)
{
    valueRX = tmp;
}
