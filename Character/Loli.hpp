#ifndef LOLI_HPP
#define LOLI_HPP
#include <string>
#include "UI/Component/Image.hpp"
#include "Engine/IScene.hpp"
#include "UI/Component/ImageButton.hpp"


class Loli{
private:

public:
    Loli();
    Loli(std::string namae, int san, int fullsan, bool wind);
    std::vector<std::string> commu;
    std::string name;
    int fullsan, san;
    bool wind;
    std::string talk();
    void draw(Engine::IScene *scene, float x, float y, float w, float h, float anchorX, float anchorY);
    Engine::ImageButton* touch(Engine::IScene *scene, float x, float y, float w, float h);

};
#endif //LOLI_HPP
