#include "solver.hpp"
#include "optilogcinterface.hpp"


CadicalWrapper152::CadicalWrapper152()
{
    solver = new CaDiCaL::Solver;
}

CadicalWrapper152::CadicalWrapper152(CadicalWrapper152& wrapper)
{
    solver = new CaDiCaL::Solver;
    wrapper.solver->copy(*solver);
}

CadicalWrapper152::~CadicalWrapper152()
{
    delete solver;
}

void CadicalWrapper152::addClause(const std::vector<int>& literals)
{
    for (auto i = literals.begin(); i != literals.end(); ++i)
    {
        solver->add(*i);
    }
    solver->add(0);
}

E_STATE CadicalWrapper152::solve(const std::vector<int>& assumptions)
{
    lastAssumptions.clear();
    for (auto i = assumptions.begin(); i != assumptions.end(); ++i)
    {
        solver->assume(*i);
        lastAssumptions.push_back(*i);
    }
    return (solver->solve() == 10 ? E_STATE::SAT : E_STATE::UNSAT);
}

unsigned int CadicalWrapper152::getNumClauses()
{
    return solver->irredundant() + solver->redundant();;
}

unsigned int CadicalWrapper152::getMaxVar()
{
    return solver->vars();
}

void CadicalWrapper152::getModel(std::vector<int> &model)
{
    int maxvar = solver->vars();
    for(int i = 1; i <= maxvar; ++i)
    {
        int lit = solver->val(i) > 0 ? i : -i;
        model.push_back(lit);
    }
}

void CadicalWrapper152::core(std::vector<int>& conflict)
{
    for (auto i = lastAssumptions.begin(); i != lastAssumptions.end(); ++i)
    {
        if(solver->failed(*i))
        {
            conflict.push_back(*i);
        }
    }
}

void CadicalWrapper152::traceProof(const char *path)
{
    solver->trace_proof(path);
}


void CadicalWrapper152::set(const char* key, int value)
{
    solver->set(key, value);
}

void CadicalWrapper152::get(const char * key, Parameter & value)
{
    int v = solver->get(key);
    value.set(v);
}

void CadicalWrapper152::interrupt()
{
    solver->terminate();
}

void CadicalWrapper152::clearInterrupt() { }

iSAT* CadicalWrapper152::clone() {
    return new CadicalWrapper152(*this);
}

std::vector<OptilogConfigParameter> configurableParameters
{
    OptilogConfigParameter("arena",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("arenacompact",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("arenasort",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("arenatype",(int)  1,(int)  3,(int)  3),
    OptilogConfigParameter("binary",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("block",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("blockmaxclslim",(int)  1,(int) 2e9,(int)  1e5),
    OptilogConfigParameter("blockminclslim",(int)  2,(int) 2e9,(int)  2),
    OptilogConfigParameter("blockocclim",(int)  1,(int) 2e9,(int)  1e2),
    OptilogConfigParameter("bump",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("bumpreason",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("bumpreasondepth",(int)  1,(int)  3,(int)  1),
    OptilogConfigParameter("check",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("checkassumptions",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("checkconstraint",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("checkfailed",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("checkfrozen",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("checkproof",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("checkwitness",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("chrono",(int)  0,(int)  2,(int)  1),
    OptilogConfigParameter("chronoalways",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("chronolevelim",(int)  0,(int) 2e9,(int)  1e2),
    OptilogConfigParameter("chronoreusetrail",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("compact",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("compactint",(int)  1,(int) 2e9,(int)  2e3),
    OptilogConfigParameter("compactlim",(int)  0,(int) 1e3,(int)  1e2),
    OptilogConfigParameter("compactmin",(int)  1,(int) 2e9,(int)  1e2),
    OptilogConfigParameter("condition",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("conditionint",(int)  1,(int) 2e9,(int)  1e4),
    OptilogConfigParameter("conditionmaxeff",(int)  0,(int) 2e9,(int)  1e7),
    OptilogConfigParameter("conditionmaxrat",(int)  1,(int) 2e9,(int)  100),
    OptilogConfigParameter("conditionmineff",(int)  0,(int) 2e9,(int)  1e6),
    OptilogConfigParameter("conditionreleff",(int)  1,(int) 1e5,(int)  100),
    OptilogConfigParameter("cover",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("covermaxclslim",(int)  1,(int) 2e9,(int)  1e5),
    OptilogConfigParameter("covermaxeff",(int)  0,(int) 2e9,(int)  1e8),
    OptilogConfigParameter("coverminclslim",(int)  2,(int) 2e9,(int)  2),
    OptilogConfigParameter("covermineff",(int)  0,(int) 2e9,(int)  1e6),
    OptilogConfigParameter("coverreleff",(int)  1,(int) 1e5,(int)  4),
    OptilogConfigParameter("decompose",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("decomposerounds",(int)  1,(int)  16,(int)  2),
    OptilogConfigParameter("deduplicate",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("eagersubsume",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("eagersubsumelim",(int)  1,(int) 1e3,(int)  20),
    OptilogConfigParameter("elim",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("elimands",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("elimaxeff",(int)  0,(int) 2e9,(int)  2e9),
    OptilogConfigParameter("elimbackward",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("elimboundmax",(int)  -1,(int) 2e6,(int)  16),
    OptilogConfigParameter("elimboundmin",(int)  -1,(int) 2e6,(int)  0),
    OptilogConfigParameter("elimclslim",(int)  2,(int) 2e9,(int)  1e2),
    OptilogConfigParameter("elimequivs",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("elimineff",(int)  0,(int) 2e9,(int)  1e7),
    OptilogConfigParameter("elimint",(int)  1,(int) 2e9,(int)  2e3),
    OptilogConfigParameter("elimites",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("elimlimited",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("elimocclim",(int)  0,(int) 2e9,(int)  1e2),
    OptilogConfigParameter("elimprod",(int)  0,(int) 1e4,(int)  1),
    OptilogConfigParameter("elimreleff",(int)  1,(int) 1e5,(int)  1e3),
    OptilogConfigParameter("elimrounds",(int)  1,(int) 512,(int)  2),
    OptilogConfigParameter("elimsubst",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("elimsum",(int)  0,(int) 1e4,(int)  1),
    OptilogConfigParameter("elimxorlim",(int)  2,(int)  27,(int)  5),
    OptilogConfigParameter("elimxors",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("emagluefast",(int)  1,(int) 2e9,(int)  33),
    OptilogConfigParameter("emaglueslow",(int)  1,(int) 2e9,(int)  1e5),
    OptilogConfigParameter("emajump",(int)  1,(int) 2e9,(int)  1e5),
    OptilogConfigParameter("emalevel",(int)  1,(int) 2e9,(int)  1e5),
    OptilogConfigParameter("emasize",(int)  1,(int) 2e9,(int)  1e5),
    OptilogConfigParameter("ematrailfast",(int)  1,(int) 2e9,(int)  1e2),
    OptilogConfigParameter("ematrailslow",(int)  1,(int) 2e9,(int)  1e5),
    OptilogConfigParameter("flush",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("flushfactor",(int)  1,(int) 1e3,(int)  3),
    OptilogConfigParameter("flushint",(int)  1,(int) 2e9,(int)  1e5),
    OptilogConfigParameter("forcephase",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("inprocessing",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("instantiate",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("instantiateclslim",(int)  2,(int) 2e9,(int)  3),
    OptilogConfigParameter("instantiateocclim",(int)  1,(int) 2e9,(int)  1),
    OptilogConfigParameter("instantiateonce",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("lucky",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("minimize",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("minimizedepth",(int)  0,(int) 1e3,(int)  1e3),
    OptilogConfigParameter("phase",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("probe",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("probehbr",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("probeint",(int)  1,(int) 2e9,(int)  5e3),
    OptilogConfigParameter("probemaxeff",(int)  0,(int) 2e9,(int)  1e8),
    OptilogConfigParameter("probemineff",(int)  0,(int) 2e9,(int)  1e6),
    OptilogConfigParameter("probereleff",(int)  1,(int) 1e5,(int)  20),
    OptilogConfigParameter("proberounds",(int)  1,(int)  16,(int)  1),
    OptilogConfigParameter("profile",(int)  0,(int)  4,(int)  2),
    OptilogConfigParameter("radixsortlim",(int)  0,(int) 2e9,(int)  800),
    OptilogConfigParameter("realtime",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("reduce",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("reduceint",(int)  10,(int) 1e6,(int)  300),
    OptilogConfigParameter("reducetarget",(int)  10,(int) 1e2,(int)  75),
    OptilogConfigParameter("reducetier1glue",(int)  1,(int) 2e9,(int)  2),
    OptilogConfigParameter("reducetier2glue",(int)  1,(int) 2e9,(int)  6),
    OptilogConfigParameter("reluctant",(int)  0,(int) 2e9,(int)  1024),
    OptilogConfigParameter("reluctantmax",(int)  0,(int) 2e9,(int) 1048576),
    OptilogConfigParameter("rephase",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("rephaseint",(int)  1,(int) 2e9,(int)  1e3),
    OptilogConfigParameter("report",(int)  0,(int)  1,(int) 1),
    OptilogConfigParameter("reportall",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("reportsolve",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("restart",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("restartint",(int)  1,(int) 2e9,(int)  2),
    OptilogConfigParameter("restartmargin",(int)  0,(int) 1e2,(int)  10),
    OptilogConfigParameter("restartreusetrail",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("restoreall",(int)  0,(int)  2,(int)  0),
    OptilogConfigParameter("restoreflush",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("reverse",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("score",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("scorefactor",(int) 500,(int) 1e3,(int)  950),
    OptilogConfigParameter("seed",(int)  0,(int) 2e9,(int)  0),
    OptilogConfigParameter("shrink",(int)  0,(int)  3,(int)  3),
    OptilogConfigParameter("shrinkreap",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("shuffle",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("shufflequeue",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("shufflerandom",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("shufflescores",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("stabilize",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("stabilizefactor",(int) 101,(int) 2e9,(int)  200),
    OptilogConfigParameter("stabilizeint",(int)  1,(int) 2e9,(int)  1e3),
    OptilogConfigParameter("stabilizemaxint",(int)  1,(int) 2e9,(int)  2e9),
    OptilogConfigParameter("stabilizeonly",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("subsume",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("subsumebinlim",(int)  0,(int) 2e9,(int)  1e4),
    OptilogConfigParameter("subsumeclslim",(int)  0,(int) 2e9,(int)  1e2),
    OptilogConfigParameter("subsumeint",(int)  1,(int) 2e9,(int)  1e4),
    OptilogConfigParameter("subsumelimited",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("subsumemaxeff",(int)  0,(int) 2e9,(int)  1e8),
    OptilogConfigParameter("subsumemineff",(int)  0,(int) 2e9,(int)  1e6),
    OptilogConfigParameter("subsumeocclim",(int)  0,(int) 2e9,(int)  1e2),
    OptilogConfigParameter("subsumereleff",(int)  1,(int) 1e5,(int)  1e3),
    OptilogConfigParameter("subsumestr",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("target",(int)  0,(int)  2,(int)  1),
    OptilogConfigParameter("terminateint",(int)  0,(int) 1e4,(int)  10),
    OptilogConfigParameter("ternary",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("ternarymaxadd",(int)  0,(int) 1e4,(int)  1e3),
    OptilogConfigParameter("ternarymaxeff",(int)  0,(int) 2e9,(int)  1e8),
    OptilogConfigParameter("ternarymineff",(int)  1,(int) 2e9,(int)  1e6),
    OptilogConfigParameter("ternaryocclim",(int)  1,(int) 2e9,(int)  1e2),
    OptilogConfigParameter("ternaryreleff",(int)  1,(int) 1e5,(int)  10),
    OptilogConfigParameter("ternaryrounds",(int)  1,(int)  16,(int)  2),
    OptilogConfigParameter("transred",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("transredmaxeff",(int)  0,(int) 2e9,(int)  1e8),
    OptilogConfigParameter("transredmineff",(int)  0,(int) 2e9,(int)  1e6),
    OptilogConfigParameter("transredreleff",(int)  1,(int) 1e5,(int)  1e2),
    OptilogConfigParameter("vivify",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("vivifymaxeff",(int)  0,(int) 2e9,(int)  2e7),
    OptilogConfigParameter("vivifymineff",(int)  0,(int) 2e9,(int)  2e4),
    OptilogConfigParameter("vivifyonce",(int)  0,(int)  2,(int)  0),
    OptilogConfigParameter("vivifyredeff",(int)  0,(int) 1e3,(int)  75),
    OptilogConfigParameter("vivifyreleff",(int)  1,(int) 1e5,(int)  20),
    OptilogConfigParameter("walk",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("walkmaxeff",(int)  0,(int) 2e9,(int)  1e7),
    OptilogConfigParameter("walkmineff",(int)  0,(int) 1e7,(int)  1e5),
    OptilogConfigParameter("walknonstable",(int)  0,(int)  1,(int)  1),
    OptilogConfigParameter("walkredundant",(int)  0,(int)  1,(int)  0),
    OptilogConfigParameter("walkreleff",(int)  1,(int) 1e5,(int)  20),
};

OPTILOG_C_INTERFACE(CadicalWrapper152, "Cadical152")
OPTILOG_CONFIG_C_INTERFACE(configurableParameters)