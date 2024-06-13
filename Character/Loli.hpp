#ifndef LOLI_HPP
#define LOLI_HPP
#include <string>
#include "UI/Component/Image.hpp"
class Loli{
private:

public:
    Loli();
    Loli(std::string namae, int san, int fullsan): name(namae), san(san), fullsan(fullsan){}
    std::string name;
    int fullsan, san;
    bool wind;
    Engine::Image* Tachie;
};
#endif //LOLI_HPP
