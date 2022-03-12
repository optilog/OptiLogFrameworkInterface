#ifndef _OPTILOG_C_INTERFACE_HPP
#define _OPTILOG_C_INTERFACE_HPP

#include <vector>
#include "isat.hpp"
#include "configparameter.hpp"
#include "parameter.hpp"
#include "pyexceptions.hpp"

#define OPTILOG_C_INTERFACE(name_class, name_solver)\
                                                    \
extern "C" const char* name_isat_solver() {         \
  return name_solver;                               \
}                                                   \
                                                    \
extern "C" iSAT* create_isat_solver()               \
{                                                   \
  return new name_class();                          \
}                                                   \
                                                    \
extern "C" void destroy_isat_solver( iSAT* object ) \
{                                                   \
  /* cast to internal type */                       \
  name_class* pc = (name_class*) object;            \
  /* delete pointer */                              \
  delete pc;                                        \
}     

#define OPTILOG_CONFIG_C_INTERFACE(static_vec)                            \
extern "C" void configurable_isat_solver(std::vector<OptilogConfigParameter>* target)  \
{                                                                         \
  copy(static_vec.begin(), static_vec.end(), back_inserter(*target));     \
}

#endif