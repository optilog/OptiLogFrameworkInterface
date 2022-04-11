#ifndef _PYCONFIGPARAMETER_HPP_
#define _PYCONFIGPARAMETER_HPP_

enum OPTILOG_CONFIG_PARAMETER_TYPE { OPTI_CONFIG_LONG, OPTI_CONFIG_DOUBLE, OPTI_CONFIG_BOOL };

class OptilogConfigParameter {
    public:
        OptilogConfigParameter(const char* name, long lower_bound, long upper_bound, long default_value);
        OptilogConfigParameter(const char* name, int lower_bound, int upper_bound, int default_value);
        OptilogConfigParameter(const char* name, double lower_bound, double upper_bound, double default_value);
        OptilogConfigParameter(const char* name, float lower_bound, float upper_bound, float default_value);
        OptilogConfigParameter(const char* name, bool default_value);
        OptilogConfigParameter(OptilogConfigParameter const& other);
        ~OptilogConfigParameter();
        OPTILOG_CONFIG_PARAMETER_TYPE getType();
        void* getLowerBound();
        void* getUpperBound();
        void* getDefaultValue();
        const char* getName();
    private:
        void reserveSpace();
        int sizeOfValues();
        char* name;
        void* lower_bound;
        void* upper_bound;
        void* default_value;
        OPTILOG_CONFIG_PARAMETER_TYPE type;
};

#endif