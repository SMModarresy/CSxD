#ifndef CSXD_PLAYERALREADYINTEAMEXCEPTION_H
#define CSXD_PLAYERALREADYINTEAMEXCEPTION_H


#include <exception>

using namespace std;

class PlayerAlreadyInTeamException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_PLAYERALREADYINTEAMEXCEPTION_H
