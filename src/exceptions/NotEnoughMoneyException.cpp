#include "NotEnoughMoneyException.h"

const char* NotEnoughMoneyException::what() const noexcept {
    return "not enough money";
}
