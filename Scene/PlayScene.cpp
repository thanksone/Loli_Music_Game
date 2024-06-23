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

#include "WinScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Enemy/TapEnemy.hpp"
#include "Engine/Resources.hpp"
#include "Enemy/HoldEnemy.hpp"
#include "UI/Animation/OnlineJudge.hpp"

bool cmp(noto a, noto b){
    return a.em < b.em;
}
std::string tostring(int x){
    std::string S;
    for(int i = 1e6; i; i /= 10){
        S += x / i + 48;
        x %= i;
    }
    return S;
}
void PlayScene::Initialize() {
    std::cout << "PlayScene\n";
    perfect = good = bad = miss = 0;
    score = 0, acc = 0, combo = 0, maxcombo = 0, total = 0;
    x0 = 150, ghostW = 250;
    pause = 0;
    deadline = 960;
	ReadScore();
	Construct();
    curtime = 0;
    for(int i = 0; i < 12; i++) Hold[i] = 0;
    boing = 1;
    Song = AudioHelper::PlaySample("songs/" + give.filename + ".ogg", 0, AudioHelper::BGMVolume);
    last = AudioHelper::GetSampleLength(Song) + 1;
}
void PlayScene::Terminate() {
	AudioHelper::StopSample(Song);
	Song = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void PlayScene::Update(float deltaTime) {
    if(curtime >= last){
        WinScene* scene = dynamic_cast<WinScene*>(Engine::GameEngine::GetInstance().GetScene("win"));
        scene->user = user, scene->give = give;
        scene->score = score, scene->acc = acc, scene->perfect = perfect, scene->good = good, scene->bad = bad, scene->miss = miss, scene->maxcombo = maxcombo;
        Engine::GameEngine::GetInstance().ChangeScene("win");
    }
    if(pause >= 5) return;
    else if(pause <= 3 && pause >= 0){
        pause -= deltaTime;
        Left->Text = std::to_string((int)pause);
        return;
    }else if(pause <= 0 && !boing) {
        Left->Visible = 0;
        Song = AudioHelper::PlaySample("songs/" + give.filename + ".ogg", 0, AudioHelper::BGMVolume, curtime);
        boing = 1;
    }
    curtime += deltaTime;
    while(!Note.empty() && Note.front().em <= curtime){
        auto [type, ghost, at, em, speed] = Note.front();
        Note.pop();
        int change = 0;
        if(user.wind){
            change = rand() % user.loli->fullsan;
            change = change <= (user.loli->fullsan - user.loli->san) / 12;
        }
        change <<= 1;
        type += change;
        if(type & 1){
            NoteGroup[ghost][type]->AddNewObject(new HoldEnemy(x0 + ghost * ghostW + ghostW / 2, deadline + (curtime - at) * speed, type, ghost, speed));
        }else{
            NoteGroup[ghost][type]->AddNewObject(new TapEnemy(x0 + ghost * ghostW + ghostW / 2, deadline + (curtime - at) * speed, type, ghost, speed));
        }
    }
    for(int i = 0; i < 12; i++){
        if(Hold[i]) Line[i % 6]->Hit(2 * (i / 6) + 1);
    }
    IScene::Update(deltaTime);
}
void PlayScene::Draw() const {
	IScene::Draw();
}
void PlayScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
    if(keyCode == ALLEGRO_KEY_S) Hold[0] = 1, Line[0]->Down(), Line[0]->Hit(0);
    else if(keyCode == ALLEGRO_KEY_D) Hold[1] = 1, Line[1]->Down(), Line[1]->Hit(0);
    else if(keyCode == ALLEGRO_KEY_F) Hold[2] = 1, Line[2]->Down(), Line[2]->Hit(0);
    else if(keyCode == ALLEGRO_KEY_J) Hold[3] = 1, Line[3]->Down(), Line[3]->Hit(0);
    else if(keyCode == ALLEGRO_KEY_K) Hold[4] = 1, Line[4]->Down(), Line[4]->Hit(0);
    else if(keyCode == ALLEGRO_KEY_L) Hold[5] = 1, Line[5]->Down(), Line[5]->Hit(0);
    else if(keyCode == ALLEGRO_KEY_W) Hold[6] = 1, Line[0]->Down(), Line[0]->Hit(2);
    else if(keyCode == ALLEGRO_KEY_E) Hold[7] = 1, Line[1]->Down(), Line[1]->Hit(2);
    else if(keyCode == ALLEGRO_KEY_R) Hold[8] = 1, Line[2]->Down(), Line[2]->Hit(2);
    else if(keyCode == ALLEGRO_KEY_U) Hold[9] = 1, Line[3]->Down(), Line[3]->Hit(2);
    else if(keyCode == ALLEGRO_KEY_I) Hold[10] = 1, Line[4]->Down(), Line[4]->Hit(2);
    else if(keyCode == ALLEGRO_KEY_O) Hold[11] = 1, Line[5]->Down(), Line[5]->Hit(2);
}
void PlayScene::OnKeyUp(int keyCode){
    IScene::OnKeyUp(keyCode);
    if(keyCode == ALLEGRO_KEY_S) Hold[0] = 0, Line[0]->Up();
    else if(keyCode == ALLEGRO_KEY_D) Hold[1] = 0, Line[1]->Up();
    else if(keyCode == ALLEGRO_KEY_F) Hold[2] = 0, Line[2]->Up();
    else if(keyCode == ALLEGRO_KEY_J) Hold[3] = 0, Line[3]->Up();
    else if(keyCode == ALLEGRO_KEY_K) Hold[4] = 0, Line[4]->Up();
    else if(keyCode == ALLEGRO_KEY_L) Hold[5] = 0, Line[5]->Up();
    else if(keyCode == ALLEGRO_KEY_W) Hold[6] = 0, Line[0]->Up();
    else if(keyCode == ALLEGRO_KEY_E) Hold[7] = 0, Line[1]->Up();
    else if(keyCode == ALLEGRO_KEY_R) Hold[8] = 0, Line[2]->Up();
    else if(keyCode == ALLEGRO_KEY_U) Hold[9] = 0, Line[3]->Up();
    else if(keyCode == ALLEGRO_KEY_I) Hold[10] = 0, Line[4]->Up();
    else if(keyCode == ALLEGRO_KEY_O) Hold[11] = 0, Line[5]->Up();
}
void PlayScene::ReadScore() {
    std::ifstream fin("../Resource/scores/" + give.filename + "_" + diff + ".loli");
    float time = 0, bpm;
    int n, k, type, ghost, len;
    float at, speed;
    std::vector<noto> notes;
    fin >> n;
    for(int i = 0; i < n; i++){
        fin >> bpm >> k;
        while(k--) {
            fin >> type >> ghost >> len >> at >> speed;
            if(type){
                for (int j = 0; j < len; j++){
                    notes.push_back({type, ghost, time + at * (float)60 / bpm, time + at * (float)60 / bpm - (float)deadline / (speed * user.setting.fallspeed), speed * user.setting.fallspeed});
                    total++;
                }
            }else {
                notes.push_back({type, ghost, time + at * (float) 60 / bpm,time + at * (float) 60 / bpm - (float)deadline / (speed * user.setting.fallspeed),speed * user.setting.fallspeed});
                total++;
            }
        }
        time += 60.0 / bpm;
    }
    fin.close();
    std::sort(notes.begin(), notes.end(), cmp);
    while(!Note.empty()) Note.pop();
    for(noto N : notes) Note.push(N);
}
void PlayScene::Construct() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x, h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2, halfH = h / 2;
    for(int i = 0; i < 7; i++){
        AddNewObject(new Engine::Image("stage-select/foo.png", x0 + ghostW * i, 957, 4, 957, 0.5, 1));
    }
    Pause = new Engine::ImageButton("play/stop.png", "play/stop.png", 10, 10, 100, 100);
    Pause->SetOnClickCallback(std::bind(&PlayScene::PauseOnClick, this));
    AddNewControlObject(Pause);
    Back = new Engine::ImageButton("play/back.png", "play/back.png", halfW - 200, halfH, 100, 100, 0.5, 0.5);
    Back->SetOnClickCallback(std::bind(&PlayScene::BackOnClick, this));
    AddNewControlObject(Back);
    Back->Visible = 0;
    Restart = new Engine::ImageButton("play/restart.png", "play/restart.png", halfW, halfH, 100, 100, 0.5, 0.5);
    Restart->SetOnClickCallback(std::bind(&PlayScene::RestartOnClick, this));
    AddNewControlObject(Restart);
    Restart->Visible = 0;
    Continue = new Engine::ImageButton("play/continue.png", "play/continue.png", halfW + 200, halfH, 100, 100, 0.5, 0.5);
    Continue->SetOnClickCallback(std::bind(&PlayScene::ContinueOnClick, this));
    AddNewControlObject(Continue);
    Continue->Visible = 0;
    Left = new Engine::Label("", user.font, 108, halfW, halfH, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(Left);
    Left->Visible = 0;
    Score = new Engine::Label("0000000", user.font, 48, w - 30, 30, 255, 255, 255, 255, 1, 0);
    AddNewObject(Score);
    Combo = new Engine::Label("", user.font, 60, halfW, 30, 255, 255, 255, 255, 0.5, 0);
    AddNewObject(Combo);
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 4; j++){
            NoteGroup[i][j] = new Group;
            AddNewObject(NoteGroup[i][j]);
        }
        Line[i] = new Turret("play/turretup.png", "play/turretdown.png", x0 + ghostW * i + ghostW / 2 - 90, deadline - 90, i);
        AddNewObject(Line[i]);
    }
    EffectGroup = new Group;
    AddNewObject(EffectGroup);
}
void PlayScene::PauseOnClick(){
    AudioHelper::StopSample(Song);
    pause = 5;
    boing = 0;
    Back->Visible = 1, Restart->Visible = 1, Continue->Visible = 1, Pause->Visible = 0;
}
void PlayScene::BackOnClick(){
    Engine::GameEngine::GetInstance().ChangeScene("main");
}
void PlayScene::RestartOnClick(){
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void PlayScene::ContinueOnClick(){
    pause = 3;
    Back->Visible = 0, Restart->Visible = 0, Continue->Visible = 0, Pause->Visible = 1;
    Left->Text = "3", Left->Visible = 1;
}
void PlayScene::Hit(int type, int g){
    if(type == 1){
        perfect++, acc += 1.0 / (float)total, combo++;
        EffectGroup->AddNewObject(new OnlineJudge("Perfect", user.font, 0.2, x0 + g * ghostW + ghostW / 2, deadline - 80));
    }else if(type == 2){
        good++, acc += 0.65 / (float)total, combo++;
        EffectGroup->AddNewObject(new OnlineJudge("Good", user.font, 0.2, x0 + g * ghostW + ghostW / 2, deadline - 80));
    }else if(type == 3){
        bad++, combo = 0;
        EffectGroup->AddNewObject(new OnlineJudge("Bad", user.font, 0.2, x0 + g * ghostW + ghostW / 2, deadline - 80));
    }else{
        miss++, combo = 0;
        EffectGroup->AddNewObject(new OnlineJudge("Miss", user.font, 0.2, x0 + g * ghostW + ghostW / 2, deadline - 80));
    }
    maxcombo = std::max(maxcombo, combo);
    score = 9e5 * acc + 1e5 * (float)maxcombo / (float)total;
    Combo->Text = std::to_string(combo);
    Combo->Visible = combo;
    Score->Text = tostring((int)round(score));
}