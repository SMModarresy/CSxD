#ifndef CSXD_WEAPONNOTAVAILABLEEXCEPTION_H
#define CSXD_WEAPONNOTAVAILABLEEXCEPTION_H


#include <exception>

using namespace std;

class WeaponNotAvailableException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_WEAPONNOTAVAILABLEEXCEPTION_H
