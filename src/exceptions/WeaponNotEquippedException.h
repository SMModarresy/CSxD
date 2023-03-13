#ifndef CSXD_WEAPONNOTEQUIPPEDEXCEPTION_H
#define CSXD_WEAPONNOTEQUIPPEDEXCEPTION_H


#include <exception>

using namespace std;

class WeaponNotEquippedException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_WEAPONNOTEQUIPPEDEXCEPTION_H
