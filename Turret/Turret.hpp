#ifndef TURRET_HPP
#define TURRET_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "UI/Component/Image.hpp"

class Enemy;
class PlayScene;

class Turret: public Engine::Image {
protected:
    int ghost;
    PlayScene* getPlayScene();
    std::string ouob, douo;
public:
    Enemy* Target[4];
    Turret(std::string ouob, std::string douo, float x, float y, int ghost);
    void Down();
    void Up();
    void Hit(int type);
    void SearchTarget(int type);
    void Update(float deltaTime) override;
};
#endif // TURRET_HPP
