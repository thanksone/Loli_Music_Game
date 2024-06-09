#ifndef LOLI_HPP
#define LOLI_HPP
#include <string>
#include "UI/Component/Image.hpp"
class Loli : public Engine::Image {
private:
    std::string name;
    int san;
public:
    int GetSan();
    void ChangeSan(int val);
};
#endif //LOLI_HPP
