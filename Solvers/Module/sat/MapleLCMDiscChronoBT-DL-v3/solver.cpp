#include "solver.hpp"
#include "core/SolverTypes.h"
#include "mtl/Vec.h"
#include "optilogcinterface.hpp"


MapleLCMChronoBTDLv3Wrapper::MapleLCMChronoBTDLv3Wrapper() {}

MapleLCMChronoBTDLv3Wrapper::~MapleLCMChronoBTDLv3Wrapper() {}

int MapleLCMChronoBTDLv3Wrapper::newVar()
{
    return Solver::newVar();
}

void MapleLCMChronoBTDLv3Wrapper::addClause(const std::vector<int>& literals)
{
    Minisat::vec<Minisat::Lit> clause;
    int max_var = createVectorFromIntToLit(literals, clause);
    createVars(max_var);
    Solver::addClause(clause);
}

E_STATE MapleLCMChronoBTDLv3Wrapper::solve(const std::vector<int>& assumptions)
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

unsigned int MapleLCMChronoBTDLv3Wrapper::getNumClauses()
{
    return Solver::nClauses();
}

unsigned int MapleLCMChronoBTDLv3Wrapper::getMaxVar()
{
    return Solver::nVars();
}

void MapleLCMChronoBTDLv3Wrapper::getModel(std::vector<int> &vectModel)
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

bool MapleLCMChronoBTDLv3Wrapper::propagate(const std::vector<int> &assum,
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

void MapleLCMChronoBTDLv3Wrapper::setConflictsBudget(long long budget)
{
    if (budget != 0 && budget != -1) {  // it is 0 by default
		Solver::setConfBudget(budget);
    } else {
        Solver::budgetOff();
    }
}

void MapleLCMChronoBTDLv3Wrapper::setPropagationBudget(long long budget)
{
    if (budget != 0 && budget != -1) { // it is 0 by default
		Solver::setPropBudget(budget);
    } else {
        Solver::budgetOff();
    }
}

E_STATE MapleLCMChronoBTDLv3Wrapper::solveLimit(const std::vector<int>& assumptions)
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

void MapleLCMChronoBTDLv3Wrapper::core(std::vector<int>& vectConflict)
{
    createVectorFromLitToInt(conflict, vectConflict, getVariable);
}

void MapleLCMChronoBTDLv3Wrapper::interrupt()
{
    Solver::interrupt();
}

void MapleLCMChronoBTDLv3Wrapper::clearInterrupt()
{
    Solver::clearInterrupt();
}

/*============================================================================*/
/*                            PRIVATE METHODS                                 */
/*============================================================================*/

inline void MapleLCMChronoBTDLv3Wrapper::createVars(int max_var)
{
    while (max_var >= Solver::nVars()) Solver::newVar();
}

int MapleLCMChronoBTDLv3Wrapper::getVariable(const Minisat::Lit variable)
{
    return (Minisat::var(variable) + 1)
         * ( (Minisat::sign(variable)) ?  1 : -1 );
}

int MapleLCMChronoBTDLv3Wrapper::getVariableOpositePolarity(const Minisat::Lit variable)
{
    return (Minisat::var(variable) + 1)
         * ( (Minisat::sign(variable)) ?  -1 : 1 );
}

int MapleLCMChronoBTDLv3Wrapper::createVectorFromIntToLit(
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

void MapleLCMChronoBTDLv3Wrapper::createVectorFromLitToInt(
        const Minisat::vec<Minisat::Lit> &lits,
        std::vector<int> &ints, callbackFunction getVariablePrt)
{
    for(int i = 0; i < lits.size(); ++i)
    {
        ints.push_back( getVariablePrt(lits[i]) );
    }
}


OPTILOG_C_INTERFACE(MapleLCMChronoBTDLv3Wrapper, "MapleLCMChronoBTDLv3")