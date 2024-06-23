#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <utility>

#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Turret.hpp"
#include "Engine/Resources.hpp"

PlayScene* Turret::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Turret::Turret(std::string ouob, std::string douo, float x, float y, int ghost): Engine::Image(ouob, x, y, 180, 180), ouob(ouob), douo(douo), ghost(ghost){
    for(int i = 0; i < 4; i++) Target[i] = nullptr;
}
void Turret::Hit(int type){
    if(!Target[type]) {
        SearchTarget(type);
        if(!Target[type]) return;
    }
    float dis = std::abs(Target[type]->Position.y - Position.y);
    if(dis <= Target[type]->perfect){
        Target[type]->Hit(0);
        getPlayScene()->Hit(1, ghost);
    }else if(!(type & 1) && dis <= Target[type]->perfect * 2.0){
        Target[type]->Hit(0);
        getPlayScene()->Hit(2, ghost);
    }else if(!(type & 1) && dis <= Target[type]->perfect * 3.0){
        Target[type]->Hit(0);
        getPlayScene()->Hit(3, ghost);
    }
}
void Turret::Down(){
    bmp = Engine::Resources::GetInstance().GetBitmap(douo);
}
void Turret::Up(){
    bmp = Engine::Resources::GetInstance().GetBitmap(ouob);
}
void Turret::SearchTarget(int type){
    int my = 0;
    for(auto it : getPlayScene()->NoteGroup[ghost][type]->GetObjects()){
        if(it->Position.y > my){
            my = it->Position.y;
            Target[type] = dynamic_cast<Enemy*>(it);
        }
    }
}
void Turret::Update(float deltaTime) {
    for(int i = 0; i < 4; i++){
        if(!Target[i]) SearchTarget(i);
    }
}
