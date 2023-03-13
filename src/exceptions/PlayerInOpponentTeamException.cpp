#include "PlayerInOpponentTeamException.h"

const char *PlayerInOpponentTeamException::what() const noexcept {
    return "player is in the opponent team";
}
