#ifndef CSXD_ACTIONATILLEGALTIMEEXCEPTION_H
#define CSXD_ACTIONATILLEGALTIMEEXCEPTION_H


#include <exception>

using namespace std;

class ActionAtIllegalTimeException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_ACTIONATILLEGALTIMEEXCEPTION_H
