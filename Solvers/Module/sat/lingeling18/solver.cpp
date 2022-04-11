#include <stdlib.h>
#include "solver.hpp"
#include "optilogcinterface.hpp"

Lingeling18Wrapper::Lingeling18Wrapper()
{
    solver = lglinit();
    lglsetopt(solver, "simplify", 0);
}

Lingeling18Wrapper::~Lingeling18Wrapper()
{
    lglrelease(solver);
}

void Lingeling18Wrapper::addClause(const std::vector<int>& literals)
{
    for (auto i = literals.begin(); i != literals.end(); ++i)
    {
        lgladd(solver, *i);
		lglfreeze(solver, abs(*i));
    }
    lgladd(solver, 0);
}

E_STATE Lingeling18Wrapper::solve(const std::vector<int>& assumptions)
{
    for (auto i = assumptions.begin(); i != assumptions.end(); ++i)
    {
        lglassume(solver, *i);
    }
    int result = lglsat(solver);
    if(result == 10) return E_STATE::SAT;
    else if(result == 20) return E_STATE::UNSAT;
    return E_STATE::UNKNOWN;
}

unsigned int Lingeling18Wrapper::getNumClauses()
{
    return (unsigned int)lglnclauses(solver);
}

unsigned int Lingeling18Wrapper::getMaxVar()
{
    return (unsigned int)lglmaxvar(solver);
}

void Lingeling18Wrapper::getModel(std::vector<int> &model)
{
    int maxvar = lglmaxvar(solver);
    model.resize(maxvar);

    for(unsigned int i = 1; i <= maxvar; i++)
    {
        int lit = lglderef(solver, i) > 0 ? i : -i;
        model[i - 1] = lit;
    }
}

OPTILOG_C_INTERFACE(Lingeling18Wrapper, "Lingeling18")