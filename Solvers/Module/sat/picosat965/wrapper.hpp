#ifndef _PICOSAT_WRAPPER_HPP_
#define _PICOSAT_WRAPPER_HPP_

#include <vector>
#include "isat.hpp"

extern "C" {
  #include "picosat.h"
}

class PicosatWrapper : public iSAT {
public:
  PicosatWrapper();
  virtual ~PicosatWrapper();

  void addClause(const std::vector<int>& literals) override;
  E_STATE solve(const std::vector<int>& assumptions) override;
  unsigned int getNumClauses() override;
  unsigned int getMaxVar() override;
  void getModel(std::vector<int> &model) override;

private:
  PicoSAT* solver;
};

#endif // _PICOSAT_WRAPPER_HPP_
