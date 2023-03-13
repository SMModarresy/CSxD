#include "PlayerAlreadyInTeamException.h"

const char *PlayerAlreadyInTeamException::what() const noexcept {
    return "player is already in the team";
}
