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
#include "UI/Component/Slider.hpp"

void EditScene::Initialize() {
    halfW = Engine::GameEngine::GetInstance().GetScreenSize().x / 2;
    halfH = Engine::GameEngine::GetInstance().GetScreenSize().y / 2;
    gon = pi = on = total = 0;
    state.push_back(std::vector<int>(3, 0)), word.push_back({}), note.push_back({});
    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&EditScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 400, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&EditScene::SaveOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Save", "pirulen.ttf", 48, halfW - 200, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 5 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&EditScene::InsertOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("bpm", "pirulen.ttf", 48, halfW, halfH * 5 / 4, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 400, halfH * 5 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&EditScene::InsertOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("meter", "pirulen.ttf", 48, halfW - 200, halfH * 5 / 4, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 3 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&EditScene::LPMOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("+", "pirulen.ttf", 48, halfW - 200, halfH * 3 / 4, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 3 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&EditScene::LPMOnClick, this, -1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("-", "pirulen.ttf", 48, halfW - 100, halfH * 3 / 4, 0, 0, 0, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("lines per meter", "pirulen.ttf", 48, halfW - 200, halfH * 3 / 4, 255, 255, 255, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 400, halfH * 3 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&EditScene::AddOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("ADD", "pirulen.ttf", 48, halfW - 200, halfH * 3 / 4, 0, 0, 0, 255, 0.5, 0.5));
    Slider *sliderPOS;
    sliderPOS = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
    sliderPOS->SetOnValueChangedCallback(std::bind(&EditScene::POSSliderOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderPOS);
    AddNewObject(new Engine::Label("POS: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH - 50, 255, 255, 255, 255, 0.5,0.5));
    sliderPOS->SetValue(0);
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
    on = 0;
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
    if(on == 0) return;
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
    word[gon][on]=new Engine::Label(std::to_string(state[gon][on]),"pirulen.ttf", 48, halfW-200, halfH/4+50, 255, 255, 255, 255, 0, 0.5);
    addObject(1,word[gon][on]);
}
void EditScene::BackOnClick(){
    SaveOnClick();
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void EditScene::SaveOnClick(){
    std::ofstream fout;
    fout.open("Resource/scoreboard.txt");
    fout << state.size() << "\n";
    for(int i = 0; i < state.size(); i++){
        for(int x : state[i]) fout << x << " ";
        for(std::vector<int> notes : note[i]){
            for(int x : notes) fout << x << " ";
            fout << "\n";
        }
        fout << "\n";
    }
    fout.close();
}
void EditScene::InsertOnClick(int type){
    on = type;
}
void EditScene::AddOnClick(){
    state.push_back(state[state.size() - 1]);
    word.push_back(std::vector<Engine::Label*>(2, nullptr));
    note.push_back({});
}
void EditScene::LPMOnClick(int val){
    if(!(lpm + val)) return;
    lpm += val;
}
void EditScene::POSSliderOnValueChanged(float value){
    int sum = 0, i, j;
    for(i = 1; i < state.size(); i++){
        if((float)(sum + state[i][2]) / (float)total > value) break;
        sum += state[i][2];
    }
    gon = i;
    for(j = 0; j < state[i][2]; j++){
        if((float)(sum + j) / (float)total > value) break;
    }
    pi = j;
}