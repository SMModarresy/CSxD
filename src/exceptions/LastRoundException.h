#ifndef CSXD_LASTROUNDEXCEPTION_H
#define CSXD_LASTROUNDEXCEPTION_H


#include <exception>

using namespace std;

class LastRoundException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_LASTROUNDEXCEPTION_H
