#include <stdexcept>

class GenericException : public std::runtime_error 
{
public:

    GenericException(std::string msg)
        : runtime_error(msg)
    {
        msg += " :(";
    }
};