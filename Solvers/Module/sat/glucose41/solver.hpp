#ifndef _GLUCOSE41_WRAPPER_HPP_
#define _GLUCOSE41_WRAPPER_HPP_

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdio.h>
#include "isat.hpp"
#include "core/Solver.h"


class Glucose41Wrapper : public iSAT, public Glucose41::Solver {

public:
    Glucose41Wrapper();
    Glucose41Wrapper(Glucose41Wrapper& wrapper);
    virtual ~Glucose41Wrapper();

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
    void setIncremental() override;
    E_STATE getPolarity(int var) override;
    void setVarPolarity(int var, bool polarity) override;
    void unsetVarPolarity(int var) override;
    void traceProof(const char *path) override;
    void interrupt() override;
    void clearInterrupt() override;

    E_STATE getValue(int variable) override;
    void setDecisionVariable(int var, bool b) override;
    bool isFullModel() override;
    long getNumConflicts() override;
    void getLearntClauses(std::vector<std::vector<int>> &learntClauses) override;
    E_STATE solveHardLimited(const std::vector<int>& assumptions) override;

    void setStaticHeuristic(std::vector<int> * heuristic) override;
    void enableStaticHeuristic(bool value) override;
    void getActivities(std::vector<double>& activities) override;
    iSAT* clone() override;

    void set(const char* key, int value) override;
    void set(const char* key, double value) override;
    void set(const char * key, bool value) override;
    void get(const char * key, Parameter & value) override;

protected:
    std::unordered_map<std::string, double*> doubleParams;
    std::unordered_map<std::string, uint32_t*> uintegerParams;
    std::unordered_map<std::string, int*> integerParams;
    std::unordered_map<std::string, bool*> booleanParams;

private:
    Glucose41::lbool solveLimitedFullAssignment(const Glucose41::vec<Glucose41::Lit>& assupmtLit);
    E_STATE boolToState(Glucose41::lbool b);
    typedef int (*callbackFunction)(Glucose41::Lit);  // type for lit to variable
    inline void createVars(int max_var);
    static int getVariable(const Glucose41::Lit variable);
    static int getVariableOpositePolarity(const Glucose41::Lit variable);
    int createVectorFromIntToLit(const std::vector<int> &ints,
                                 Glucose41::vec<Glucose41::Lit> &lits);
    void createVectorFromLitToInt(const Glucose41::vec<Glucose41::Lit> &lits,
                                  std::vector<int> &ints, 
                                  callbackFunction getVariablePrt);

    template<typename T> bool setParameterValue(const char * key, T value, 
                                std::unordered_map<std::string, T*> & paramsMap);

    template<typename T> bool getParameterValue(const char * key, Parameter & value, 
                                std::unordered_map<std::string, T> & paramsMap);

    void createMaps();
};

#endif
