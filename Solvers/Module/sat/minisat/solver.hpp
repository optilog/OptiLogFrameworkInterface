#ifndef _MINISAT_WRAPPER_HPP_
#define _MINISAT_WRAPPER_HPP_

#include <vector>
#include "isat.hpp"
#include "core/Solver.h"

class MinisatWrapper : public iSAT, public Minisat::Solver {
public:
    MinisatWrapper();
    ~MinisatWrapper();

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
    void core(std::vector<int>& conflict) override;
    void interrupt() override;
    void clearInterrupt() override;

private:
    typedef int (*callbackFunction)(Minisat::Lit);  // type for lit to variable

    inline void createVars(int max_var);
    static int getVariable(const Minisat::Lit variable);
    static int getVariableOpositePolarity(const Minisat::Lit variable);
    int createVectorFromIntToLit(const std::vector<int> &ints,
                                Minisat::vec<Minisat::Lit> &lits);
    void createVectorFromLitToInt(const Minisat::vec<Minisat::Lit> &lits,
                                std::vector<int> &ints,
                                callbackFunction getVariablePrt);
};

#endif // _MINISAT_WRAPPER_HPP_
