#include <allegro5/allegro.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include<iostream>

#include "Engine/Point.hpp"
#include "Engine/AudioHelper.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Animation/Plane.hpp"
#include "EditScene.hpp"

void EditScene::Initialize(){
    doko = 0;
}
void EditScene::Terminate(){

}
void EditScene::Update(float deltatime){

}
void EditScene::Draw() const{

}
void EditScene::OnMouseDown(int button, int mx, int my) {

}
void EditScene::OnMouseMove(int mx, int my){

}
void EditScene::OnMouseUp(int button, int mx, int my){

}
void EditScene::OnKeyDown(int keyCode){
    if(!doko) return;
    IScene::OnKeyDown(keyCode);
    if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        inf[gon][doko] *= 10;
        inf[gon][doko] +=('0'+keyCode-ALLEGRO_KEY_0);
    }
    else if(keyCode >= ALLEGRO_KEY_PAD_0 && keyCode <= ALLEGRO_KEY_PAD_9) {
        inf[gon][doko] *= 10;
        inf[gon][doko] +=('A'+keyCode-ALLEGRO_KEY_PAD_0);
    }
    else if(keyCode == ALLEGRO_KEY_BACKSPACE) {
        inf[gon][doko] /= 10;
    }
    RemoveObject(show[gon][doko]->GetObjectIterator());
    show[gon][doko] = new Engine::Label(std::to_string(inf[gon][doko]),"pirulen.ttf", 48, Engine::GameEngine::GetInstance().GetScreenSize().x/2-200, Engine::GameEngine::GetInstance().GetScreenSize().y/8+50, 255, 255, 255, 255, 0, 0.5);
    addObject(1,show[gon][doko]);
}