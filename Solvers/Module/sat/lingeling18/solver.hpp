#ifndef _LINGELING18_WRAPPER_HPP_
#define _LINGELING18_WRAPPER_HPP_

#include <vector>
#include "isat.hpp"

extern "C" {
    #include "lglib.h"
    LGL* lglinit(void);
    void lglsetopt(LGL*, const char*, int);
    void lglrelease(LGL*);
    void lgladd(LGL*, int);
    void lglfreeze(LGL*, int);
    void lglassume(LGL*, int);
    int lglsat(LGL*);
    int lglnclauses(LGL*);
    int lglmaxvar(LGL*);
    int lglderef (LGL *, int lit);
}

class Lingeling18Wrapper : public iSAT {
public:
  Lingeling18Wrapper();
  ~Lingeling18Wrapper();

  void addClause(const std::vector<int>& literals) override;
  E_STATE solve(const std::vector<int>& assumptions) override;
  unsigned int getNumClauses() override;
  unsigned int getMaxVar() override;
  void getModel(std::vector<int> &model) override;

private:
  LGL* solver;
};

#endif // _LINGELING_WRAPPER_HPP_
