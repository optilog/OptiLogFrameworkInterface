#include "solver.hpp"
#include "optilogcinterface.hpp"


CadicalWrapper::CadicalWrapper()
{
    solver = new CaDiCaL::Solver;
}

CadicalWrapper::CadicalWrapper(CadicalWrapper& wrapper)
{
    solver = new CaDiCaL::Solver;
    wrapper.solver->copy(*solver);
}

CadicalWrapper::~CadicalWrapper()
{
    delete solver;
}

void CadicalWrapper::addClause(const std::vector<int>& literals)
{
    for (auto i = literals.begin(); i != literals.end(); ++i)
    {
        solver->add(*i);
    }
    solver->add(0);
}

E_STATE CadicalWrapper::solve(const std::vector<int>& assumptions)
{
    lastAssumptions.clear();
    for (auto i = assumptions.begin(); i != assumptions.end(); ++i)
    {
        solver->assume(*i);
        lastAssumptions.push_back(*i);
    }
    return (solver->solve() == 10 ? E_STATE::SAT : E_STATE::UNSAT);
}

unsigned int CadicalWrapper::getNumClauses()
{
    return solver->irredundant() + solver->redundant();;
}

unsigned int CadicalWrapper::getMaxVar()
{
    return solver->vars();
}

void CadicalWrapper::getModel(std::vector<int> &model)
{
    int maxvar = solver->vars();
    for(int i = 1; i <= maxvar; ++i)
    {
        int lit = solver->val(i) > 0 ? i : -i;
        model.push_back(lit);
    }
}

void CadicalWrapper::core(std::vector<int>& conflict)
{
    for (auto i = lastAssumptions.begin(); i != lastAssumptions.end(); ++i)
    {
        if(solver->failed(*i))
        {
            conflict.push_back(*i);
        }
    }
}

void CadicalWrapper::traceProof(const char *path)
{
    solver->trace_proof(path);
}


void CadicalWrapper::set(const char* key, int value)
{
    solver->set(key, value);
}

void CadicalWrapper::get(const char * key, Parameter & value)
{
    int v = solver->get(key);
    value.set(v);
}

void CadicalWrapper::interrupt()
{
    solver->terminate();
}

void CadicalWrapper::clearInterrupt() { }

iSAT* CadicalWrapper::clone() {
    return new CadicalWrapper(*this);
}


std::vector<OptilogConfigParameter> configurableParameters
{
    OptilogConfigParameter("arena", 0, 1, 1),
    OptilogConfigParameter("arenacompact", 0, 1, 1),
    OptilogConfigParameter("arenasort", 0, 1, 1),
    OptilogConfigParameter("arenatype", 1, 3, 3),
    OptilogConfigParameter("block", 0, 1, 0),
    OptilogConfigParameter("blockmaxclslim", 1, 1000000000, 1000),
    OptilogConfigParameter("blockminclslim", 2, 1000000000, 2),
    OptilogConfigParameter("blockocclim", 1, 1000000000, 100),
    OptilogConfigParameter("bump", 0, 1, 1),
    OptilogConfigParameter("bumpreason", 0, 1, 1),
    OptilogConfigParameter("bumpreasondepth", 1, 3, 1),
    OptilogConfigParameter("checkassumptions", 0, 1, 1),
    OptilogConfigParameter("checkfailed", 0, 1, 1),
    OptilogConfigParameter("checkfrozen", 0, 1, 0),
    OptilogConfigParameter("checkproof", 0, 1, 1),
    OptilogConfigParameter("checkwitness", 0, 1, 1),
    OptilogConfigParameter("chrono", 0, 2, 1),
    OptilogConfigParameter("chronoalways", 0, 1, 0),
    OptilogConfigParameter("chronolevelim", 0, 1000000000, 100),
    OptilogConfigParameter("chronoreusetrail", 0, 1, 1),
    OptilogConfigParameter("compact", 0, 1, 1),
    OptilogConfigParameter("compactint", 1, 1000000000, 2000),
    OptilogConfigParameter("compactlim", 0, 1000, 100),
    OptilogConfigParameter("compactmin", 1, 1000000000, 100),
    OptilogConfigParameter("cover", 0, 1, 0),
    OptilogConfigParameter("covermaxeff", 0, 1000000000, 100000000),
    OptilogConfigParameter("covermineff", 0, 1000000000, 1000000),
    OptilogConfigParameter("coverreleff", 0, 1000, 4),
    OptilogConfigParameter("decompose", 0, 1, 1),
    OptilogConfigParameter("decomposerounds", 1, 16, 2),
    OptilogConfigParameter("deduplicate", 0, 1, 1),
    OptilogConfigParameter("eagersubsume", 0, 1, 1),
    OptilogConfigParameter("eagersubsumelim", 1, 1000, 20),
    OptilogConfigParameter("elim", 0, 1, 1),
    OptilogConfigParameter("elimands", 0, 1, 1),
    OptilogConfigParameter("elimaxeff", 0, 1000000000, 1000000000),
    OptilogConfigParameter("elimbackward", 0, 1, 1),
    OptilogConfigParameter("elimboundmax", -1, 256, 16),
    OptilogConfigParameter("elimboundmin", -1, 1000, 0),
    OptilogConfigParameter("elimclslim", 2, 1000000000, 100),
    OptilogConfigParameter("elimequivs", 0, 1, 1),
    OptilogConfigParameter("elimineff", 0, 1000000000, 10000000),
    OptilogConfigParameter("elimint", 1, 1000000000, 2000),
    OptilogConfigParameter("elimites", 0, 1, 1),
    OptilogConfigParameter("elimlimited", 0, 1, 1),
    OptilogConfigParameter("elimocclim", 0, 1000000000, 1000),
    OptilogConfigParameter("elimprod", 0, 10000, 1),
    OptilogConfigParameter("elimreleff", 0, 10000, 1000),
    OptilogConfigParameter("elimrounds", 1, 512, 2),
    OptilogConfigParameter("elimsubst", 0, 1, 1),
    OptilogConfigParameter("elimxorlim", 2, 27, 5),
    OptilogConfigParameter("elimxors", 0, 1, 1),
    OptilogConfigParameter("emagluefast", 1, 1000000000, 33),
    OptilogConfigParameter("emaglueslow", 1, 1000000000, 100000),
    OptilogConfigParameter("emajump", 1, 1000000000, 100000),
    OptilogConfigParameter("emalevel", 1, 1000000000, 100000),
    OptilogConfigParameter("emasize", 1, 1000000000, 100000),
    OptilogConfigParameter("ematrailfast", 1, 1000000000, 100),
    OptilogConfigParameter("ematrailslow", 1, 1000000000, 100000),
    OptilogConfigParameter("flush", 0, 1, 1),
    OptilogConfigParameter("flushfactor", 1, 1000, 3),
    OptilogConfigParameter("flushint", 1, 1000000000, 100000),
    OptilogConfigParameter("forcephase", 0, 1, 0),
    OptilogConfigParameter("inprocessing", 0, 1, 1),
    OptilogConfigParameter("instantiate", 0, 1, 0),
    OptilogConfigParameter("instantiateclslim", 2, 1000000000, 3),
    OptilogConfigParameter("instantiateocclim", 1, 1000000000, 1),
    OptilogConfigParameter("instantiateonce", 0, 1, 1),
    OptilogConfigParameter("lucky", 0, 1, 1),
    OptilogConfigParameter("minimize", 0, 1, 1),
    OptilogConfigParameter("minimizedepth", 0, 1000, 1000),
    OptilogConfigParameter("phase", 0, 1, 1),
    OptilogConfigParameter("probe", 0, 1, 1),
    OptilogConfigParameter("probehbr", 0, 1, 1),
    OptilogConfigParameter("probeint", 1, 1000000000, 5000),
    OptilogConfigParameter("probemaxeff", 0, 1000000000, 100000000),
    OptilogConfigParameter("probemineff", 0, 1000000000, 1000000),
    OptilogConfigParameter("probereleff", 0, 1000, 20),
    OptilogConfigParameter("proberounds", 1, 16, 1),
    OptilogConfigParameter("profile", 0, 4, 2),
    OptilogConfigParameter("radixsortlim", 0, 1000000000, 800),
    OptilogConfigParameter("realtime", 0, 1, 0),
    OptilogConfigParameter("reduce", 0, 1, 1),
    OptilogConfigParameter("reduceint", 10, 1000000, 300),
    OptilogConfigParameter("reducekeepglue", 1, 1000000000, 3),
    OptilogConfigParameter("reducetarget", 10, 100, 75),
    OptilogConfigParameter("reluctant", 0, 1000000000, 1024),
    OptilogConfigParameter("reluctantmax", 0, 1000000000, 1048576),
    OptilogConfigParameter("rephase", 0, 1, 1),
    OptilogConfigParameter("rephaseint", 1, 1000000000, 1000),
    OptilogConfigParameter("report", 0, 1, 1),
    OptilogConfigParameter("reportall", 0, 1, 0),
    OptilogConfigParameter("reportsolve", 0, 1, 0),
    OptilogConfigParameter("restart", 0, 1, 1),
    OptilogConfigParameter("restartint", 1, 1000000000, 2),
    OptilogConfigParameter("restartmargin", 0, 100, 10),
    OptilogConfigParameter("restartreusetrail", 0, 1, 1),
    OptilogConfigParameter("restoreall", 0, 2, 0),
    OptilogConfigParameter("restoreflush", 0, 1, 0),
    OptilogConfigParameter("reverse", 0, 1, 0),
    OptilogConfigParameter("score", 0, 1, 1),
    OptilogConfigParameter("scorefactor", 500, 1000, 950),
    OptilogConfigParameter("shuffle", 0, 1, 0),
    OptilogConfigParameter("shufflequeue", 0, 1, 1),
    OptilogConfigParameter("shufflerandom", 0, 1, 0),
    OptilogConfigParameter("shufflescores", 0, 1, 1),
    OptilogConfigParameter("simplify", 0, 1, 1),
    OptilogConfigParameter("stabilize", 0, 1, 1),
    OptilogConfigParameter("stabilizefactor", 101, 1000000000, 200),
    OptilogConfigParameter("stabilizeint", 1, 1000000000, 1000),
    OptilogConfigParameter("stabilizemaxint", 1, 1000000000, 1000000000),
    OptilogConfigParameter("stabilizeonly", 0, 1, 0),
    OptilogConfigParameter("stabilizephase", 0, 1, 1),
    OptilogConfigParameter("subsume", 0, 1, 1),
    OptilogConfigParameter("subsumebinlim", 0, 1000000000, 10000),
    OptilogConfigParameter("subsumeclslim", 0, 1000000000, 1000),
    OptilogConfigParameter("subsumeint", 1, 1000000000, 10000),
    OptilogConfigParameter("subsumelimited", 0, 1, 1),
    OptilogConfigParameter("subsumemaxeff", 0, 1000000000, 100000000),
    OptilogConfigParameter("subsumemineff", 0, 1000000000, 1000000),
    OptilogConfigParameter("subsumeocclim", 0, 1000000000, 100),
    OptilogConfigParameter("subsumereleff", 0, 10000, 1000),
    OptilogConfigParameter("subsumestr", 0, 1, 1),
    OptilogConfigParameter("ternary", 0, 1, 1),
    OptilogConfigParameter("ternarymaxadd", 0, 10000, 1000),
    OptilogConfigParameter("ternarymaxeff", 0, 1000000000, 100000000),
    OptilogConfigParameter("ternarymineff", 1, 1000000000, 1000000),
    OptilogConfigParameter("ternaryocclim", 1, 1000000000, 100),
    OptilogConfigParameter("ternaryreleff", 0, 100000, 10),
    OptilogConfigParameter("ternaryrounds", 1, 16, 2),
    OptilogConfigParameter("transred", 0, 1, 1),
    OptilogConfigParameter("transredmaxeff", 0, 1000000000, 100000000),
    OptilogConfigParameter("transredmineff", 0, 1000000000, 1000000),
    OptilogConfigParameter("transredreleff", 0, 1000, 100),
    OptilogConfigParameter("vivify", 0, 1, 1),
    OptilogConfigParameter("vivifymaxeff", 0, 1000000000, 100000000),
    OptilogConfigParameter("vivifymineff", 0, 1000000000, 100000),
    OptilogConfigParameter("vivifyonce", 0, 2, 0),
    OptilogConfigParameter("vivifyredeff", 0, 1000, 300),
    OptilogConfigParameter("vivifyreleff", 0, 1000, 80),
    OptilogConfigParameter("walk", 0, 1, 1),
    OptilogConfigParameter("walkmaxeff", 0, 1000000000, 10000000),
    OptilogConfigParameter("walkmineff", 0, 10000000, 100000),
    OptilogConfigParameter("walknonstable", 0, 1, 1),
    OptilogConfigParameter("walkredundant", 0, 1, 0),
    OptilogConfigParameter("walkreleff", 0, 1000, 20)
};

OPTILOG_C_INTERFACE(CadicalWrapper, "Cadical")
OPTILOG_CONFIG_C_INTERFACE(configurableParameters)