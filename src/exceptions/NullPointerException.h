#ifndef CSXD_NULLPOINTEREXCEPTION_H
#define CSXD_NULLPOINTEREXCEPTION_H


#include <exception>
#include <string>
#include <utility>

using namespace std;

class NullPointerException : public exception {
public:
    explicit NullPointerException(string  variable_name);
    const char* what() const noexcept override;
protected:
    string variable_name;
};


#endif //CSXD_NULLPOINTEREXCEPTION_H
