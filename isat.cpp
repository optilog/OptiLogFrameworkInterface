#include "isat.hpp"


int iSAT::newVar()
{
    throw NotImplementedException("newVar");
}

void iSAT::addClause(const std::vector<int>& literals)
{
    (void) literals;
    throw NotImplementedException("addClause");
}

E_STATE iSAT::solve(const std::vector<int>& assumptions)
{
    (void) assumptions;
    throw NotImplementedException("solve");
}

unsigned int iSAT::getNumClauses()
{
    throw NotImplementedException("getNumClauses");
}

unsigned int iSAT::getMaxVar()
{
    throw NotImplementedException("getMaxVar");
}

bool iSAT::propagate(const std::vector<int> &assumptions,
                          std::vector<int> &propagated,
                          int phases)
{
    (void) assumptions;
    (void) propagated;
    (void) phases;
    throw NotImplementedException("propagate");
}

void iSAT::getModel(std::vector<int> &model)
{
    (void) model;
    throw NotImplementedException("getModel");
}

void iSAT::setConflictsBudget(long long budget)
{
    (void) budget;
    throw NotImplementedException("setConflictsBudget");
}

void iSAT::setPropagationBudget(long long budget)
{
    (void) budget;
    throw NotImplementedException("setPropagationBudget");
}

E_STATE iSAT::solveLimit(const std::vector<int>& assumptions)
{
    (void) assumptions;
    throw NotImplementedException("solveLimit");
}

void iSAT::core(std::vector<int>& conflict)
{
    (void) conflict;
    throw NotImplementedException("core");
}

void iSAT::setIncremental()
{
    throw NotImplementedException("setIncremental");
}

E_STATE iSAT::getPolarity(int var)
{
    (void) var;
    throw NotImplementedException("getPolarity");
}

void iSAT::setVarPolarity(int var, bool polarity)
{
    (void) var;
    (void) polarity;
    throw NotImplementedException("setVarPolarity");
}

void iSAT::unsetVarPolarity(int var)
{
    (void) var;
    throw NotImplementedException("unsetVarPolarity");
}

void iSAT::traceProof(const char *path)
{
    (void) path;
    throw NotImplementedException("traceProof");
}

void iSAT::interrupt()
{
    throw NotImplementedException("interrupt");
}

void iSAT::clearInterrupt()
{
    throw NotImplementedException("clearInterrupt");
}

E_STATE iSAT::getValue(int variable)
{
    (void) variable;
    throw NotImplementedException("getValue");
}

void iSAT::setDecisionVariable(int var, bool b)
{
    (void) var;
    (void) b;
    throw NotImplementedException("setDecisionVariable");
}

bool iSAT::isFullModel()
{
    throw NotImplementedException("isFullModel");
}

long iSAT::getNumConflicts()
{
    throw NotImplementedException("getNumConflicts");
}

void iSAT::getLearntClauses(std::vector<std::vector<int>> &learntClauses)
{
    (void) learntClauses;
    throw NotImplementedException("getLearntClauses");
}

E_STATE iSAT::solveHardLimited(const std::vector<int>& assumptions)
{
    (void) assumptions;
    throw NotImplementedException("solveHardLimited");
}

void iSAT::setStaticHeuristic(std::vector<int> * heuristic)
{
    (void) heuristic;
    throw NotImplementedException("setStaticHeuristic");
}

void iSAT::enableStaticHeuristic(bool value)
{
    (void) value;
    throw NotImplementedException("enableStaticHeuristic");
}

void iSAT::getActivities(std::vector<double>& activities)
{
    (void) activities;
    throw NotImplementedException("getActivities");
}

void iSAT::set(const char * key, int value)
{
    (void) key;
    (void) value;
    throw NotImplementedException("set");
}

void iSAT::set(const char * key, double value)
{
    (void) key;
    (void) value;
    throw NotImplementedException("set");
}

void iSAT::set(const char * key, bool value)
{
    (void) key;
    (void) value;
    throw NotImplementedException("set");
}

void iSAT::set(const char * key, char * value)
{
    (void) key;
    (void) value;
    throw NotImplementedException("set");
}

void iSAT::get(const char * key, Parameter & value)
{
    (void) key;
    (void) value;
    throw NotImplementedException("get");
}

int iSAT::getVersion()
{
    return 2;
}