#include "configparameter.hpp"
#include <string.h>

OptilogConfigParameter::OptilogConfigParameter(const char* name, long lower_bound, long upper_bound, long default_value) {
    int len_name = strlen(name) + 1;
    this->name = new char[len_name];
    this->type = OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_LONG;
    this->reserveSpace();
    memcpy(this->default_value, &default_value, sizeof(long));
    memcpy(this->lower_bound, &lower_bound, sizeof(long));
    memcpy(this->upper_bound, &upper_bound, sizeof(long));
    memcpy(this->name, name, sizeof(char) * len_name);
}

OptilogConfigParameter::OptilogConfigParameter(const char* name, int lower_bound, int upper_bound, int default_value) :
OptilogConfigParameter(name, (long) lower_bound, (long) upper_bound, (long) default_value) { }

OptilogConfigParameter::OptilogConfigParameter(const char* name, double lower_bound, double upper_bound, double default_value) {
    int len_name = strlen(name) + 1;
    this->name = new char[len_name];
    this->type = OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_DOUBLE;
    this->reserveSpace();
    memcpy(this->default_value, &default_value, sizeof(double));
    memcpy(this->lower_bound, &lower_bound, sizeof(double));
    memcpy(this->upper_bound, &upper_bound, sizeof(double));
    memcpy(this->name, name, sizeof(char) * len_name);
}

OptilogConfigParameter::OptilogConfigParameter(const char* name, float lower_bound, float upper_bound, float default_value) :
OptilogConfigParameter(name, (double) lower_bound, (double) upper_bound, (double) default_value) { }

OptilogConfigParameter::OptilogConfigParameter(const char* name, bool default_value) {
    int len_name = strlen(name) + 1;
    this->name = new char[len_name];
    this->type = OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_BOOL;
    this->reserveSpace();
    bool f = false;
    bool t = true;
    memcpy(this->default_value, &default_value, sizeof(bool));
    memcpy(this->lower_bound, &f, sizeof(bool));
    memcpy(this->upper_bound, &t, sizeof(bool));
    memcpy(this->name, name, sizeof(char) * len_name);
}

OptilogConfigParameter::OptilogConfigParameter(OptilogConfigParameter const& other) {
    int len_name = strlen(other.name) + 1;
    this->type = other.type;
    this->name = new char[len_name];
    this->reserveSpace();
    int size = this->sizeOfValues();
    memcpy(this->default_value, other.default_value, size);
    memcpy(this->lower_bound, other.lower_bound, size);
    memcpy(this->upper_bound, other.upper_bound, size);
    memcpy(this->name, other.name, sizeof(char) * len_name);
}

OPTILOG_CONFIG_PARAMETER_TYPE OptilogConfigParameter::getType(){
    return this->type;
}

OptilogConfigParameter::~OptilogConfigParameter(){
    if(this->type == OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_LONG)
    {
        delete ((long *)(this->default_value));
        delete ((long *)(this->lower_bound));
        delete ((long *)(this->upper_bound));
    } else if (this->type == OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_DOUBLE){
        delete ((double *)(this->default_value));
        delete ((double *)(this->lower_bound));
        delete ((double *)(this->upper_bound));
    } else if (this->type == OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_BOOL){
        delete ((bool *)(this->default_value));
        delete ((bool *)(this->lower_bound));
        delete ((bool *)(this->upper_bound));
    }
    delete this->name;
}

void OptilogConfigParameter::reserveSpace(){
    if(this->type == OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_LONG)
    {
        this->default_value = new long;
        this->lower_bound = new long;
        this->upper_bound = new long;
    } else if (this->type == OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_DOUBLE){
        this->default_value = new double;
        this->lower_bound = new double;
        this->upper_bound = new double;
    } else if (this->type == OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_BOOL){
        this->default_value = new bool;
        this->lower_bound = new bool;
        this->upper_bound = new bool;
    }
}

int OptilogConfigParameter::sizeOfValues(){
    if(this->type == OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_LONG)
    {
        return sizeof(long);
    } else if (this->type == OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_DOUBLE){
        return sizeof(double);
    } else if (this->type == OPTILOG_CONFIG_PARAMETER_TYPE::OPTI_CONFIG_BOOL){
        return sizeof(bool);
    }
    return 0;
}

void* OptilogConfigParameter::getLowerBound(){
    return this->lower_bound;
}

void* OptilogConfigParameter::getUpperBound(){
    return this->upper_bound;
}

void* OptilogConfigParameter::getDefaultValue(){
    return this->default_value;   
}

const char* OptilogConfigParameter::getName(){
    return this->name;
}