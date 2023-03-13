#ifndef CSXD_TEAMISFULLEXCEPTION_H
#define CSXD_TEAMISFULLEXCEPTION_H


#include <exception>

using namespace std;

class TeamIsFullException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_TEAMISFULLEXCEPTION_H
