#ifndef CSXD_NOTENOUGHMONEYEXCEPTION_H
#define CSXD_NOTENOUGHMONEYEXCEPTION_H


#include <exception>

using namespace std;

class NotEnoughMoneyException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_NOTENOUGHMONEYEXCEPTION_H
