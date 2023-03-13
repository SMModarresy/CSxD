#ifndef CSXD_PLAYERINOPPONENTTEAMEXCEPTION_H
#define CSXD_PLAYERINOPPONENTTEAMEXCEPTION_H


#include <exception>

using namespace std;

class PlayerInOpponentTeamException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_PLAYERINOPPONENTTEAMEXCEPTION_H
