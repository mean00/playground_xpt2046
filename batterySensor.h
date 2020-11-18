#include "simpleADC.h"

/**
 * 
 * @param pinV
 * @param pinA
 */
class BatterySensor
{
public:
                            BatterySensor(int pinV, int pinA);
            bool            rawRead(float &v, float &a);
            bool            readVoltageCurrent(float &volt,float &current);
protected:
            simpleAdc    *_adc;
            int           _pinV, _pinA;
            float         _vcc;
            
protected:
            
};

// EOF