#ifndef LOLI_HPP
#define LOLI_HPP
#include <string>
#include "UI/Component/Image.hpp"
class Loli : public Engine::Image {
private:
    std::string name;
    int fullsan, san;
public:
    int GetSan();
    int GetFullSan();
    void ChangeSan(int val);
};
#endif //LOLI_HPP
