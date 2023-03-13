#ifndef CSXD_PLAYERNOTFOUNDEXCEPTION_H
#define CSXD_PLAYERNOTFOUNDEXCEPTION_H


#include <exception>

using namespace std;

class PlayerNotFoundException : public exception {
    const char* what() const noexcept override;
};


#endif //CSXD_PLAYERNOTFOUNDEXCEPTION_H
