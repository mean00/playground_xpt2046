#pragma once

/**
 This polls N pins and update the client "value"
 It contains races, probably not a problem
 
 */

#include "MapleFreeRTOS1000_pp.h"
#include "simpleADC.h"

class AdcPoll;
/**
 * 
 * @param poller
 * @param pin
 */
class AdcPollClient
{
public:
                AdcPollClient(int pin, AdcPoll &poller);
        int     getValue() {return _value;}
  virtual void  setValue(int v) {_value=v;}
        
        // This is used by AdcPoll
protected:
        int     _value;
};

/**
 */
class AdcPoll
{
public:    
    AdcPoll(int frequency)
    {
        _nbPoint=0;
        _frequency=frequency;
        _adc=new simpleAdc(PA0);    ;
    }
    virtual ~AdcPoll()
    {
        
    }
    void    add(int pin, AdcPollClient *client);    
    void    start();
    void    run();
     float  getVcc() {return _adc->getVcc();}
protected:
    int             _nbPoint;
    int             _pins[8];
    AdcPollClient * _clients[8];
    uint16_t        _values[8];
    TaskHandle_t    _taskHandle;
    int             _frequency;
    simpleAdc       *_adc;    
};