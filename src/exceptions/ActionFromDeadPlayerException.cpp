#include "ActionFromDeadPlayerException.h"

const char *ActionFromDeadPlayerException::what() const noexcept {
    return "a dead player can't perform this action";
}
