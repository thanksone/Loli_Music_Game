#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "Engine/AudioHelper.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"

PlayScene* Enemy::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Enemy::Enemy(std::string img, float x, float y, int type, int ghost, float speed) :
	Engine::Sprite(img, x, y), type(type), ghost(ghost), speed(speed){
    Velocity = Engine::Point(0, speed);
    perfect = 4.8 * speed;
}
void Enemy::Hit(){
    getPlayScene()->Line[ghost]->Target[type] = nullptr;
    getPlayScene()->NoteGroup[ghost][type]->RemoveObject(objectIterator);
}
void Enemy::Update(float deltaTime){
    if(((type & 1) && Position.y - perfect > getPlayScene()->deadline) || Position.y - 3.0 * perfect > getPlayScene()->deadline) {
        getPlayScene()->Hit(4);
        Hit();
    }
	Sprite::Update(deltaTime);
}
void Enemy::Draw() const{
	Sprite::Draw();
}
