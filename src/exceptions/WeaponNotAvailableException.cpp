#include "WeaponNotAvailableException.h"

const char *WeaponNotAvailableException::what() const noexcept {
    return "this weapon is not available";
}
