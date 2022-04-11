#include "wrapper.hpp"
#include "optilogcinterface.hpp"


PicosatWrapper::PicosatWrapper()
{
    solver = picosat_init ();
}

PicosatWrapper::~PicosatWrapper()
{
    picosat_reset(solver);
}

void PicosatWrapper::addClause(const std::vector<int>& literals)
{
    for (auto i = literals.begin(); i != literals.end(); ++i)
    {
        picosat_add(solver, *i);
    }
    picosat_add(solver, 0);
}

E_STATE PicosatWrapper::solve(const std::vector<int>& assumptions)
{
    for (auto i = assumptions.begin(); i != assumptions.end(); ++i)
    {
        picosat_assume(solver, *i);
    }

    int result = picosat_sat (solver, -1);
    if(result == 10) return E_STATE::SAT;
    else if(result == 20) return E_STATE::UNSAT;
    return E_STATE::UNKNOWN;
}

unsigned int PicosatWrapper::getNumClauses()
{
    return (unsigned int)picosat_added_original_clauses(solver);
}

unsigned int PicosatWrapper::getMaxVar()
{
    return (unsigned int)picosat_variables(solver);
}

void PicosatWrapper::getModel(std::vector<int> &model)
{
    int maxvar = picosat_variables(solver);
    model.resize(maxvar);

    for(int i = 1; i <= maxvar; i++)
    {
        int lit = picosat_deref(solver, i) > 0 ? i : -i;
        model[i - 1] = lit;
    }
}

OPTILOG_C_INTERFACE(PicosatWrapper, "Picosat965")
