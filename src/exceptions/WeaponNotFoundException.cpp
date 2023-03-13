#include "WeaponNotFoundException.h"

const char *WeaponNotFoundException::what() const noexcept {
    return "this weapon was not found";
}
