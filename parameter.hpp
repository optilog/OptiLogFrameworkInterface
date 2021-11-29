#ifndef _PYPARAMETER_HPP_
#define _PYPARAMETER_HPP_

#include <cstddef>

enum PARAMETER_TYPE { INTEGER, UNSIGNED_INTEGER, LONG, UNSIGNED_LONG, FLOAT, DOUBLE, STRING, BOOL, UNDEF };

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
