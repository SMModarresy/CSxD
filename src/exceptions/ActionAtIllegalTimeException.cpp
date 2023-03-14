#include "ActionAtIllegalTimeException.h"

const char* ActionAtIllegalTimeException::what() const noexcept {
    return "this action is illegal at this time";
}
