#include "User.hpp"
bool User::SanCheck(){
    return Character[onField].GetSan() >= Character[onField].GetFullSan() * 4 / 5;
}
