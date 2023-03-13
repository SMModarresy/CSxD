#ifndef CSXD_FRIENDLYFIREEXCEPTION_H
#define CSXD_FRIENDLYFIREEXCEPTION_H


#include <exception>

using namespace std;

class FriendlyFireException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_FRIENDLYFIREEXCEPTION_H
