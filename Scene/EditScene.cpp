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

#define mid (l + r >> 1)
#define lc (p << 1)
#define rc (p << 1 | 1)
#define ff first
#define ss second

int seg[6][4096], tag[6][4096];
void pull(int g, int p){
    seg[g][p] = seg[g][lc] + seg[g][rc];
}
void push(int g, int p, int l, int r){
    seg[g][lc] += (mid - l + 1) * tag[g][p];
    seg[g][rc] += (r - mid) * tag[g][p];
    tag[g][lc] += tag[g][p], tag[g][rc] += tag[g][p];
    tag[g][p] = 0;
}
void update(int g, int p, int l, int r, int ql, int qr, int x){
    if(ql > r || qr < l) return;
    if(l >= ql && r <= qr){
        seg[g][p] += (r - l + 1) * x;
        tag[g][p] += x;
    }
    push(g, p, l, r);
    update(g, lc, l, mid, ql, qr, x);
    update(g, rc, mid + 1, r, ql, qr, x);
    pull(g, p);
}
int query(int g, int p, int l, int r, int ql, int qr){
    if(ql > r || qr < l) return 0;
    if(l >= ql && r <= qr) return seg[g][p];
    push(g, p, l, r);
    return query(g, lc, l, mid, ql, qr) + query(g, rc, mid + 1, r, ql, qr);
}
bool cmp(note a, note b){
    return a.at < b.at;
}
void EditScene::Initialize() {
    halfW = Engine::GameEngine::GetInstance().GetScreenSize().x / 2;
    halfH = Engine::GameEngine::GetInstance().GetScreenSize().y / 2;
    pi = 0, on = total = 0, lpm = 1;
    ghostW = 200, lineH = 250;
    x0 = 40;
    speed = 1;
    last = 0;
    imgTarget.clear(), Boing.clear(), show.clear();
    for(int i = 0; i < 4; i++) show.push_back(nullptr);
    std::string L;
    for(int i = 0; i < 60; i++) L += '_';
    RunningLine = new Engine::Label(L, "pirulen.ttf", 30, x0, 1000);
    AddNewObject(RunningLine); 
    RunningLine->Visible = 0;
    for(int i = 0; i < 6; i++){
        for(int &s : seg[i]) s = 0;
        for(int &t : tag[i]) t = 0;
    }
    std::cout << "flag1\n";
    ReadScore();
    std::cout << "flag2\n";
    ConstructUI();
    std::cout << "flag3\n";
    ClearNote();
    std::cout << "flag4\n";
    ClearLine();
    std::cout << "flag5\n";
    DisplayNote();
    std::cout << "flag6\n";
    DisplayLine();
    std::cout << "flag7\n";
}
void EditScene::Terminate() {
    AudioHelper::StopSample(audio);
    IScene::Terminate();
}
void EditScene::Update(float deltatime) {
    if(last <= 0) return;
    past += deltatime, last -= deltatime;
    while(front < Boing.size() && Boing[front].at <= past){
        if(Boing[front].type) tapsound = AudioHelper::PlaySample("Resources/audios/tap.ogg", 0, AudioHelper::SFXVolume, 0);
        else holdsound = AudioHelper::PlaySample("Resources/audios/hold.ogg", 0, AudioHelper::SFXVolume, 0);
    }
    while(!Time.empty() && past >= Time.front()){
        Time.pop();
        now++;
        if(now >= pi + 4){
            pi = std::min(now, total - 4);
            ClearNote();
            DisplayNote();
            RunningLine->Position = Engine::Point(x0, (pi - now) * 250);
        }
    }
    RunningLine->Position.y += 250.0 * (60.0 / BPM[now]) * deltatime;
    if(last <= 0){
        AudioHelper::StopSample(audio);
        RunningLine->Position = Engine::Point(x0, 1000);
        RunningLine->Visible = 0;
    }
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
    if (imgTarget.empty()) return;
    int x = (mx - x0) / ghostW, y = my / lineH;
    if (mx < 0 || mx >= (halfW << 1) || my < 0 || my >= (halfH << 1)) {
        for (Engine::IObject *img : imgTarget) img->Visible = false;
        return;
    }
    for(Engine::IObject *img : imgTarget){
        img->Visible = true;
        img->Position.x = x * ghostW;
        img->Position.y = y * lineH;
    }
}
void EditScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
    my = 1000 - my;
    int x = (mx - x0) / ghostW, y = my / 250;
    if(imgTarget.empty() || mx < x0 || mx > x0 + 6 * ghostW || my < 0 || my >= 1000 || pi + y >= total){
        for(Engine::IObject *img : imgTarget) RemoveObject(img->GetObjectIterator());
        imgTarget.clear();
        return;
    }
    my = (int)round((float)(my / lpm) * lineH);
    note N = {hold, x, len, (float)(my % 250) / (float)250, speed};
    if(CheckSpaceValid(N, my)){
        AddNoteButton(N, x0 + x * ghostW + ghostW / 2, my);
        Note[y].push_back(N);
    }
    for(Engine::IObject *img : imgTarget) RemoveObject(img->GetObjectIterator());
    imgTarget.clear();
}
void EditScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if(!on) return;
    if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        BPMS[pi + on - 1] += (char)('0' + keyCode - ALLEGRO_KEY_0);
    }else if(keyCode == ALLEGRO_KEY_FULLSTOP) {
        BPMS[pi + on - 1] += '.';
    }else if(keyCode == ALLEGRO_KEY_BACKSPACE) {
        if(BPMS[pi + on - 1].size()) BPMS[pi + on - 1].pop_back();
    }
    if(BPMS[pi + on - 1].empty() || std::stof(BPMS[pi + on - 1]) == 0.0) BPM[pi + on - 1] = 60;
    else BPM[pi + on - 1] = std::stof(BPMS[pi + on - 1]);
    show[on - 1]->Text = BPMS[pi + on - 1];
}
void EditScene::BackOnClick(){
    SaveOnClick("editting");
    Engine::GameEngine::GetInstance().ChangeScene("main");
}
void EditScene::SaveOnClick(std::string dir){
    std::ofstream fout;
    fout.open("Resource/" + dir + "/" + songname + "/" + diff + ".txt");
    fout << total << "\n";
    for(int i = 0; i < total; i++){
        fout << BPM[i] << " " << Note[i].size() << "\n";
        for(auto [type, ghost, len, at, speed] : Note[i]){
            fout << type << " " << len << " " << ghost << " " << at << " " << speed << "\n";
        }
        fout << "\n";
    }
    fout.close();
}
void EditScene::InsertOnClick(int type){
    on = type;
}
void EditScene::AddOnClick(){
    total++;
    if(BPM.size()) BPM.push_back(BPM.back()), BPMS.push_back(BPMS.back());
    else BPM.push_back(60), BPMS.push_back("60");
    //TODO : label position
    Note.push_back({});
}
void EditScene::DeleteOnClick() {
    total--;
    BPM.pop_back(), BPMS.pop_back(), Note.pop_back();
    if(pi + 3 == Note.size()){
        pi = std::max(0, pi - 1);
        ClearNote(), DisplayNote();
    }
}
void EditScene::LineAddOnClick(int val){
    if(!(lpm + val) || lpm + val > 12) return;
    lpm += val;
    lineH = 250.0 / (float)lpm;
    ClearLine();
    DisplayLine();
}
void EditScene::LenAddOnClick(int val){
    if(!(len + val) || len + val > 4) return;
    len += val;
    if(~val) HoldNote[len]->Visible = 1;
    else HoldNote[len + 1]->Visible = 0;
}
void EditScene::SpeedAddOnClick(float val){
    if(speed + val <= 0 || speed + val > 5) return;
    speed += val;
}
void EditScene::PiAddOnClick(int val){
    if(!~(pi + val) || pi + val >= total) return;
    pi += val;
    ClearNote();
    DisplayNote();
}
void EditScene::PlayOnClick(){
    float time = 0;
    last = past = 0;
    now = pi, front = 0;
    Boing.clear();
    while(!Time.empty()) Time.pop();
    for(int i = 0; i < pi; i++) time += 60.0 / BPM[i];
    for(int i = pi; i < pi + 4; i++){
        for(auto [t, g, l, a, s] : Note[i]){
            if(!t){
                Boing.push_back({t, g, l, a * (float)60 / BPM[i] + last, s});
                continue;
            }
            for(int j = 0; j < l; j++){
                Boing.push_back({t, g, l, (a + (float)j / (float)4) * (float)60 / BPM[i] + last, s});
            }
        }
        last += 60.0 / BPM[i];
        Time.push(last);
    }
    std::sort(Boing.begin(), Boing.end(), cmp);
    RunningLine->Visible = 1;
    audio = AudioHelper::PlaySample("Resources/audios/songs/" + songname + ".ogg", 0, AudioHelper::BGMVolume, time);
}
void EditScene::PlayHeadOnClick(){
    last = past = 0;
    now = front = 0;
    while(!Time.empty()) Time.pop();
    for(int i = 0; i < total; i++){
        for(auto [t, g, l, a, s] : Note[i]){
            if(!t){
                Boing.push_back({t, g, l, a * (float)60 / BPM[i] + last, s});
                continue;
            }
            for(int j = 0; j < l; j++){
                Boing.push_back({t, g, l, (a + (float)j / (float)4) * (float)60 / BPM[i] + last, s});
            }
        }
        last += 60.0 / BPM[i];
        Time.push(last);
    }
    pi = 0, ClearNote(), DisplayNote();
    std::sort(Boing.begin(), Boing.end(), cmp);
    RunningLine->Visible = 1;
    audio = AudioHelper::PlaySample("Resources/EditScore/Song/" + songname + ".ogg", 0, AudioHelper::BGMVolume, 0);
}
void EditScene::StopOnClick(){
    last = 0;
    Boing.clear();
    AudioHelper::StopSample(audio);
    RunningLine->Visible = 0;
}
void EditScene::POSSliderOnValueChanged(float value){
    ClearNote();
    pi = std::max(0, std::min((int)((float)total * value), total - 4));
    DisplayNote();
}
void EditScene::ReadScore(){
    BPM.clear(), BPMS.clear(), Note.clear();
    std::string file = "Resource/editting/" + songname +  "/" + diff + ".txt";
    std::ifstream fin(file);
    int time, notes, type, ghost, len;
    float at, speed, bpm;
    fin >> total;
    for(int i = 0; i < total; i++){
        if(!(fin >> bpm >> notes)) break;
        BPM.push_back(bpm), BPMS.push_back(std::to_string(bpm));
        for(int j = 0; j < notes; j++){
            fin >> type >> ghost >> len >> at >> speed;
            Note[i].push_back({type, ghost, len, at, speed});
        }
    }
    fin.close();
}
void EditScene::DeleteNoteClick(int k){
    auto [p, N] = onField[k];
    for(int i = 0; i < Note[p].size(); i++){
        if(Note[p][i] == N){
            Note[p].erase(Note[p].begin() + i);
            break;
        }
    }
    DeleteNoteButton(k);
}
void EditScene::DeleteNoteButton(int n){
    int y = onField[n].ff * 250 + round(onField[n].ss.at * 250.0);
    if(onField[n].ss.type) update(onField[n].ss.ghost, 1, 0, 1024, y, y + round((float)onField[n].ss.len * 250.0 / 4.0) - 1, -1);
    else update(onField[n].ss.ghost, 1, 0, 1024, y, y, -1);
    RemoveControl(NoteButtonCtrl[n]->GetControlIterator());
    for(Engine::IObject *img : NoteButtonObj[n]) RemoveObject(img->GetObjectIterator());
    NoteButtonCtrl[n] = nullptr, NoteButtonObj[n].clear();
}
void EditScene::AddNoteButton(note N, int x, int y){
    onField.push_back({(1000 - y) / 250, N});
    if(N.type) update(N.ghost, 1, 0, 1024, y, y + round((float)N.len * 250.0 / 4.0) - 1, 1);
    else update(N.ghost, 1, 0, 1024, y, y, 1);
    Engine::ImageButton *btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", x, 1000 - y, 0, 0, 0.5, 0.5);
    btn->SetOnClickCallback(std::bind(&EditScene::DeleteNoteClick, this, NoteButtonCtrl.size()));
    NoteButtonCtrl.push_back(btn), NoteButtonObj.push_back({dynamic_cast<IObject*>(btn)});
    for(int i = 1; i < N.len; i++) NoteButtonObj.back().push_back(new Engine::Image("HoldNote.png", x, 1000 - (y + round((float)i * 250.0 / 4.0)), 0, 0, 0.5, 0.5));
    for(int i = 0; i < N.len; i++) addObject(true, NoteButtonObj.back()[i]);
    addControl(true, btn);
}
void EditScene::DisplayNote(){
    for(int i = pi; i < std::min(total, pi + 4); i++){
        if(i >= Note.size()) break;
        for(note N : Note[i]){
            AddNoteButton(N, x0 + N.ghost * ghostW + ghostW / 2, round(250.0 * ((float)(i - pi) + N.at)));
        }
    }
    for(int i = 0; i < std::min(total - pi, 4); i++){
        show[i] = new Engine::Label(BPMS[pi + i], "pirulen.ttf", 48, 100, 200, 255, 255, 255, 255, 0.5, 0.5);
        addObject(1, show[i]);
    }
}
void EditScene::DisplayLine(){
    Engine::Label* line;
    std::string foo, bar;
    for(int i = 0; i < 60; i++) foo += '_';
    for(int i = 0; i < 90; i++) bar += '_';
    for(float i = 1000; i > 0; i -= lineH){
        line = new Engine::Label((int)round(i) % 250? bar : foo, "pirulen.ttf", (int)round(i) % (int)round((float)lpm * lineH)? 20 : 30, x0, (int)i, 255, 255, 255, 255, 0, 1);
        Line.push_back(line);
        addObject(1, line);
    }
}
void EditScene::ClearNote(){
    onField.clear();
    for(int i = 0; i < NoteButtonCtrl.size(); i++){
        if(NoteButtonCtrl[i]) DeleteNoteButton(i);
    }
    NoteButtonCtrl.clear(), NoteButtonObj.clear();
    for(int i = 0; i < 4; i++){
        if(show[i]) RemoveObject(show[i]->GetObjectIterator());
    }
}
void EditScene::ClearLine(){
    for(Engine::Label* &line : Line){
        RemoveObject(line->GetObjectIterator());
    }
    Line.clear();
}
bool EditScene::CheckSpaceValid(note N, int y){
    if(N.type && (float)(y % 250) + (float)(len - 1) * 250.0 / 4.0 < 250.0) return 0;
    if(N.type && query(N.ghost, 1, 0, 1024, y, y + round((float)N.len * 250.0 / 4.0) - 1)) return 0;
    if(!N.type && query(N.ghost, 1, 0, 1024, y, y)) return 0;
    return 1;
}
void EditScene::ConstructUI(){
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x, h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    halfW = w / 2;
    halfH = h / 2;
    Engine::ImageButton* btn;
    //TODO : button position
    /*btn->SetOnClickCallback(std::bind(&EditScene::BackOnClick, this));
    btn->SetOnClickCallback(std::bind(&EditScene::SaveOnClick, this, "editting"));
    btn->SetOnClickCallback(std::bind(&EditScene::SaveOnClick, this, "scores"));
    btn->SetOnClickCallback(std::bind(&EditScene::InsertOnClick, this, 1));
    btn->SetOnClickCallback(std::bind(&EditScene::InsertOnClick, this, 2));
    btn->SetOnClickCallback(std::bind(&EditScene::AddOnClick, this));
    btn->SetOnClickCallback(std::bind(&EditScene::DeleteOnClick, this));
    btn->SetOnClickCallback(std::bind(&EditScene::LineAddOnClick, this, 1));
    btn->SetOnClickCallback(std::bind(&EditScene::LineAddOnClick, this, -1));
    btn->SetOnClickCallback(std::bind(&EditScene::LenAddOnClick, this, 1));
    btn->SetOnClickCallback(std::bind(&EditScene::LenAddOnClick, this, -1));
    btn->SetOnClickCallback(std::bind(&EditScene::SpeedAddOnClick, this, 0.1));
    btn->SetOnClickCallback(std::bind(&EditScene::SpeedAddOnClick, this, -0.1));
    btn->SetOnClickCallback(std::bind(&EditScene::SpeedAddOnClick, this, 1));
    btn->SetOnClickCallback(std::bind(&EditScene::SpeedAddOnClick, this, -1));
    btn->SetOnClickCallback(std::bind(&EditScene::PiAddOnClick, this, 1));
    btn->SetOnClickCallback(std::bind(&EditScene::PiAddOnClick, this, -1));
    btn->SetOnClickCallback(std::bind(&EditScene::PlayOnClick, this));
    btn->SetOnClickCallback(std::bind(&EditScene::PlayHeadOnClick, this));
    btn->SetOnClickCallback(std::bind(&EditScene::StopOnClick, this));*/
    //TODO HoldNote
    Slider *sliderPOS;
    sliderPOS = new Slider(x0, h - 60, 6 * ghostW, 4, 0);
    sliderPOS->SetOnValueChangedCallback(std::bind(&EditScene::POSSliderOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderPOS);
    sliderPOS->SetValue(0);
}