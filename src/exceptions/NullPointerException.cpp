#include "NullPointerException.h"

#include <utility>

NullPointerException::NullPointerException(string variable_name) : exception(), variable_name(std::move(variable_name)) {}

const char *NullPointerException::what() const noexcept {
    return ("pointer '" + variable_name + "' is null").c_str();
}
