#ifndef CSXD_ATTACKDEADPLAYEREXCEPTION_H
#define CSXD_ATTACKDEADPLAYEREXCEPTION_H


#include <exception>

using namespace std;

class AttackDeadPlayerException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_ATTACKDEADPLAYEREXCEPTION_H
