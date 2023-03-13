#include "AttackDeadPlayerException.h"

const char *AttackDeadPlayerException::what() const noexcept {
    return "attacking a dead player";
}
