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
    pi = on = total = 0, lpm = 1;
    ghostW = 250, lineH = 250;
    imgTarget = nullptr;
    ReadScore();
    ConstructUI();
    ClearNote();
    ClearLine();
    DisplayNote();
    DisplayLine();
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
    if(!imgTarget) return;
    int x = (mx - x0) / ghostW, y = my / lineH;
    if (mx < 0 || mx >= (halfW << 1) || my < 0 || my >= (halfH << 1)) {
        imgTarget->Visible = false;
        return;
    }
    imgTarget->Visible = true;
    imgTarget->Position.x = x * ghostW;
    imgTarget->Position.y = y * lineH;
}
void EditScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
    int x = (mx - x0) / ghostW, y = my / round(lineH * lpm);
    if(!imgTarget || mx < x0 || mx >= x0 + 6 * ghostW || my < 0 || my >= 4 * lineH){
        imgTarget = nullptr;
        return;
    }
    AddNoteButton(holdtype, len, x0 + x * ghostW + ghostW / 2, y * lineH);
}
void EditScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if(!on) return;
    if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        BPM[pi + on - 1] *= 10;
        BPM[pi + on - 1] += ('0'+keyCode-ALLEGRO_KEY_0);
    }
    else if(keyCode >= ALLEGRO_KEY_PAD_0 && keyCode <= ALLEGRO_KEY_PAD_9) {
        BPM[pi + on - 1] *= 10;
        BPM[pi + on - 1] +=('A'+keyCode-ALLEGRO_KEY_A);
    }
    else if(keyCode == ALLEGRO_KEY_BACKSPACE) {
        BPM[pi + on - 1] /= 10;
    }
    Word[pi + on - 1]->Text = std::to_string(BPM[pi + on - 1]);
}
void EditScene::BackOnClick(){
    SaveOnClick();
    Engine::GameEngine::GetInstance().ChangeScene("main");
}
void EditScene::SaveOnClick(){
    std::ofstream fout;
    fout.open("Resource/scoreboard.txt");
    fout << total << "\n";
    for(int i = 0; i < total; i++){
        fout << BPM[i] << " " << Note[i].size() << "\n";
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
    BPM.push_back(BPM.back());
    //TODO : label position
    Word.push_back(new Engine::Label("", "pirulen.ttf", 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5));
    Note.push_back({});
}
void EditScene::LPMOnClick(int val){
    if(!(lpm + val) && lpm + val > 10) return;
    lpm += val;
    lineH = (float)halfH / 2.0 / (float)lpm;
    ClearLine();
    DisplayLine();
}
void EditScene::POSSliderOnValueChanged(float value){
    ClearNote();
    pi = std::max(0, std::min((int)((float)total * value), total - 4));
    DisplayNote();
}
void EditScene::ReadScore(){
    BPM.clear(), Word.clear(), Note.clear();
    std::string file = "Resource/EditScore/" + filename;
    std::ifstream fin(file);
    int bpm, time, notes, type, ghost;
    float len, at, speed;
    fin >> total;
    BPM.push_back(0), Word.push_back(nullptr), Note.push_back({});
    for(int i = 1; i <= total; i++){
        if(!(fin >> bpm >> notes)) break;
        BPM.push_back(bpm);
        //TODO : label position
        Word.push_back(new Engine::Label("", "pirulen.ttf", 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5));
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
void EditScene::DisplayNote(){
    for(int i = pi; i < pi + 4; i++){
        for(note N : Note[i]){
            AddNoteButton(N.type, N.len , x0 + N.ghost * ghostW + ghostW / 2, halfH * 2 - (i - pi + N.at) * lineH * (float)lpm);
        }
    }
}
void EditScene::DisplayLine(){
    Engine::Label* line;
    std::string foo, bar;
    for(int i = 0; i < 60; i++) foo += '_';
    for(int i = 0; i < 90; i++) bar += '_';
    for(float i = 1000; i > 0; i -= lineH){
        line = new Engine::Label((int)round(i) % (int)round((float)lpm * lineH)? bar : foo, "pirulen.ttf", (int)round(i) % (int)round((float)lpm * lineH)? 20 : 30, x0, (int)i, 255, 255, 255, 255, 0, 1);
        Line.push_back(line);
        addObject(1, line);
    }
    std::cout << "\n";
}
void EditScene::DeleteNoteClick(int p){
    for(int i = 0; i < Note[p].size(); i++){
        if(Note[p][i] == ){
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
void EditScene::AddNoteButton(int type, float len, int x, int y){
    //TODO : button position
    onField.push_back(Note())
    Engine::ImageButton *btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 400, halfH * 3 / 4 - 50, 300, 150, 0.5, 0.5);
    btn->SetOnClickCallback(std::bind(&EditScene::DeleteNoteClick, this, NoteButtonCtrl.size()));
    NoteButtonCtrl.push_back(btn), NoteButtonObj.push_back(dynamic_cast<IObject*>(btn));
    addObject(true, NoteButtonObj.back());
    addControl(true, btn);
}
void EditScene::ClearNote(){
    onField.clear();
    for(int i = 0; i < 4; i++) L[G[i]] = -1, G[i] = 0;
    for(int i = 0; i < NoteButtonCtrl.size(); i++){
        if(NoteButtonCtrl[i]) DeleteNoteButton(i);
    }
    NoteButtonCtrl.clear(), NoteButtonObj.clear();
}
void EditScene::ClearLine(){
    for(Engine::Label* &line : Line){
        RemoveObject(line->GetObjectIterator());
    }
    Line.clear();
}