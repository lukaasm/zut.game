#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED

#include <exception>
#include <string>

class Exception: public std::exception
{
    public:
        Exception(std::string & whatStr) : std::exception()
        {
            m_whatStr = whatStr;
        }

        Exception(const char * whatStr) : std::exception()
        {
            m_whatStr = whatStr;
        }

        virtual const char* what() const throw()
        {
            return m_whatStr.c_str();
        }

    private:
        std::string m_whatStr;
};


#endif // EXCEPTION_H_INCLUDED
