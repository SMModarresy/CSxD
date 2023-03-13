#include "FriendlyFireException.h"

const char *FriendlyFireException::what() const noexcept {
    return "friendly fire";
}
