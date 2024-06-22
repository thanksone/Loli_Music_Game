#include "Loli.hpp"
Loli::Loli(std::string namae, int san, int fullsan, bool wind): name(namae), san(san), fullsan(fullsan), wind(wind){}
void Loli::draw(Engine::IScene *scene, float x, float y, float w, float h, float anchorX, float anchorY){
    std::string tachie = (wind? "characters/sanbad" : "characters/sangood") + name + ".png";
    scene->AddNewObject(new Engine::Image(tachie, x, y, w, h, anchorX, anchorY));
}