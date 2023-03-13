#include "PlayerNotFoundException.h"

const char *PlayerNotFoundException::what() const noexcept {
    return "this player was not found";
}
