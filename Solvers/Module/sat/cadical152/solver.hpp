#ifndef _CADICAL_WRAPPER_HPP_
#define _CADICAL_WRAPPER_HPP_

#include <vector>
#include "isat.hpp"
#include "cadical.hpp"

class CadicalWrapper152 : public iSAT {
public:
    CadicalWrapper152();
    CadicalWrapper152(CadicalWrapper152& wrapper);
    virtual ~CadicalWrapper152();

    void addClause(const std::vector<int>& literals) override;
    E_STATE solve(const std::vector<int>& assumptions) override;
    unsigned int getNumClauses() override;
    unsigned int getMaxVar() override;
    void getModel(std::vector<int> &model) override;
    void core(std::vector<int>& conflict) override;
    void traceProof(const char *path) override;
    void interrupt() override;
    void clearInterrupt() override;
    iSAT* clone() override;

    void set(const char* key, int value) override;
    void get(const char * key, Parameter & value) override;

private:
    CaDiCaL::Solver * solver = new CaDiCaL::Solver;
    std::vector<int> lastAssumptions;
};

#endif // _CADICAL_WRAPPER_HPP_
