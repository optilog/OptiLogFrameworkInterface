#ifndef _PYPARAMETER_HPP_
#define _PYPARAMETER_HPP_

#include <cstddef>

enum PARAMETER_TYPE { OPTI_PT_INTEGER, OPTI_PT_UNSIGNED_INTEGER, OPTI_PT_LONG, OPTI_PT_UNSIGNED_LONG, OPTI_PT_FLOAT, OPTI_PT_DOUBLE, OPTI_PT_STRING, OPTI_PT_BOOL, OPTI_PT_UNDEF };

class Parameter
{
public:
    Parameter();
    ~Parameter();
    void set(int value);
    void set(long value);
    void set(unsigned int value);
    void set(unsigned long value);
    void set(float value);
    void set(double value);
    void set(bool value);
    void set(const char * value);
    PARAMETER_TYPE getType();
    void * getValue();

private:
    PARAMETER_TYPE type;
    void * value = NULL;
    void freeValue();
};

#endif
