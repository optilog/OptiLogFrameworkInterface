#include <string.h>
#include "parameter.hpp"

Parameter::Parameter()
{
    this->type = PARAMETER_TYPE::UNDEF;
}

void Parameter::set(int value)
{
    freeValue();
    this->value = new int;
    this->type = PARAMETER_TYPE::INTEGER;
    memcpy(this->value, &value, sizeof(int));
}

void Parameter::set(long value)
{
    freeValue();
    this->value = new long;
    this->type = PARAMETER_TYPE::LONG;
    memcpy(this->value, &value, sizeof(long));
}

void Parameter::set(unsigned int value)
{
    freeValue();
    this->value = new unsigned int;
    this->type = PARAMETER_TYPE::UNSIGNED_INTEGER;
    memcpy(this->value, &value, sizeof(unsigned int));
}

void Parameter::set(unsigned long value)
{
    freeValue();
    this->value = new unsigned long;
    this->type = PARAMETER_TYPE::UNSIGNED_INTEGER;
    memcpy(this->value, &value, sizeof(unsigned long));
}

void Parameter::set(float value)
{
    freeValue();
    this->value = new float;
    this->type = PARAMETER_TYPE::FLOAT;
    memcpy(this->value, &value, sizeof(float));
}

void Parameter::set(double value)
{
    freeValue();
    this->value = new double;
    this->type = PARAMETER_TYPE::DOUBLE;
    memcpy(this->value, &value, sizeof(double));
}

void Parameter::set(bool value)
{
    freeValue();
    this->value = new bool;
    this->type = PARAMETER_TYPE::BOOL;
    memcpy(this->value, &value, sizeof(bool));
}

void Parameter::set(const char * value)
{
    freeValue();
    this->value = new char[strlen(value)+1];
    this->type = PARAMETER_TYPE::STRING;
    memcpy(this->value, &value, strlen(value)+1);
}

Parameter::~Parameter()
{
    freeValue();
}

void Parameter::freeValue()
{
    if(this->type == PARAMETER_TYPE::INTEGER)
    {
        delete ((int *)(this->value));
    } else if (this->type == PARAMETER_TYPE::UNSIGNED_INTEGER){
        delete ((unsigned int *)(this->value));
    } else if (this->type == PARAMETER_TYPE::LONG){
        delete ((long *)(this->value));
    } else if (this->type == PARAMETER_TYPE::UNSIGNED_LONG){
        delete ((unsigned long *)(this->value));
    } else if (this->type == PARAMETER_TYPE::FLOAT){
        delete ((float *)(this->value));
    } else if (this->type == PARAMETER_TYPE::DOUBLE){
        delete ((double *)(this->value));
    } else if (this->type == PARAMETER_TYPE::BOOL){
        delete ((bool *)(this->value));
    } else if (this->type == PARAMETER_TYPE::STRING){
        delete ((char *)(this->value));
    }
}

PARAMETER_TYPE Parameter::getType()
{
    return this->type;
}

void * Parameter::getValue()
{
    return this->value;
}