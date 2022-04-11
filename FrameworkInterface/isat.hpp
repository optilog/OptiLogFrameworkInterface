#ifndef _ISAT_HPP_
#define _ISAT_HPP_

#include <vector>
#include "pyexceptions.hpp"
#include "parameter.hpp"
#include "configparameter.hpp"

#define CONFIG_NAME ""

enum E_STATE { UNSAT=0, SAT=1, UNKNOWN=2 };

class iSAT {

public:
  /*!
  Creates a new variable and returns its DIMACS id.
  \return The returned value will be `iSAT.getMaxVar() + 1`.
  */
  virtual int newVar();
  /*!
  Adds a new clause to this solver.
  \param literals The clause to be added to the solver.
  */
  virtual void addClause(const std::vector<int>& literals);
  /*!
  \return Returns the maximum DIMACS variable id in the solver.
  */
  virtual unsigned int getMaxVar();
  /*!
  \return Returns the number of clauses within the solver.
  */
  virtual unsigned int getNumClauses();
  /*!
  Sets the maximum number of conflicts allowed by the budget, used by the methods \ref solveLimited or \ref solveHardLimited.
  A \ref solve call will reset all budgets.

  \param budget: the budget for the number of conflicts.
  */
  virtual void setConflictsBudget(long long budget);
  /*!
  Sets the maximum number of propagations allowed by the budget, used by the methods \ref solveLimited or \ref solveHardLimited.
  A \ref solve call will reset all budgets.

  \param budget: the budget for the number of propagations.
  */
  virtual void setPropagationBudget(long long budget);
  /*!
  \param assumptions: Vector of assumptions.
  \return Returns ``SAT`` if all variables are assigned and no contradiction is found.
    Returns ``UNSAT``, if the formula is proven to be unsatisfiable.
    Otherwise, returns ``UNKNOWN`` if all decision variables are assigned and no contradiction is found.
  */
  virtual E_STATE solve(const std::vector<int>& assumptions);
  /*!
  Applies unit propagation with an optional vector of assumptions.
  
  \param assumptions The vector of assumptions.
  \param propagated The vector used to store the propagations
  \param phases A limit on the number of `phases` of propagation. A value of zero sets no limit.
  \return Returns ``SAT`` if the solver finds no contradiction, and the vector of assigned literals. 
          Returns ``UNSAT`` if the solver finds a contradiction and an empty vector.
  */
  virtual bool propagate(const std::vector<int> &assumptions,
                         std::vector<int> &propagated,
                         int phases);
  /*!
  If the solver returns False, computes a subset of the assumptions that form a core of unsatisfiability.
  Otherwise must return an empty vector or an exception.

  \param conflict Vector used to store the core
  */
  virtual void core(std::vector<int>& conflict);
  /*!
  \return Returns a model for the decision variables that satisfies the formula.
  */
  virtual void getModel(std::vector<int> &model);
  /*!
  Stops the solving process if the budget is already exhausted at the beginning of the next restart.

  If no budget has been set, this call is equivalent to the \ref solve method.

  \param assumptions: Vector of assumptions.
  \return Returns the same as method \ref solve and additionally returns ``UNKNOWN`` if the budget is exhausted.
  */
  virtual E_STATE solveLimit(const std::vector<int>& assumptions);
  /*!
  Enables the `incremental` mode in solvers of the `Glucose` family.
  This mode enables changes in the LBD heuristic that may help improve performance in incremental sat solving.
  For more details see "Improving Glucose for Incremental SAT Solving with Assumptions: Application to MUS Extraction".
  */
  virtual void setIncremental();
  /*!
  Returns the preferred value for the given variable when the solver makes a decision.
  This value may be updated during the resolution process by phase saving policies.
  The default preferred value depends on the solver implementation.
  \param var The variable id
  \return The preferred polarity value for a variable
  */
  virtual E_STATE getPolarity(int var);
  /*!
  Disables the polarity set by the method \ref setPolarity.
  \param var The variable id
  */
  virtual void unsetVarPolarity(int var);
  /*!
  Forces the solver to set the given polarity when deciding the variable.
  */
  virtual void setVarPolarity(int var, bool polarity);
  /*!
  Dumps the proof of the resolution process to the specified ``path``.
  This method must be called before executing any of the ``solve`` like methods.
  If this method is called after any ``solve`` like call an exception will arise.

  \param path: The file path where the proof is going to be written.

  \b Warning:
    Solver interfaces are not homogeneous so this specification may not be entirely suitable.
  */
  virtual void traceProof(const char *path);
  /*!
  Interrupts the resolution process.
  This interruption will happen at the beginning of the next restart.
  */
  virtual void interrupt();
  /*!
  Clears an interruption.
  The interruption must be cleared before calling ``solve`` like methods.
  Not all the solvers force to clear the interruptions.
  */
  virtual void clearInterrupt();
  /*!
  \param variable The variable id.
  \return Returns the value assigned to the given variable.
    If the variable is unassigned, returns ``UNKNOWN`` (meaning undefined).
  */
  virtual E_STATE getValue(int variable);
  /*!
  \param var The variable to be set as decision variable.
  \param dec If dec is True, sets the variable as a decision variable. 
    If dec is False the variable is not a decision variable.
    By default all variables are decision variables.
  */
  virtual void setDecisionVariable(int var, bool b);
  /*!
  \return Returns True if model assigns all the variables.
  */
  virtual bool isFullModel();
  /*!
  Clears an interruption.
  The interruption must be cleared before calling ``solve`` like methods.
  Not all the solvers force to clear the interruptions.
  */
  virtual long getNumConflicts();
  /*!
  \return Returns the vector of learnt clauses, including the learnt unit clauses.
  */
  virtual void getLearntClauses(std::vector<std::vector<int>> &learntClauses);
  /*!
  Stops the solving process if the budget is already exhausted.

  If no budget has been set, this call is equivalent to the \ref solve method.

  \param assumptions Vector of assumptions.
  \return Returns the same as method \ref solve and additionally returns ``UNKNOWN`` if the budget is exhausted.
  */
  virtual E_STATE solveHardLimited(const std::vector<int>& assumptions);

  /*!
  
  Sets an static heuristic. The heuristic is described as a vector of pairs of variables and values.

  If all the variables in the heuristic have been decided, the solver applies the regular procedure.
  
  If an heuristic is active then its polarities take precedence over the ones assigned by the method \ref setPolarity.

  \b Warning:
    Notice the vector is passed by reference and becomes the callee responsibility to deallocate when a new heuristic is set.
    The vector is allocated with the keyword ``new``, therefore it *has to be* deallocated with the keyword ``delete``.
  \param heuristic A list of literals.
  */
  virtual void setStaticHeuristic(std::vector<int> * heuristic);
  /*!
  \param enable Enables or disables the static heuristic set by method \ref setStaticHeuristic
  */
  virtual void enableStaticHeuristic(bool enable);
  /*!
  \param activities Vector to store the activities of the variables in order
  */
  virtual void getActivities(std::vector<double>& activities);
  /*!
  Clones the solver, including the loaded clauses and internal state
  \param solver Solver to clone
  */
  virtual iSAT* clone();

  /*!
  Sets the value for a given parameter.
  \param key Parameter name.
  \param value Parameter value.
  */
  virtual void set(const char * key, int value);
  /*!
  Sets the value for a given parameter.
  \param key Parameter name.
  \param value Parameter value.
  */
  virtual void set(const char * key, double value);
  /*!
  Sets the value for a given parameter.
  \param key Parameter name.
  \param value Parameter value.
  */
  virtual void set(const char * key, bool value);
  /*!
  Sets the value for a given parameter.
  \param key Parameter name.
  \param value Parameter value.
  */
  virtual void set(const char * key, char * value);
  /*!
  Sets the value for a given parameter.
  \param key Parameter name.
  \param value Parameter value used to the return value.
  */
  virtual void get(const char * key, Parameter & value);
  /*!
  Returns the version of the interface.
  This number describes information of the available methods
  */
  virtual int getVersion();
};

/*
extern "C" iSAT* create_isat_solver();
extern "C" void destroy_isat_solver(iSAT* object);
extern "C" const char* name_isat_solver();
*/

#endif
