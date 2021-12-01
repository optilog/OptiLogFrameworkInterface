#ifndef _PY_EXCEPTIONS_HPP_
#define _PY_EXCEPTIONS_HPP_

#include <exception>
#include <sstream>

class OptilogException : public std::exception
{
  public:
    virtual const char* what() const throw() = 0;
};

class NotImplementedException : public OptilogException
{
  public:
    NotImplementedException(const char* message);
    const char* what() const throw() override;
  private:
    std::string msg;
};

class PythonAPIException : public OptilogException
{
  public:
    PythonAPIException();
    PythonAPIException(const char* message);
    const char* what() const throw() override;
  private:
    std::string msg;
};

class ParsingException : public OptilogException
{
  public:
    ParsingException();
    ParsingException(char ch);
    const char* what() const throw() override;
  private:
    std::string msg;
};

class FileException : public OptilogException
{
  public:
    FileException(const char * path);
    const char* what() const throw() override;
  private:
    std::string msg;
};

class IllegalSolverState : public OptilogException
{
  public:
    const char* what() const throw() override;
};

class ParameterNotFoundException : public OptilogException
{
  public:
    ParameterNotFoundException(const char * key);
    const char* what() const throw() override;

  private:
    std::string msg;
};

#endif