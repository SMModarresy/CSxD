#include "LastRoundException.h"

const char* LastRoundException::what() const noexcept {
    return "this was the last round of the game";
}
