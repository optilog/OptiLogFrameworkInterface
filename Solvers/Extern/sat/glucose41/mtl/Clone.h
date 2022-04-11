#ifndef Glucose41_Clone_h
#define Glucose41_Clone_h


namespace Glucose41 {

    class Clone {
        public:
          virtual Clone* clone() const = 0;
    };
};

#endif