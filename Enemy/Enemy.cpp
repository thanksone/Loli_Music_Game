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
Enemy::Enemy(std::string img, std::string bombimg, float x, float y, int type, int ghost, float speed) :
	Engine::Sprite(img, x, y, 180, 180), bombimg(bombimg), type(type), ghost(ghost), speed(speed){
    Velocity = Engine::Point(0, speed);
    perfect = 0.08 * speed;
}
void Enemy::Hit(bool miss){
    if(!miss && type) getPlayScene()->tap = AudioHelper::PlaySample("hold.ogg", 0, getPlayScene()->user.setting.SFXVolume);
    else if(!miss) getPlayScene()->tap = AudioHelper::PlaySample("tap.ogg", 0, getPlayScene()->user.setting.SFXVolume);
    getPlayScene()->Line[ghost]->Target[type] = nullptr;
    if(!miss) getPlayScene()->EffectGroup->AddNewObject(new DirtyEffect(bombimg, 0.2, Position.x, Position.y));
    getPlayScene()->NoteGroup[ghost][type]->RemoveObject(objectIterator);
}
void Enemy::Update(float deltaTime){
    if(((type & 1) && Position.y - perfect > getPlayScene()->deadline) || Position.y - 3.0 * perfect > getPlayScene()->deadline) {
        getPlayScene()->Hit(4, ghost);
        Hit(1);
    }
	Sprite::Update(deltaTime);
}
void Enemy::Draw() const{
	Sprite::Draw();
}
