#ifndef _GLUCOSE30_WRAPPER_HPP_
#define _GLUCOSE30_WRAPPER_HPP_

#include <vector>
#include "isat.hpp"
#include "core/Solver.h"

class Glucose30Wrapper : public iSAT {
public:
    Glucose30Wrapper();
    virtual ~Glucose30Wrapper();

    int newVar() override;
    void addClause(const std::vector<int>& literals) override;
    E_STATE solve(const std::vector<int>& assumptions) override;
    unsigned int getNumClauses() override;
    unsigned int getMaxVar() override;
    void getModel(std::vector<int> &model) override;
    bool propagate(const std::vector<int> &assumptions,
                   std::vector<int> &propagated,
                   int phases) override;
    void setConflictsBudget(long long budget) override;
    void setPropagationBudget(long long budget) override;
    E_STATE solveLimit(const std::vector<int>& assumptions) override;
    E_STATE solveHardLimited(const std::vector<int>& assumptions) override;
    void core(std::vector<int>& conflict) override;
    bool isFullModel() override;
    long getNumConflicts() override;
    void setIncremental() override;
    void traceProof(const char *path) override;
    void interrupt() override;
    void clearInterrupt() override;

private:
    Glucose30::Solver* solver;
    E_STATE boolToState(Glucose30::lbool b);
    Glucose30::lbool solveLimitedFullAssignment(const Glucose30::vec<Glucose30::Lit>& assupmtLit);
    typedef int (*callbackFunction)(Glucose30::Lit);  // type for lit to variable

    inline void create_vars(int max_var);
    static int getVariable(const Glucose30::Lit variable);
    static int getVariableOpositePolarity(const Glucose30::Lit variable);
    int createVectorFromIntToLit(const std::vector<int> &ints,
                                 Glucose30::vec<Glucose30::Lit> &lits);
    void createVectorFromLitToInt(const Glucose30::vec<Glucose30::Lit> &lits,
                                  std::vector<int> &ints,
                                  callbackFunction getVariablePrt);
};

#endif // _GLUCOSE30_WRAPPER_HPP_
