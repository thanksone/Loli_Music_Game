#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <list>
#include <vector>
#include <string>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

class Bullet;
class PlayScene;
class Turret;

class Enemy : public Engine::Sprite {
protected:
    int ghost;
	float speed;
    std::string bombimg;
	PlayScene* getPlayScene();
public:
    int type;
    float perfect;
	Enemy(std::string img, std::string bombimg, float x, float y, int type, int ghost, float speed);
 	void Hit(bool miss);
	void Update(float deltaTime) override;
	void Draw() const override;
};
#endif // ENEMY_HPP
