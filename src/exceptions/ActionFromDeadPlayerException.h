#ifndef CSXD_ACTIONFROMDEADPLAYEREXCEPTION_H
#define CSXD_ACTIONFROMDEADPLAYEREXCEPTION_H


#include <exception>

using namespace std;

class ActionFromDeadPlayerException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_ACTIONFROMDEADPLAYEREXCEPTION_H
