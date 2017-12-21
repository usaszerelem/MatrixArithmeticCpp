#pragma once 

#include <exception> 
#include <string> 

using namespace std;

class CAppException : public exception
{
public:
    inline CAppException(const char * pszMsg) { m_strMsg = pszMsg; }
    inline CAppException(const string & strMsg) { m_strMsg = strMsg; }
    inline ~CAppException() {};

    inline virtual const char* what() const throw() { return(m_strMsg.c_str()); }

private:
    string  m_strMsg;
};