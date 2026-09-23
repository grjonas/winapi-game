#pragma once

#include <vector>
#include <cstdint>
#include <bitset>
#include <unordered_map>
#include <variant>
#include <optional>
#include <cassert>
#include <memory>

#define UNREACHABLE assert(false && "This code should be unreachable")

#define sign(sign_num) ((sign_num > 0) - (sign_num < 0))
