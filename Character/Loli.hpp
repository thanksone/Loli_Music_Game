#ifndef LOLI_HPP
#define LOLI_HPP
#include <string>
#include "UI/Component/Image.hpp"
#include "Engine/IScene.hpp"
class Loli{
private:

public:
    Loli();
    Loli(std::string namae, int san, int fullsan, bool wind);
    std::string name;
    int fullsan, san;
    bool wind;
    void draw(Engine::IScene *scene, float x, float y, float w, float h, float anchorX, float anchorY);
};
#endif //LOLI_HPP
