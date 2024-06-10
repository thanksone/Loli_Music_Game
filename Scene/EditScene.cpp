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
    ReadScore();
    ConstructUI();
    ClearScene();
    Display();
}
void EditScene::Terminate() {
    IScene::Terminate();
}
void EditScene::Update(float deltatime) {

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
        State[gon][on] *= 10;
        State[gon][on] += ('0'+keyCode-ALLEGRO_KEY_0);
    }
    else if(keyCode >= ALLEGRO_KEY_PAD_0 && keyCode <= ALLEGRO_KEY_PAD_9) {
        State[gon][on] *= 10;
        State[gon][on] +=('A'+keyCode-ALLEGRO_KEY_A);
    }
    else if(keyCode == ALLEGRO_KEY_BACKSPACE) {
        State[gon][on] /= 10;
    }
    Word[gon][on]->Text = std::to_string(State[gon][on]);
}
void EditScene::BackOnClick(){
    SaveOnClick();
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void EditScene::SaveOnClick(){
    std::ofstream fout;
    fout.open("Resource/scoreboard.txt");
    fout << State.size() << "\n";
    for(int i = 0; i < State.size(); i++){
        for(int x : State[i]) fout << x << " ";
        for(auto [type, ghost, len, at, speed] : Note[i]){
            fout << type << " " << len << " " << ghost << " " << time << " " << speed << "\n";
        }
        fout << "\n";
    }
    fout.close();
}
void EditScene::InsertOnClick(int type){
    on = type;
}
void EditScene::AddOnClick(){
    State.push_back(State[State.size() - 1]);
    //TODO : label position
    Word.push_back({new Engine::Label("", "pirulen.ttf", 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5), new Engine::Label("", "pirulen.ttf", 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5)});
    Note.push_back({});
}
void EditScene::LPMOnClick(int val){
    if(!(lpm + val)) return;
    lpm += val;
}
void EditScene::POSSliderOnValueChanged(float value){
    ClearScene();
    FindPos(std::min((int)((float)total * value), total - 4));
    Display();
}
void EditScene::FindPos(int pos){
    int sum = 0, i, j;
    for(i = 1; i < State.size(); i++){
        if(sum + State[i][1] > pos) break;
        sum += State[i][1];
    }
    gon = i;
    for(j = 0; j < State[i][1]; j++){
        if(sum + j > pos) break;
    }
    pi = j;
}
void EditScene::ReadScore(){
    State.clear(), Word.clear(), Note.clear();
    std::string file = "Resource/EditScore/" + filename;
    std::ifstream fin(file);
    int bpm, time, notes, type, ghost;
    float len, at, speed;
    fin >> gon;
    State.push_back({0, 0, 0}), Word.push_back({nullptr, nullptr}), Note.push_back({});
    for(int i = 1; i <= gon; i++){
        fin >> bpm >> time >> notes;
        State.push_back({bpm, time, notes});
        //TODO : label position
        Word.push_back({new Engine::Label("", "pirulen.ttf", 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5), new Engine::Label("", "pirulen.ttf", 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5)});
        for(int j = 0; j < notes; j++){
            fin >> type >> ghost >> len >> at >> speed;
            Note[i].push_back({type, ghost, len, at, speed});
        }
    }
    fin.close();
}
void EditScene::ConstructUI(){
    Engine::ImageButton* btn;
    //TODO : button position
    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 200, halfH * 7 / 4 - 50, 300, 150);
    btn->SetOnClickCallback(std::bind(&EditScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW, halfH * 7 / 4, 125, 30, 32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 400, halfH * 7 / 4 - 50, 300, 150);
    btn->SetOnClickCallback(std::bind(&EditScene::SaveOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Save", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW - 200, halfH * 7 / 4, 125,30,32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 200, halfH * 5 / 4 - 50, 300, 150);
    btn->SetOnClickCallback(std::bind(&EditScene::InsertOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("bpm", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW, halfH * 5 / 4, 125,30,32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 400, halfH * 5 / 4 - 50, 300, 150);
    btn->SetOnClickCallback(std::bind(&EditScene::InsertOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("meter", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW - 200, halfH * 5 / 4, 125,30,32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 200, halfH * 3 / 4 - 50, 300, 150);
    btn->SetOnClickCallback(std::bind(&EditScene::LPMOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("+", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW - 200, halfH * 3 / 4, 125,30,32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 200, halfH * 3 / 4 - 50, 300, 150);
    btn->SetOnClickCallback(std::bind(&EditScene::LPMOnClick, this, -1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("-", "pirulen.ttf", 48, halfW - 100, halfH * 3 / 4, 0, 0, 0, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("lines per meter", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW - 200, halfH * 3 / 4, 255, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 400, halfH * 3 / 4 - 50, 300, 150);
    btn->SetOnClickCallback(std::bind(&EditScene::AddOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("ADD", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW - 200, halfH * 3 / 4, 125,30,32, 255, 0.5, 0.5));

    Slider *sliderPOS;
    sliderPOS = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
    sliderPOS->SetOnValueChangedCallback(std::bind(&EditScene::POSSliderOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderPOS);
    sliderPOS->SetValue(0);
}
void EditScene::ConstructNote(int g, note N){
    int p = onField.size();
    onField.push_back({g, N});
    AddNoteButton(g, N);
}
void EditScene::Display(){
    int g = gon, p = pi;
    for(int i = 0; i < 4; i++, p++){
        while(p >= State[g][1]) g++, p = 0;
        for(auto [type, ghost, len, at, speed] : Note[g]) {
            if (at >= p && at < p + 1) ConstructNote(g, note(type, ghost, len, at, speed));
        }
    }
}
void EditScene::DeleteNoteClick(int n){
    auto [g, tmp] = onField[n];
    for(int i = 0; i < Note[g].size(); i++){
        if(Note[g][i] == tmp){
            Note[g].erase(Note[g].begin() + i);
            break;
        }
    }
    DeleteNoteButton(n);
}
void EditScene::DeleteNoteButton(int n){
    RemoveControlObject(NoteButtonCtrl[n]->GetControlIterator(), NoteButtonObj[n]->GetObjectIterator());
    NoteButtonCtrl[n] = nullptr, NoteButtonObj[n] = nullptr;
}
void EditScene::AddNoteButton(int g, note N){
    //TODO : button position
    Engine::ImageButton *btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 400, halfH * 3 / 4 - 50, 300, 150);
    btn->SetOnClickCallback(std::bind(&EditScene::DeleteNoteClick, this, NoteButtonCtrl.size()));
    NoteButtonCtrl.push_back(btn), NoteButtonObj.push_back(dynamic_cast<IObject*>(btn));
    addObject(true, NoteButtonObj.back());
    addControl(true, btn);
}
void EditScene::ClearScene(){
    onField.clear();
    for(int i = 0; i < NoteButtonCtrl.size(); i++){
        if(NoteButtonCtrl[i]) DeleteNoteButton(i);
    }
    NoteButtonCtrl.clear(), NoteButtonObj.clear();
}