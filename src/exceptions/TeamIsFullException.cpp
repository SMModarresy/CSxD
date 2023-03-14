#include "TeamIsFullException.h"

const char* TeamIsFullException::what() const noexcept {
    return "this team is full";
}
