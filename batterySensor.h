#include "adcPoll.h"

/**
 * 
 * @param pinV
 * @param pinA
 */
class BatterySensor: public xTask
{
public:
                            BatterySensor(AdcPoll *poller, int pinV, int pinA);
            bool            rawRead(float &v, float &a);
            bool            getVoltageCurrent(float &v, float &a)
            {
                v=_voltage;
                a=_current;
                return true;
            }
protected:
            bool           readVoltageCurrent(float &volt,float &current);
            void           run();
            AdcPoll       *_adc;
            int           _pinV, _pinA;
            float         _vcc;
            float         _voltage,_current;
            AdcPollClient _currentPoller, _voltagePoller;
            
protected:
            
};

// EOF