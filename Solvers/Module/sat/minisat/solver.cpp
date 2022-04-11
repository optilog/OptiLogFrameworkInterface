#include "solver.hpp"
#include "core/SolverTypes.h"
#include "mtl/Vec.h"
#include "optilogcinterface.hpp"


MinisatWrapper::MinisatWrapper() {}

MinisatWrapper::~MinisatWrapper() {}

int MinisatWrapper::newVar()
{
    return Solver::newVar();
}

void MinisatWrapper::addClause(const std::vector<int>& literals)
{
    Minisat::vec<Minisat::Lit> clause;
    int max_var = createVectorFromIntToLit(literals, clause);
    createVars(max_var);
    Solver::addClause(clause);
}

E_STATE MinisatWrapper::solve(const std::vector<int>& assumptions)
{
    Minisat::vec<Minisat::Lit> assupmtLit;
    int max_var = createVectorFromIntToLit(assumptions, assupmtLit);

    createVars(max_var);
    Solver::budgetOff();
    Minisat::lbool res = solveLimited(assupmtLit);
    if(res == Minisat::lbool((uint8_t)0)){ // l_True
        return E_STATE::SAT;
    } else if(res == Minisat::lbool((uint8_t)1)) {  // l_False
        return E_STATE::UNSAT;
    }
    return E_STATE::UNKNOWN;
}

unsigned int MinisatWrapper::getNumClauses()
{
    return Solver::nClauses();
}

unsigned int MinisatWrapper::getMaxVar()
{
    return Solver::nVars();
}

void MinisatWrapper::getModel(std::vector<int> &vectModel)
{
    Minisat::lbool False = Minisat::lbool((uint8_t)1);
    Minisat::lbool Undef = Minisat::lbool((uint8_t)2);

    for(int i = 0; i < model.size(); ++i) {
        if(model[i] != Undef)
        {
            vectModel.push_back(model[i] == False ? -(i+1) : (i+1));
        }
    }
}

bool MinisatWrapper::propagate(const std::vector<int> &assum,
                                 std::vector<int> &prop,
                                 int phases)
{
    Minisat::vec<Minisat::Lit> assumptions;
    int max_var = createVectorFromIntToLit(assum, assumptions);

    createVars(max_var);

    Minisat::vec<Minisat::Lit> propagated;
    bool res = Solver::prop_check(assumptions, propagated, phases);

    createVectorFromLitToInt(propagated, prop, getVariableOpositePolarity);

    return res;
}

void MinisatWrapper::setConflictsBudget(long long budget)
{
    if (budget != 0 && budget != -1) {  // it is 0 by default
		Solver::setConfBudget(budget);
    } else {
        Solver::budgetOff();
    }
}

void MinisatWrapper::setPropagationBudget(long long budget)
{
    if (budget != 0 && budget != -1) { // it is 0 by default
		Solver::setPropBudget(budget);
    } else {
        Solver::budgetOff();
    }
}

E_STATE MinisatWrapper::solveLimit(const std::vector<int>& assumptions)
{
    Minisat::vec<Minisat::Lit> assupmtLit;
    int max_var = createVectorFromIntToLit(assumptions, assupmtLit);

    createVars(max_var);

    Minisat::lbool res = Solver::solveLimited(assupmtLit);
    if(res == Minisat::lbool((uint8_t)0)) { // l_True
        return E_STATE::SAT;
    } else if(res == Minisat::lbool((uint8_t)1)) {  // l_False
        return E_STATE::UNSAT;
    }
    return E_STATE::UNKNOWN;
}

void MinisatWrapper::core(std::vector<int>& vectConflict)
{
    createVectorFromLitToInt(conflict, vectConflict, getVariable);
}

void MinisatWrapper::interrupt()
{
    Solver::interrupt();
}

void MinisatWrapper::clearInterrupt()
{
    Solver::clearInterrupt();
}

/*============================================================================*/
/*                            PRIVATE METHODS                                 */
/*============================================================================*/

inline void MinisatWrapper::createVars(int max_var)
{
    while (max_var >= Solver::nVars()) Solver::newVar();
}

int MinisatWrapper::getVariable(const Minisat::Lit variable)
{
    return (Minisat::var(variable) + 1)
         * ( (Minisat::sign(variable)) ?  1 : -1 );
}

int MinisatWrapper::getVariableOpositePolarity(const Minisat::Lit variable)
{
    return (Minisat::var(variable) + 1)
         * ( (Minisat::sign(variable)) ?  -1 : 1 );
}

int MinisatWrapper::createVectorFromIntToLit(
        const std::vector<int> &ints,
        Minisat::vec<Minisat::Lit> &lits)
{
    int var, max_var = -1;

    for (unsigned int  i = 0; i < ints.size(); ++i)
    {
        var = abs(ints[i])-1;
        if(var > max_var)
          max_var = var;

        lits.push( (ints[i] > 0) ? Minisat::mkLit(var) :
                                  ~Minisat::mkLit(var) );
    }
    return max_var;
}

void MinisatWrapper::createVectorFromLitToInt(
        const Minisat::vec<Minisat::Lit> &lits,
        std::vector<int> &ints, callbackFunction getVariablePrt)
{
    for(int i = 0; i < lits.size(); ++i)
    {
        ints.push_back( getVariablePrt(lits[i]) );
    }
}


OPTILOG_C_INTERFACE(MinisatWrapper, "Minisat")