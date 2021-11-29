#include "isat.hpp"


int iSAT::newVar()
{
    throw NotImplementedException();
}

void iSAT::addClause(const std::vector<int>& literals)
{
    (void) literals;
    throw NotImplementedException();
}

E_STATE iSAT::solve(const std::vector<int>& assumptions)
{
    (void) assumptions;
    throw NotImplementedException();
}

unsigned int iSAT::getNumClauses()
{
    throw NotImplementedException();
}

unsigned int iSAT::getMaxVar()
{
    throw NotImplementedException();
}

bool iSAT::propagate(const std::vector<int> &assumptions,
                          std::vector<int> &propagated,
                          int phases)
{
    (void) assumptions;
    (void) propagated;
    (void) phases;
    throw NotImplementedException();
}

void iSAT::getModel(std::vector<int> &model)
{
    (void) model;
    throw NotImplementedException();
}

void iSAT::setConflictsBudget(long long budget)
{
    (void) budget;
    throw NotImplementedException();
}

void iSAT::setPropagationBudget(long long budget)
{
    (void) budget;
    throw NotImplementedException();
}

E_STATE iSAT::solveLimit(const std::vector<int>& assumptions)
{
    (void) assumptions;
    throw NotImplementedException();
}

void iSAT::core(std::vector<int>& conflict)
{
    (void) conflict;
    throw NotImplementedException();
}

void iSAT::setIncremental()
{
    throw NotImplementedException();
}

E_STATE iSAT::getPolarity(int var)
{
    (void) var;
    throw NotImplementedException();
}

void iSAT::setVarPolarity(int var, bool polarity)
{
    (void) var;
    (void) polarity;
    throw NotImplementedException();
}

void iSAT::unsetVarPolarity(int var)
{
    (void) var;
    throw NotImplementedException();
}

void iSAT::traceProof(const char *path)
{
    (void) path;
    throw NotImplementedException();
}

void iSAT::interrupt()
{
    throw NotImplementedException();
}

void iSAT::clearInterrupt()
{
    throw NotImplementedException();
}

E_STATE iSAT::getValue(int variable)
{
    (void) variable;
    throw NotImplementedException();
}

void iSAT::setDecisionVariable(int var, bool b)
{
    (void) var;
    (void) b;
    throw NotImplementedException();
}

bool iSAT::isFullModel()
{
    throw NotImplementedException();
}

long iSAT::getNumConflicts()
{
    throw NotImplementedException();
}

void iSAT::getLearntClauses(std::vector<std::vector<int>> &learntClauses)
{
    (void) learntClauses;
    throw NotImplementedException();
}

E_STATE iSAT::solveHardLimited(const std::vector<int>& assumptions)
{
    (void) assumptions;
    throw NotImplementedException();
}

void iSAT::setStaticHeuristic(std::vector<int> * heuristic)
{
    (void) heuristic;
    throw NotImplementedException();
}

void iSAT::enableStaticHeuristic(bool value)
{
    (void) value;
    throw NotImplementedException();
}

void iSAT::getActivities(std::vector<double>& activities)
{
    (void) activities;
    throw NotImplementedException();
}

void iSAT::set(const char * key, int value)
{
    (void) key;
    (void) value;
    throw NotImplementedException();
}

void iSAT::set(const char * key, double value)
{
    (void) key;
    (void) value;
    throw NotImplementedException();
}

void iSAT::set(const char * key, bool value)
{
    (void) key;
    (void) value;
    throw NotImplementedException();
}

void iSAT::set(const char * key, char * value)
{
    (void) key;
    (void) value;
    throw NotImplementedException();
}

void iSAT::get(const char * key, Parameter & value)
{
    (void) key;
    (void) value;
    throw NotImplementedException();
}

int iSAT::getVersion()
{
    return 1;
}