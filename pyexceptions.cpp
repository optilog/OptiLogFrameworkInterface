#include <sstream>
#include "pyexceptions.hpp"

NotImplementedException::NotImplementedException(const char* message){
    std::ostringstream stringStream;
    stringStream << "Not implemented exception: " << message;
    msg = stringStream.str();
}

const char* NotImplementedException::what() const throw()
{
    return msg.c_str();
}

PythonAPIException::PythonAPIException(){
    msg = "Unexpected Python API Error";
}

PythonAPIException::PythonAPIException(const char* message){
    std::ostringstream stringStream;
    stringStream << "Unexpected Python API Error: " << message;
    msg = stringStream.str();
}

const char* PythonAPIException::what() const throw()
{
    return msg.c_str();
}

ParsingException::ParsingException()
{
    msg = "Parsing Error";
}

ParsingException::ParsingException(char ch)
{
    std::ostringstream stringStream;
    stringStream << "Parsing Error. Unexpected character: " << ch;
    msg = stringStream.str();
}

const char* ParsingException::what() const throw()
{
    return msg.c_str();
}

FileException::FileException(const char * path)
{
    std::ostringstream stringStream;
    stringStream << "Could not open file: " << path;
    msg = stringStream.str();
}

const char* FileException::what() const throw()
{
    return msg.c_str();
}

const char* IllegalSolverState::what() const throw()
{
    return "Illegal method call on solver on this state";
}

ParameterNotFoundException::ParameterNotFoundException(const char * key)
{
    std::ostringstream stringStream;
    stringStream << "Key " << key << " not found in parameters list.";
    msg = stringStream.str();
}

const char* ParameterNotFoundException::what() const throw()
{
    return msg.c_str();
}
