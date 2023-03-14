#include "WeaponNotEquippedException.h"

const char* WeaponNotEquippedException::what() const noexcept {
    return "player is not equipped with this weapon";
}
