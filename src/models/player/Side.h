#ifndef CSXD_SIDE_H
#define CSXD_SIDE_H

#include "nlohmann/json.hpp"

enum Side {
    COUNTER_TERRORIST = 1,
    TERRORIST = 2,
    ALL = 3
};

NLOHMANN_JSON_SERIALIZE_ENUM(Side, {
    {COUNTER_TERRORIST, "counter_terrorist"},
    {TERRORIST, "terrorist"},
    {ALL, "all"},
})

#endif //CSXD_SIDE_H
