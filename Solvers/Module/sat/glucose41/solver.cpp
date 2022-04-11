#include "solver.hpp"
#include "core/SolverTypes.h"
#include "mtl/Vec.h"
#include "optilogcinterface.hpp"

Glucose41Wrapper::Glucose41Wrapper() : Solver::Solver() 
{
    createMaps();
}

Glucose41Wrapper::Glucose41Wrapper(Glucose41Wrapper& wrapper) : Solver::Solver(wrapper) 
{
    createMaps();
}

Glucose41Wrapper::~Glucose41Wrapper() {}

int Glucose41Wrapper::newVar()
{
    return Solver::newVar() + 1;
}

void Glucose41Wrapper::addClause(const std::vector<int>& literals)
{
    Glucose41::vec<Glucose41::Lit> clauses;
    int max_var = createVectorFromIntToLit(literals, clauses);
    createVars(max_var);
    Solver::addClause(clauses);
}

Glucose41::lbool Glucose41Wrapper::solveLimitedFullAssignment(const Glucose41::vec<Glucose41::Lit>& assupmtLit)
{
    Glucose41::lbool res = solveLimited(assupmtLit);
    if (res == g41l_True) {
        res = isFullModel() ? g41l_True : g41l_Undef;
    }
    return res;
}

E_STATE Glucose41Wrapper::solve(const std::vector<int>& assumptions)
{
    Glucose41::vec<Glucose41::Lit> assupmtLit;
    int max_var = createVectorFromIntToLit(assumptions, assupmtLit);

    createVars(max_var);
    Solver::budgetOff();

    Glucose41::lbool res = solveLimitedFullAssignment(assupmtLit);
    
    return boolToState(res);
}

unsigned int Glucose41Wrapper::getNumClauses()
{
    return Solver::nClauses();
}

unsigned int Glucose41Wrapper::getMaxVar()
{
    return Solver::nVars();
}

void Glucose41Wrapper::getModel(std::vector<int> &vectModel)
{
    Glucose41::lbool False = Glucose41::lbool((uint8_t)1);
    Glucose41::lbool Undef = Glucose41::lbool((uint8_t)2);

    for(int i = 0; i < Solver::model.size(); ++i) {
        if(Solver::model[i] != Undef)
        {
            vectModel.push_back(model[i] == False ? -(i+1) : (i+1));
        }
    }
}

bool Glucose41Wrapper::propagate(const std::vector<int> &assum,
                                 std::vector<int> &prop,
                                 int phases)
{
    Glucose41::vec<Glucose41::Lit> assumptions;
    int max_var = createVectorFromIntToLit(assum, assumptions);

    createVars(max_var);

    Glucose41::vec<Glucose41::Lit> propagated;
    bool res = Solver::prop_check(assumptions, propagated, phases);

    createVectorFromLitToInt(propagated, prop, getVariableOpositePolarity);

    return res;
}

void Glucose41Wrapper::setConflictsBudget(long long budget)
{
    if (budget != 0 && budget != -1) { // it is 0 by default
        Solver::setConfBudget(budget);
    } else {
        Solver::budgetOff();
    }
}

void Glucose41Wrapper::setPropagationBudget(long long budget)
{
    if (budget != 0 && budget != -1) { // it is 0 by default
        Solver::setPropBudget(budget);
    } else {
        Solver::budgetOff();
    }
}

void Glucose41Wrapper::setIncremental()
{
    Solver::setIncrementalMode();
}

E_STATE Glucose41Wrapper::solveLimit(const std::vector<int>& assumptions)
{
    Glucose41::vec<Glucose41::Lit> assupmtLit;
    int max_var = createVectorFromIntToLit(assumptions, assupmtLit);

    createVars(max_var);

    Glucose41::lbool res = solveLimitedFullAssignment(assupmtLit);
    return boolToState(res);
}

void Glucose41Wrapper::core(std::vector<int>& vectConflict)
{
    createVectorFromLitToInt(conflict, vectConflict, getVariable);
}

E_STATE Glucose41Wrapper::getPolarity(int var)
{
    char forced_pol = forcedPolarity[var - 1];
    if (forced_pol == 0){
        return E_STATE::UNKNOWN;
    } else if (forced_pol != 1) {
        //Glucoses uses inverted polarity internally, so we convert here
        return E_STATE::SAT;
    }
    return E_STATE::UNSAT;
}

void Glucose41Wrapper::setVarPolarity(int var, bool polarity)
{
    char pi = 0;
    //Glucoses uses inverted polarity internally, so we convert here
    if (polarity){
        pi = -1;
    } else {
        pi = 1;
    }
    Solver::setForcedPolarity(var - 1, pi);
}

void Glucose41Wrapper::unsetVarPolarity(int var)
{
    Solver::setForcedPolarity(var - 1, 0);
}

void Glucose41Wrapper::traceProof(const char *path)
{
    Solver::certifiedOutput = fopen(path, "w+");

	if (Solver::certifiedOutput == 0) {
		throw FileException(path);
	}

	setlinebuf(certifiedOutput);
    Solver::certifiedUNSAT = true;
    Solver::certifiedPyFile = certifiedOutput;
}

void Glucose41Wrapper::interrupt()
{
    Solver::interrupt();
}

void Glucose41Wrapper::clearInterrupt()
{
    Solver::clearInterrupt();
}

E_STATE Glucose41Wrapper::getValue(int variable)
{
    int index_var = variable - 1;
    bool modelExists = Solver::model.size() != 0;
    Glucose41::lbool assigned = value(index_var);
    Glucose41::lbool in_model = g41l_Undef;
    if(modelExists){
        in_model = Solver::model[index_var];
    }

    if (assigned != g41l_Undef && modelExists && assigned != in_model){
        printf("Unexpected Error: Broken Internal Invariant in getValue\n");
        exit(-1);
    }
    
    if (assigned != g41l_Undef){
        return boolToState(assigned);
    }

    return boolToState(in_model);
}

void Glucose41Wrapper::setDecisionVariable(int var, bool b)
{
    Solver::setDecisionVar(var - 1, b);
}

bool Glucose41Wrapper::isFullModel()
{
    if(Solver::model.size() == 0) return false;

    Glucose41::lbool Undef = Glucose41::lbool((uint8_t)2);

    for(int i = 0; i < model.size(); ++i) {
        if(Solver::model[i] == Undef)
        {
            return false;
        }
    }
    return true;
}

long Glucose41Wrapper::getNumConflicts()
{
    return conflicts;
}

void Glucose41Wrapper::getLearntClauses(std::vector<std::vector<int>> &learntClauses)
{
    int variable;
    learntClauses.reserve(learnts.size());
    for(unsigned int i = 0; i < learnts.size(); i++)
    {
        const Glucose41::Clause &cl = ca[learnts[i]];
        std::vector<int> newClause;
        
        newClause.reserve(cl.size());
        for(unsigned int j = 0; j < cl.size(); j++)
        {
           newClause.push_back(getVariable(cl[j]));
        }
        learntClauses.push_back(newClause);
    }
}

E_STATE Glucose41Wrapper::solveHardLimited(const std::vector<int>& assumptions)
{
    bool currentLubyRestart = luby_restart;
    int currentRestartFactor = luby_restart_factor;
    bool currDisableSimplify = disable_simplify;
    luby_restart = true;
    luby_restart_factor = 1;
    disable_simplify = true;

    Glucose41::vec<Glucose41::Lit> assupmtLit;
    int max_var = createVectorFromIntToLit(assumptions, assupmtLit);

    createVars(max_var);

    Glucose41::lbool res = solveLimitedFullAssignment(assupmtLit);

    luby_restart = currentLubyRestart;
    luby_restart_factor = currentRestartFactor;
    disable_simplify = currDisableSimplify;

    return boolToState(res);
}

E_STATE Glucose41Wrapper::boolToState(Glucose41::lbool b){
    if(b == g41l_True) {
        return E_STATE::SAT;
    } else if(b == g41l_False) {
        return E_STATE::UNSAT;
    }
    return E_STATE::UNKNOWN;
}

void Glucose41Wrapper::setStaticHeuristic(std::vector<int> * heuristic)
{
    Glucose41::vec<Glucose41::Lit> * lits = new Glucose41::vec<Glucose41::Lit>();
    createVectorFromIntToLit(*heuristic, *lits);
    delete heuristic;
    Glucose41::Solver::setStaticHeuristic(lits);
}

void Glucose41Wrapper::enableStaticHeuristic(bool value)
{
    Glucose41::Solver::enableStaticHeuristic(value);
}

/*============================================================================*/
/*                            PRIVATE METHODS                                 */
/*============================================================================*/

inline void Glucose41Wrapper::createVars(int max_var)
{
    while (max_var >= Solver::nVars()) Solver::newVar();
}

int Glucose41Wrapper::getVariable(const Glucose41::Lit variable)
{
    return (Glucose41::var(variable) + 1)
         * ( (Glucose41::sign(variable)) ?  1 : -1 );
}

int Glucose41Wrapper::getVariableOpositePolarity(const Glucose41::Lit variable)
{
    return (Glucose41::var(variable) + 1)
         * ( (Glucose41::sign(variable)) ?  -1 : 1 );
}

int Glucose41Wrapper::createVectorFromIntToLit(
        const std::vector<int> &ints,
        Glucose41::vec<Glucose41::Lit> &lits)
{
    int var, max_var = -1;

    lits.capacity(ints.size());
    for (unsigned int  i = 0; i < ints.size(); ++i)
    {
        var = abs(ints[i])-1;
        if(var > max_var)
          max_var = var;

        lits.push( (ints[i] > 0) ? Glucose41::mkLit(var) :
                                  ~Glucose41::mkLit(var) );
    }
    return max_var;
}

void Glucose41Wrapper::createVectorFromLitToInt(
        const Glucose41::vec<Glucose41::Lit> &lits,
        std::vector<int> &ints, 
        callbackFunction getVariablePrt)
{
    ints.reserve(lits.size());
    for(int i = 0; i < lits.size(); ++i)
    {
        ints.push_back( getVariablePrt(lits[i]) );
    }
}

void Glucose41Wrapper::getActivities(std::vector<double>& activities)
{
    activities.reserve(activity.size());
    for(int i = 0; i < activity.size(); i++){
        activities.push_back(activity[i]);
    }
}

iSAT* Glucose41Wrapper::clone()
{
    return new Glucose41Wrapper(*this);
}

void Glucose41Wrapper::set(const char * key, int value)
{
    if(setParameterValue<int>(key, value, integerParams)) return;
    if(setParameterValue<uint32_t>(key, value, uintegerParams)) return;
    if(setParameterValue<double>(key, value, doubleParams)) return;
    if(setParameterValue<bool>(key, (value != 0), booleanParams)) return;

    throw ParameterNotFoundException(key);
}

void Glucose41Wrapper::set(const char * key, double value)
{
    if(setParameterValue<double>(key, value, doubleParams)) return;
    if(setParameterValue<int>(key, value, integerParams)) return;
    if(setParameterValue<uint32_t>(key, value, uintegerParams)) return;
    if(setParameterValue<bool>(key, (value != 0), booleanParams)) return;

    throw ParameterNotFoundException(key);
}

void Glucose41Wrapper::set(const char * key, bool value)
{
    if(setParameterValue<bool>(key, value, booleanParams)) return;
    throw ParameterNotFoundException(key);
}

void Glucose41Wrapper::get(const char * key, Parameter & value)
{
    if(getParameterValue<double *>(key, value, doubleParams)) return;
    if(getParameterValue<int *>(key, value, integerParams)) return;
    if(getParameterValue<uint32_t *>(key, value, uintegerParams)) return;
    if(getParameterValue<bool *>(key, value, booleanParams)) return;

    throw ParameterNotFoundException(key);
}

template<typename T> 
bool Glucose41Wrapper::setParameterValue(const char * key, T value, 
                        std::unordered_map<std::string, T*> & paramsMap)
{
    auto it = paramsMap.find(std::string(key));
    if(it != paramsMap.end())
    {
        *(it->second) = value;
        return true;
    }
    return false;
}

template<typename T>
bool Glucose41Wrapper::getParameterValue(const char * key, Parameter & value, 
                        std::unordered_map<std::string, T> & paramsMap)
{
    auto it = paramsMap.find(std::string(key));
    if(it != paramsMap.end())
    {
        value.set(*(it->second));
        return true;
    }
    return false;
}

void Glucose41Wrapper::createMaps() 
{
    // doubles
    doubleParams["seed"] = &random_seed;
    doubleParams["gc-frac"] = &garbage_frac;
    doubleParams["rnd-freq"] = &random_var_freq;
    doubleParams["rnd-pol-freq"] = &random_pol_freq;
    doubleParams["cla-decay"] = &clause_decay;
    doubleParams["max-var-decay"] = &max_var_decay;
    doubleParams["var-decay"] = &var_decay;
    doubleParams["K"] = &K;
    doubleParams["R"] = &R;
    doubleParams["szTrailQueue"] = &sizeTrailQueue;
    doubleParams["szLBDQueue"] = &sizeLBDQueue;

    // integers
    integerParams["phase-saving"] = &phase_saving;
    integerParams["ccmin-mode"] = &ccmin_mode;
    integerParams["minSizeMinimizingClause"] = &lbSizeMinimizingClause;
    integerParams["specialIncReduceDB"] = &specialIncReduceDB;
    integerParams["co"] = &coLBDBound;
    integerParams["firstReduceDB"] = &firstReduceDB;
    integerParams["incReduceDB"] = &incReduceDB;

    // uint32_t
    uintegerParams["rinc"] = &restart_inc;
    uintegerParams["minLBDMinimizingClause"] = &lbLBDMinimizingClause;
    uintegerParams["minLBDFrozenClause"] = &lbLBDFrozenClause;
    uintegerParams["luby-factor"] = &luby_restart_factor;

    // boolean
    booleanParams["phase-restart"] = &randomize_on_restarts;
    booleanParams["forceunsat"] = &forceUnsatOnNewDescent;
    booleanParams["adapt"] = &adaptStrategies;
    booleanParams["fix-phas-rest"] = &fixed_randomize_on_restarts;
    booleanParams["luby"] = &luby_restart;
    booleanParams["gr"] = &glureduce;   // glucose strategy to fire clause database reduction (must be false to fire Chanseok strategy).
    booleanParams["rnd-init"] = &rnd_init_act;
    booleanParams["chanseok"] = &chanseokStrategy;

    booleanParams["enforcePolarity"] = &enforcePolarity;
}

std::vector<OptilogConfigParameter> configurableParameters {
    OptilogConfigParameter("forceunsat", true),
    OptilogConfigParameter("adapt", true),
    OptilogConfigParameter("fix-phas-rest", false),
    OptilogConfigParameter("luby", false),
    OptilogConfigParameter("gr", true),
    OptilogConfigParameter("rnd-init", false),
    OptilogConfigParameter("gc-frac", 0.001, 3.4e+38, 0.2),
    OptilogConfigParameter("rinc", 2, 2147483647, 2),
    OptilogConfigParameter("rnd-freq", 0, 1, 0),
    OptilogConfigParameter("cla-decay", 0.001, 0.999, 0.999),
    OptilogConfigParameter("max-var-decay", 0.001, 0.999, 0.95),
    OptilogConfigParameter("var-decay", 0.001, 0.999, 0.8),
    OptilogConfigParameter("phase-restart", false),
    OptilogConfigParameter("phase-saving", 0, 2, 2),
    OptilogConfigParameter("ccmin-mode", 0, 2, 2),
    OptilogConfigParameter("minLBDMinimizingClause", 3, 2147483647, 6),
    OptilogConfigParameter("minSizeMinimizingClause", 3, 2147483647, 30),
    OptilogConfigParameter("chanseok", false),
    OptilogConfigParameter("specialIncReduceDB", 0, 2147483647, 1000),
    OptilogConfigParameter("minLBDFrozenClause", 0, 2147483647, 30),
    OptilogConfigParameter("luby-factor", 1, 2147483647, 100),
    OptilogConfigParameter("co", 2, 2147483647, 5),
    OptilogConfigParameter("firstReduceDB", 0, 2147483647, 2000),
    OptilogConfigParameter("incReduceDB", 0, 2147483647, 300),
    OptilogConfigParameter("R", 1.001, 4.999, 1.4),
    OptilogConfigParameter("K", 0.001, 0.999, 0.8),
    OptilogConfigParameter("szTrailQueue", 10, 2147483647, 5000),
    OptilogConfigParameter("szLBDQueue", 10, 2147483647, 50),
};

OPTILOG_C_INTERFACE(Glucose41Wrapper, "Glucose41")
OPTILOG_CONFIG_C_INTERFACE(configurableParameters)