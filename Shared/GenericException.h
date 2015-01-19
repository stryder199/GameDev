#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

class GenericException : public runtime_error 
{
public:

    GenericException(string msg)
        : runtime_error(msg)
    {
        msg += " :(";
    }
};