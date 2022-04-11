#include "solver.hpp"
#include "core/SolverTypes.h"
#include "mtl/Vec.h"
#include "optilogcinterface.hpp"


Glucose30Wrapper::Glucose30Wrapper()
{
    solver = new Glucose30::Solver();
}

Glucose30Wrapper::~Glucose30Wrapper()
{
    delete solver;
}

int Glucose30Wrapper::newVar()
{
    return solver->newVar();
}

void Glucose30Wrapper::addClause(const std::vector<int>& literals)
{
    Glucose30::vec<Glucose30::Lit> clauses;
    int max_var = createVectorFromIntToLit(literals, clauses);
    create_vars(max_var);
    solver->addClause(clauses);
}

E_STATE Glucose30Wrapper::solve(const std::vector<int>& assumptions)
{
    Glucose30::vec<Glucose30::Lit> assupmtLit;
    int max_var = createVectorFromIntToLit(assumptions, assupmtLit);

    create_vars(max_var);
    solver->budgetOff();

    Glucose30::lbool res = solveLimitedFullAssignment(assupmtLit);
    return boolToState(res);
}

unsigned int Glucose30Wrapper::getNumClauses()
{
    return solver->nClauses();
}

unsigned int Glucose30Wrapper::getMaxVar()
{
    return solver->nVars();
}

void Glucose30Wrapper::getModel(std::vector<int> &model)
{
    Glucose30::lbool False = Glucose30::lbool((uint8_t)1);
    Glucose30::lbool Undef = Glucose30::lbool((uint8_t)2);
    model.reserve(solver->model.size());
    for(int i = 0; i < solver->model.size(); ++i) {
        if((solver->model[i]) != Undef)
        {
            model.push_back(solver->model[i] == False ? -(i+1) : (i+1));
        }
    }
}

bool Glucose30Wrapper::propagate(const std::vector<int> &assum,
                                 std::vector<int> &prop, int phases)
{
    Glucose30::vec<Glucose30::Lit> assumptions;
    int max_var = createVectorFromIntToLit(assum, assumptions);

    create_vars(max_var);

    Glucose30::vec<Glucose30::Lit> propagated;
    bool res = solver->prop_check(assumptions, propagated, phases);

    createVectorFromLitToInt(propagated, prop, getVariableOpositePolarity);

    return res;
}

void Glucose30Wrapper::setConflictsBudget(long long budget)
{
    if (budget != 0 && budget != -1) { // it is 0 by default
		solver->setConfBudget(budget);
    } else {
        solver->budgetOff();
    }
}

void Glucose30Wrapper::setPropagationBudget(long long budget)
{
    if (budget != 0 && budget != -1) { // it is 0 by default
		solver->setPropBudget(budget);
    } else {
        solver->budgetOff();
    }
}

void Glucose30Wrapper::setIncremental()
{
    solver->setIncrementalMode();
}

E_STATE Glucose30Wrapper::solveLimit(const std::vector<int>& assumptions)
{
    Glucose30::vec<Glucose30::Lit> assupmtLit;
    int max_var = createVectorFromIntToLit(assumptions, assupmtLit);

    create_vars(max_var);

	Glucose30::lbool res = solveLimitedFullAssignment(assupmtLit);
    return boolToState(res);
}

void Glucose30Wrapper::core(std::vector<int>& vectConflict)
{
    createVectorFromLitToInt(solver->conflict, vectConflict, getVariable);
}

bool Glucose30Wrapper::isFullModel()
{
    if(solver->model.size() == 0) return false;

    Glucose30::lbool Undef = Glucose30::lbool((uint8_t)2);

    for(int i = 0; i < solver->model.size(); ++i) {
        if(solver->model[i] == Undef)
        {
            return false;
        }
    }
    return true;
}

long Glucose30Wrapper::getNumConflicts()
{
    return solver->conflicts;
}

void Glucose30Wrapper::traceProof(const char *path)
{
    solver->certifiedOutput = fopen(path, "w+");

	if (solver->certifiedOutput == 0) {
		throw FileException(path);
	}

	setlinebuf(solver->certifiedOutput);
    solver->certifiedUNSAT = true;
    fclose(solver->certifiedOutput);
}

void Glucose30Wrapper::interrupt()
{
    solver->interrupt();
}

void Glucose30Wrapper::clearInterrupt()
{
    solver->clearInterrupt();
}

E_STATE Glucose30Wrapper::solveHardLimited(const std::vector<int>& assumptions)
{
    /*bool currentLubyRestart = solver->luby_restart;
    int currentRestartFactor = solver->luby_restart_factor;
    bool currDisableSimplify = solver->disable_simplify;
    solver->luby_restart = true;
    solver->luby_restart_factor = 1;
    solver->disable_simplify = true;*/

    Glucose30::vec<Glucose30::Lit> assupmtLit;
    int max_var = createVectorFromIntToLit(assumptions, assupmtLit);

    create_vars(max_var);

    Glucose30::lbool res = solveLimitedFullAssignment(assupmtLit);

    /*solver->luby_restart = currentLubyRestart;
    solver->luby_restart_factor = currentRestartFactor;
    solver->disable_simplify = currDisableSimplify;*/

    return boolToState(res);
}

E_STATE Glucose30Wrapper::boolToState(Glucose30::lbool res){
    if(res == Glucose30::lbool((uint8_t)0)) { // l_True
        return E_STATE::SAT;
    } else if(res == Glucose30::lbool((uint8_t)1)) { // l_False
        return E_STATE::UNSAT;
    }
    return E_STATE::UNKNOWN;
}

Glucose30::lbool Glucose30Wrapper::solveLimitedFullAssignment(const Glucose30::vec<Glucose30::Lit>& assupmtLit)
{
    Glucose30::lbool res = solver->solveLimited(assupmtLit);
    if (res == Glucose30::lbool((uint8_t)0)) { // l_True
        res = isFullModel() ? Glucose30::lbool((uint8_t)0) : Glucose30::lbool((uint8_t)1);
    }
    return res;
}


/*============================================================================*/
/*                            PRIVATE METHODS                                 */
/*============================================================================*/

inline void Glucose30Wrapper::create_vars(int max_var)
{
    while (max_var >= solver->nVars()) solver->newVar();
}

inline int Glucose30Wrapper::getVariable(const Glucose30::Lit variable)
{
    return (Glucose30::var(variable) + 1)
         * ( (Glucose30::sign(variable)) ?  1 : -1 );
}

int Glucose30Wrapper::getVariableOpositePolarity(const Glucose30::Lit variable)
{
    return (Glucose30::var(variable) + 1)
         * ( (Glucose30::sign(variable)) ?  -1 : 1 );
}


int Glucose30Wrapper::createVectorFromIntToLit(
        const std::vector<int> &ints,
        Glucose30::vec<Glucose30::Lit> &lits)
{
    int var, max_var = -1;

    lits.capacity(ints.size());
    for (unsigned int  i = 0; i < ints.size(); ++i)
    {
        var = abs(ints[i])-1;
        if(var > max_var)
          max_var = var;

        lits.push( (ints[i] > 0) ? Glucose30::mkLit(var) :
                                  ~Glucose30::mkLit(var) );
    }
    return max_var;
}

void Glucose30Wrapper::createVectorFromLitToInt(
        const Glucose30::vec<Glucose30::Lit> &lits,
        std::vector<int> &ints, callbackFunction getVariablePrt)
{
    ints.reserve(lits.size());
    for(int i = 0; i < lits.size(); ++i)
    {
        ints.push_back( getVariablePrt(lits[i]) );
    }
}

OPTILOG_C_INTERFACE(Glucose30Wrapper, "Glucose30")
