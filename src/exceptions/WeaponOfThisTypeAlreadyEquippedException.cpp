#include "WeaponOfThisTypeAlreadyEquippedException.h"

const char *WeaponOfThisTypeAlreadyEquippedException::what() const noexcept {
    return "a weapon of this type is already equipped";
}
