#pragma once
#include "MapleFreeRTOS1000_pp.h"


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
        
        // This is used by AdcPoll
        int     _index;
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
    }
    virtual ~AdcPoll()
    {
        
    }
    void add(int pin, AdcPollClient *client);    
    void start();
    void run();
protected:
    int             _nbPoint;
    int             _pins[8];
    AdcPollClient * _clients[8];
    uint16_t        _values[8];
    TaskHandle_t    _taskHandle;
    int             _frequency;
};