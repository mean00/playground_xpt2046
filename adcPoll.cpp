/**
 * 
 * @param poller
 * @param pin
 */
#include "Wire.h"
#include "adcPoll.h"
#include "simpleADC.h"

/**
 */
AdcPollClient::AdcPollClient(int pin,AdcPoll &poller)
{
    _value=0;
    poller.add(pin, this);
}
/**
 */
void AdcPoll::add(int pin, AdcPollClient *client)
{
    _pins[_nbPoint]=pin;
    _clients[_nbPoint]=client;
    _nbPoint++;
    if(_nbPoint>8) xAssert(0);
}
/**
 */    
void PollTrampoline(void *c)
{
    AdcPoll *a=(AdcPoll *)c;
    a->run();
}
/**
 * 
 */
void AdcPoll::start(void)
{
    BaseType_t er=xTaskCreate(PollTrampoline,"AdcPoll",200, this,4,&_taskHandle);
    xAssert(er==pdPASS);    
}
/**
 * 
 */
void AdcPoll::run(void)
{
    simpleAdc adc(_pins[0]);    
    adc.setPins(_nbPoint, _pins);
    int f=_frequency*16*_nbPoint;
    while(1)
    {
        int nb=16*_nbPoint;
        uint16_t *data=NULL;
        if(!adc.timeSample(nb, &data, f)) xAssert(0);
        for(int i=0;i<_nbPoint;i++)
        {
            int sum=0;
            uint16_t *d=data+i;
            for(int j=0;j<16;j++)
            {
                sum+=*d;
                d+=_nbPoint;
            }
            sum=(sum+7)/16;
            _values[i]=sum;
        }
        for(int i=0;i<_nbPoint;i++)
            _clients[i]->setValue(_values[i]);
    }
}

//EOF

