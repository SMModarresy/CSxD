#ifndef CSXD_WEAPONNOTFOUNDEXCEPTION_H
#define CSXD_WEAPONNOTFOUNDEXCEPTION_H


#include <exception>

using namespace std;

class WeaponNotFoundException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_WEAPONNOTFOUNDEXCEPTION_H
