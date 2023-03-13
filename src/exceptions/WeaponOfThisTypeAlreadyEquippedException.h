#ifndef CSXD_WEAPONOFTHISTYPEALREADYEQUIPPEDEXCEPTION_H
#define CSXD_WEAPONOFTHISTYPEALREADYEQUIPPEDEXCEPTION_H


#include <exception>

using namespace std;

class WeaponOfThisTypeAlreadyEquippedException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_WEAPONOFTHISTYPEALREADYEQUIPPEDEXCEPTION_H
