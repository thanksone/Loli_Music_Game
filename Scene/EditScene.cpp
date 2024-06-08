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
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "EditScene.hpp"
void EditScene::Initialize() {
    halfW = Engine::GameEngine::GetInstance().GetScreenSize().x / 2;
    halfH = Engine::GameEngine::GetInstance().GetScreenSize().y / 2;
    gon = pi = on = 0;
    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&EditScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
}
void EditScene::Terminate() {
    IScene::Terminate();
}
void EditScene::Update(float deltaTime) {

}
void EditScene::Draw() const {
    IScene::Draw();
}
void EditScene::OnMouseDown(int button, int mx, int my) {
    IScene::OnMouseDown(button, mx, my);
}
void EditScene::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
}
void EditScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
}
void EditScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        state[gon][on] *= 10;
        state[gon][on] += ('0'+keyCode-ALLEGRO_KEY_0);
    }
    else if(keyCode >= ALLEGRO_KEY_PAD_0 && keyCode <= ALLEGRO_KEY_PAD_9) {
        state[gon][on] *= 10;
        state[gon][on] +=('A'+keyCode-ALLEGRO_KEY_A);
    }
    else if(keyCode == ALLEGRO_KEY_BACKSPACE) {
        state[gon][on] /= 10;
    }
    RemoveObject(word[gon][on]->GetObjectIterator());
    word[gon][on]=new Engine::Label(std::to_string(state[gon][on]),"pirulen.ttf", 48, Engine::GameEngine::GetInstance().GetScreenSize().x/2-200, Engine::GameEngine::GetInstance().GetScreenSize().y/8+50, 255, 255, 255, 255, 0, 0.5);
    addObject(1,word[gon][on]);
}
void EditScene::BackOnClick(){
    Engine::GameEngine::GetInstance().ChangeScene("start");
}