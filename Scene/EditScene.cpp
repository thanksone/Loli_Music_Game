#include <allegro5/allegro.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sstream>

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
#define rc ((p << 1) | 1)
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
        return;
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
std::string tostring(float f){
    std::stringstream SS;
    std::string S;
    SS << std::setprecision(5) << f;
    SS >> S;
    return S;
}
void EditScene::Initialize() {
    halfW = Engine::GameEngine::GetInstance().GetScreenSize().x / 2;
    halfH = Engine::GameEngine::GetInstance().GetScreenSize().y / 2;
    pi = 0, on = total = 0, lpm = 1, len = 1;
    ghostW = 200, lineH = 240;
    x0 = 40;
    speed = 1;
    last = 0;
    hold = 0;
    imgTarget.clear(), Boing.clear(), show = std::vector<Engine::Label*>(4, nullptr), Line.clear(), SB = std::vector<Engine::Label*>(4, nullptr);
    NoteButtonCtrl.clear(), NoteButtonObj.clear(), imgTarget.clear(), HoldNote.clear();
    for(int i = 0; i < 4; i++) show.push_back(nullptr);
    std::string L;
    for(int i = 0; i < 60; i++) L += '_';
    RunningLine = new Engine::Label(L, "pirulen.ttf", 30, x0, 960, 255, 255, 255, 255, 0, 1);
    AddNewObject(RunningLine); 
    RunningLine->Visible = 0;
    for(int i = 0; i < 6; i++){
        for(int &s : seg[i]) s = 0;
        for(int &t : tag[i]) t = 0;
    }
    ReadScore();
    ConstructUI();
    ClearNote();
    ClearLine();
    DisplayNote();
    DisplayLine();
}
void EditScene::Terminate() {
    if(audio) AudioHelper::StopSample(audio);
    audio = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void EditScene::Update(float deltatime) {
    if(last <= 0) {
        if(play){
            PiAddOnClick(-4);
            play = 0;
        }
        return;
    }
    past += deltatime, last -= deltatime;
    while(front < Boing.size() && Boing[front].at <= past){
        if(Boing[front].type) tapsound = AudioHelper::PlaySample("hold.ogg", 0, user.setting.SFXVolume, 0);
        else holdsound = AudioHelper::PlaySample("tap.ogg", 0, user.setting.SFXVolume, 0);
        front++;
    }
    while(!Time.empty() && past >= Time.front()){
        Time.pop();
        now++;
        if(now >= pi + 4){
            PiAddOnClick(4);
            RunningLine->Position.y = (std::min(pi + 4, total) - now) * 240.0;
        }
    }
    RunningLine->Position.y -= 4.0 * BPM[now] * deltatime;
    if(last <= 0){
        AudioHelper::StopSample(audio);
        RunningLine->Position.y = 960;
        RunningLine->Visible = 0;
    }
}
void EditScene::Draw() const {
    IScene::Draw();
}
void EditScene::OnMouseDown(int button, int mx, int my) {
    if(on){
        show[pi + on - 1]->Text = tostring(BPM[pi + on - 1]);
        on = 0;
    }
    IScene::OnMouseDown(button, mx, my);
}
void EditScene::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
    if (imgTarget.empty()) return;
    if (mx < 0 || mx >= (halfW << 1) || my < 0 || my >= (halfH << 1)) {
        for (Engine::IObject *img : imgTarget) img->Visible = false;
        return;
    }
    for(int i = 0; i < imgTarget.size(); i++){
        imgTarget[i]->Visible = true;
        imgTarget[i]->Position.x = mx;
        imgTarget[i]->Position.y = my - 60 * i;
    }
}
void EditScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
    my = 960 - my;
    int x = (mx - x0) / (int)ghostW, y = my / 240;
    if(imgTarget.empty() || mx < x0 || mx > x0 + 6 * ghostW || my < 0 || my >= 960 || pi + y >= total){
        for(Engine::IObject *img : imgTarget) RemoveObject(img->GetObjectIterator());
        imgTarget.clear();
        return;
    }
    my = (int)(my / lineH) * lineH;
    note N = {hold, x, hold * len, (float)(my % 240) / (float)240, speed};
    if(CheckSpaceValid(N, my)){
        AddNoteButton(N, x0 + x * ghostW + (int)ghostW / 2, my);
        Note[y + pi].push_back(N);
    }
    for(Engine::Image* img : imgTarget) RemoveObject(img->GetObjectIterator());
    imgTarget.clear();
}
void EditScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if(!on) return;
    if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        if(BPMS[pi + on - 1].size() >= 5) return;
        BPMS[pi + on - 1] += (char)('0' + keyCode - ALLEGRO_KEY_0);
    }else if(keyCode == ALLEGRO_KEY_FULLSTOP) {
        if(BPMS[pi + on - 1].size() >= 5) return;
        BPMS[pi + on - 1] += '.';
    }else if(keyCode == ALLEGRO_KEY_BACKSPACE) {
        if(BPMS[pi + on - 1].size()) BPMS[pi + on - 1].pop_back();
    }
    if(BPMS[pi + on - 1].empty() || std::stof(BPMS[pi + on - 1]) == 0.0) BPM[pi + on - 1] = 60;
    else BPM[pi + on - 1] = std::stof(BPMS[pi + on - 1]);
    show[on - 1]->Text = BPMS[pi + on - 1];
}
void EditScene::BackOnClick(){
    SaveOnClick();
    Engine::GameEngine::GetInstance().ChangeScene("main");
}
void EditScene::SaveOnClick(){
    std::ofstream fout;
    fout.open("../Resource/scores/" + filename+ ".loli", std::ios::trunc);
    fout << total << "\n";
    for(int i = 0; i < total; i++){
        fout << BPM[i] << " " << Note[i].size() << "\n";
        for(auto [type, ghost, len, at, speed] : Note[i]){
            fout << type << " " << ghost << " " << len << " " << at << " " << speed << "\n";
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
    BPM.push_back(BPM.back()), BPMS.push_back(tostring(BPM.back())), Note.push_back({});
    TIMES->Text = "Times : " + std::to_string(total);
}
void EditScene::DeleteOnClick() {
    if(total - 1 < 4) return;
    total--;
    BPM.pop_back(), BPMS.pop_back(), Note.pop_back();
    if(pi + 3 == total){
        pi = std::max(0, pi - 1);
        ClearNote(), DisplayNote();
    }
    TIMES->Text = "Times : " + std::to_string(total);
}
void EditScene::LineAddOnClick(int val){
    if(!(lpm + val) || lpm + val > 12) return;
    lpm += val;
    lineH = 240.0 / (float)lpm;
    ClearLine();
    DisplayLine();
    LPM->Text = "Lines : " + std::to_string(lpm);
}
void EditScene::LenAddOnClick(int val){
    if(!(len + val) || len + val > 4) return;
    len += val;
    if(~val) HoldNote[len - 1]->Visible = 1;
    else HoldNote[len]->Visible = 0;
}
void EditScene::SpeedAddOnClick(float val){
    if(speed + val <= 0 || speed + val > 5) return;
    speed += val;
    SPEED->Text = "Speed :" + tostring(speed);
}
void EditScene::PiAddOnClick(int val){
    on = 0;
    ClearNote();
    pi = std::max(0, std::min(pi + val, total - 4));
    DisplayNote();
    NOWAT->Text = "Times " + std::to_string(pi + 1) + " ~ " + std::to_string(pi + 4);
}
void EditScene::PlayOnClick(){
    if(last > 0) StopOnClick();
    float time = 0;
    if(pi + 4 < total) play = 1;
    last = past = 0;
    now = pi, front = 0;
    Boing.clear();
    ClearNote();
    DisplayNote();
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
    audio = AudioHelper::PlaySample("songs/" + songname + ".ogg", 0, user.setting.BGMVolume, time);
}
void EditScene::PlayHeadOnClick(){
    if(last > 0) StopOnClick();
    last = past = 0;
    now = front = 0;
    Boing.clear();
    ClearNote();
    DisplayNote();
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
    audio = AudioHelper::PlaySample("songs/" + songname + ".ogg", 0, user.setting.BGMVolume, 0);
}
void EditScene::StopOnClick(){
    last = 0;
    Boing.clear();
    AudioHelper::StopSample(audio);
    RunningLine->Position.y = 960;
    RunningLine->Visible = 0;
}
void EditScene::TapOnClick(){
    hold = 0;
    imgTarget.push_back(new Engine::Image("play/sangoodtap.png", Engine::GameEngine::GetInstance().GetScreenSize().x - 410, Engine::GameEngine::GetInstance().GetScreenSize().y - 745, 180, 180, 0.5, 0.5));
    addObject(1, imgTarget[0]);
}
void EditScene::HoldOnClick(){
    hold = 1;
    imgTarget.clear();
    for(int i = 0; i < len; i++){
        imgTarget.push_back(new Engine::Image("play/sangoodhold.png", Engine::GameEngine::GetInstance().GetScreenSize().x - 140, Engine::GameEngine::GetInstance().GetScreenSize().y - 745 - i * 60, 180, 180, 0.5, 0.5));
        addObject(1, imgTarget[i]);
    }
}
void EditScene::ReadScore(){
    BPM.clear(), BPMS.clear(), Note.clear();
    std::string file = "../Resource/scores/" + filename + ".loli";
    std::ifstream fin(file);
    int time, notes, type, ghost, len;
    float at, speed, bpm;
    fin >> total;
    for(int i = 0; i < total; i++){
        if(!(fin >> bpm >> notes)) break;
        BPM.push_back(bpm), BPMS.push_back(tostring(bpm)), Note.push_back({});
        for(int j = 0; j < notes; j++){
            fin >> type >> ghost >> len >> at >> speed;
            Note[i].push_back({type, ghost, len, at, speed});
        }
    }
    fin.close();
    while(total < 4){
        total++;
        BPM.push_back(60), BPMS.push_back("60"), Note.push_back({});
    }
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
    int y = (onField[n].ff - pi) * 240 + round(onField[n].ss.at * 240.0);
    if(onField[n].ss.type) update(onField[n].ss.ghost, 1, 0, 1023, y, y + (onField[n].ss.len - 1) * 60, -1);
    else update(onField[n].ss.ghost, 1, 0, 1023, y, y, -1);
    RemoveControlObject(NoteButtonCtrl[n]->GetControlIterator(), NoteButtonCtrl[n]->GetObjectIterator());
    for(Engine::IObject* img : NoteButtonObj[n]) RemoveObject(img->GetObjectIterator());
    NoteButtonCtrl[n] = nullptr, NoteButtonObj[n].clear();
}
void EditScene::AddNoteButton(note N, int x, int y){
    onField.push_back({pi + y / 240, N});
    if(N.type) update(N.ghost, 1, 0, 1023, y, y + (N.len - 1) * 60, 1);
    else update(N.ghost, 1, 0, 1023, y, y, 1);
    if(N.type) NoteButtonCtrl.push_back(new Engine::ImageButton("play/sangoodhold.png", "play/sangoodholdbomb.png", x - 90, 960 - y - 90, 180, 180));
    else NoteButtonCtrl.push_back(new Engine::ImageButton("play/sangoodtap.png", "play/sangoodtapbomb.png", x - 90, 960 - y - 90, 180, 180));
    NoteButtonCtrl.back()->SetOnClickCallback(std::bind(&EditScene::DeleteNoteClick, this, NoteButtonCtrl.size() - 1));
    AddNewControlObject(NoteButtonCtrl.back());
    NoteButtonObj.push_back({});
    for(int i = 1; i < N.len; i++){
        NoteButtonObj.back().push_back(new Engine::Image("play/sangoodhold.png", x, 960 - (y + i * 60), 180, 180, 0.5, 0.5));
        addObject(1, NoteButtonObj.back().back());
    }
}
void EditScene::DisplayNote(){
    NOWAT->Text = "Times " + std::to_string(pi + 1) + " ~ " + std::to_string(pi + 4);
    for(int i = pi; i < std::min(total, pi + 4); i++){
        if(i >= Note.size()) break;
        for(note N : Note[i]){
            AddNoteButton(N, x0 + N.ghost * ghostW + ghostW / 2, round(240.0 * ((float)(i - pi) + N.at)));
        }
    }
    for(int i = 0; i < 4; i++){
        show[i]->Text = BPMS[pi + i];
    }
}
void EditScene::DisplayLine(){
    Engine::Label* line;
    std::string foo, bar;
    for(int i = 0; i < 60; i++) foo += '_';
    for(int i = 0; i < 92; i++) bar += '_';
    for(float i = 960; i > 0; i -= lineH){
        line = new Engine::Label((int)round(i) % 240? bar : foo, "pirulen.ttf", (int)round(i) % (int)round((float)lpm * lineH)? 20 : 30, x0, (int)i, 255, 255, 255, 255, 0, 1);
        Line.push_back(line);
        addObject(1, line);
    }
}
void EditScene::ClearNote(){
    for(int i = 0; i < NoteButtonCtrl.size(); i++){
        if(NoteButtonCtrl[i]) DeleteNoteButton(i);
    }
    onField.clear();
    NoteButtonCtrl.clear(), NoteButtonObj.clear();
}
void EditScene::ClearLine(){
    for(Engine::Label* &line : Line){
        RemoveObject(line->GetObjectIterator());
    }
    Line.clear();
}
bool EditScene::CheckSpaceValid(note N, int y){
    if(N.type && y % 240 + (N.len - 1) * 60 > 240) return 0;
    if(N.type && query(N.ghost, 1, 0, 1023, y, y + (N.len - 1) * 60)) return 0;
    if(!N.type && query(N.ghost, 1, 0, 1023, y, y)) return 0;
    return 1;
}
void EditScene::ConstructUI(){
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x, h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    halfW = w / 2;
    halfH = h / 2;
    Engine::ImageButton* btn;
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 500, h - 85, 450, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", user.font, 36, w - 275, h - 45, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 500, h - 155, 450, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::SaveOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Save", user.font, 36, w - 275, h - 115, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 430, h - 225, 150, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::InsertOnClick, this, 1));
    AddNewControlObject(btn);
    show[0] = new Engine::Label("60", user.font, 36, w - 410, h - 185, 125,30,32, 255, 0, 0.5);
    AddNewObject(show[0]);
    SB[0] = new Engine::Label("BPM1", user.font, 36, w - 515, h - 185, 125,30,32, 255, 0, 0.5);
    AddNewObject(SB[0]);
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 190, h - 225, 150, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::InsertOnClick, this, 2));
    AddNewControlObject(btn);
    show[1] = new Engine::Label("60", user.font, 36, w - 170, h - 185, 125,30,32, 255, 0, 0.5);
    AddNewObject(show[1]);
    SB[1] = new Engine::Label("BPM2", user.font, 36, w - 275, h - 185, 125,30,32, 255, 0, 0.5);
    AddNewObject(SB[1]);
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 430, h - 295, 150, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::InsertOnClick, this, 3));
    AddNewControlObject(btn);
    show[2] = new Engine::Label("60", user.font, 36, w - 410, h - 255, 125,30,32, 255, 0, 0.5);
    AddNewObject(show[2]);
    SB[2] = new Engine::Label("BPM3", user.font, 36, w - 515, h - 255, 125,30,32, 255, 0, 0.5);
    AddNewObject(SB[2]);
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 190, h - 295, 150, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::InsertOnClick, this, 4));
    AddNewControlObject(btn);
    show[3] = new Engine::Label("60", user.font, 36, w - 170, h - 255, 125,30,32, 255, 0, 0.5);
    AddNewObject(show[3]);
    SB[3] = new Engine::Label("BPM4", user.font, 36, w - 275, h - 255, 125,30,32, 255, 0, 0.5);
    AddNewObject(SB[3]);
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 500, h - 365, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::DeleteOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("-", user.font, 36, w - 460, h - 325, 125,30,32, 255, 0.5, 0.5));
    TIMES = new Engine::Label("Times :" + std::to_string(total), user.font, 36, w - 400, h - 325, 125,30,32, 255, 0, 0.5);
    AddNewObject(TIMES);
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 130, h - 365, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::AddOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("+", user.font, 36, w - 90, h - 325, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 500, h - 435, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::LineAddOnClick, this, -1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("-", user.font, 36, w - 460, h - 395, 125,30,32, 255, 0.5, 0.5));
    LPM = new Engine::Label("Lines :1", user.font, 36, w - 400, h - 395, 125,30,32, 255, 0, 0.5);
    AddNewObject(LPM);
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 130, h - 435, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::LineAddOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("+", user.font, 36, w - 90, h - 395, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 530, h - 505, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::SpeedAddOnClick, this, -1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("-1", user.font, 36, w - 490, h - 465, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 450, h - 505, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::SpeedAddOnClick, this, -0.1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("-0.1", user.font, 36, w - 410, h - 465, 125,30,32, 255, 0.5, 0.5));
    SPEED = new Engine::Label("Speed :1", user.font, 36, w - 365, h - 465, 125,30,32, 255, 0, 0.5);
    AddNewObject(SPEED);
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 210, h - 505, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::SpeedAddOnClick, this, 0.1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("+0.1", user.font, 36, w - 170, h - 465, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 130, h - 505, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::SpeedAddOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("+1", user.font, 36, w - 90, h - 465, 125,30,32, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("+", user.font, 36, w - 90, h - 325, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.leftdirt, user.leftfloor, w - 500, h - 575, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::PiAddOnClick, this, -1));
    AddNewControlObject(btn);
    NOWAT = new Engine::Label("Times 1 ~ 4", user.font, 36, w - 400, h - 535, 125,30,32, 255, 0, 0.5);
    AddNewObject(NOWAT);
    btn = new Engine::ImageButton(user.rightdirt, user.rightfloor, w - 130, h - 575, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::PiAddOnClick, this, 1));
    AddNewControlObject(btn);
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 500, h - 645, 140, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::PlayHeadOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Start", user.font, 36, w - 430, h - 605, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 350, h - 645, 140, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::PlayOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", user.font, 36, w - 280, h - 605, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 200, h - 645, 140, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::StopOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stop", user.font, 36, w - 130, h - 605, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 230, h - 715, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::LenAddOnClick, this, -1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("-", user.font, 36, w - 190, h - 675, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, w - 130, h - 715, 80, 80);
    btn->SetOnClickCallback(std::bind(&EditScene::LenAddOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("+", user.font, 36, w - 90, h - 675, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("play/sangoodtap.png", "play/sangoodtap.png", w - 450, h - 825, 180, 180);
    btn->SetOnClickCallback(std::bind(&EditScene::TapOnClick, this));
    AddNewControlObject(btn);
    btn = new Engine::ImageButton("play/sangoodhold.png", "play/sangoodhold.png", w - 230, h - 825, 180, 180);
    btn->SetOnClickCallback(std::bind(&EditScene::HoldOnClick, this));
    AddNewControlObject(btn);
    HoldNote.push_back(nullptr);
    for(int i = 1; i < 4; i++){
        HoldNote.push_back(new Engine::Image("play/sangoodhold.png", w - 140, h - 735 - i * 60, 180, 180, 0.5, 0.5));
        HoldNote[i]->Visible = 0;
        AddNewObject(HoldNote[i]);
    }
    for(int i = 0; i < 7; i++){
        AddNewObject(new Engine::Image("stage-select/foo.png", x0 + ghostW * i, 957, 4, 957, 0.5, 1));
    }
}