//
// Created by Hsuan on 2024/4/10.
//
#include <iostream>
#include "StartScene.h"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "Scene/ScoreboardScene.h"

bool cmp(people &a,people &b){
    if(a.score==b.score){
        if(a.time==b.time) {
            return a.name<b.name;
        }
        return a.time<b.time;
    }
    return a.score>b.score;
}

void ScoreboardScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    int score;
    std::string name, time;
    record.clear();    
    std::ifstream fin("Resource/scoreboard.txt");
    while(fin>>name && fin>>score && fin>>time){
        record.push_back({name,score,time});
    }
    maxpage= record.size()/10 + (record.size()%10?1:0);
    fin.close();
    std::sort(record.begin(),record.end(),cmp);
    /*for(auto [n, s, m, d, h, i] : record) {
        std::cout << n << " " <<s<<" "<<month<<"/"<<d<<" "<<h<<":"<<i<<"\n";
    }*/
    AddNewObject(new Engine::Label("SCOREBOARD", "pirulen.ttf", 120, halfW, halfH / 3-50, 10, 255, 255, 255, 0.5, 0.5));
	
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW + 300, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::ChangeOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 48, halfW + 500, halfH*7/4, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 700, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::ChangeOnClick, this, -1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 48, halfW - 500, halfH *7/4, 0, 0, 0, 255, 0.5, 0.5));

    std::string S4=std::to_string(page+1)+"/"+std::to_string(maxpage);
    AddNewObject(new Engine::Label(S4, "OpenSans-ExtraBold.ttf", 40, halfW-100, halfH+200, 255, 255, 255, 255, 0, 0.5));

    for(int i = page * 10; i < std::min((int)record.size(), (page + 1) * 10); i++){
        std::string S1 = record[i].name;
        std::string S2=std::to_string(record[i].score);
        std::string S3=record[i].time;
        AddNewObject(new Engine::Label(S1, "OpenSans-ExtraBold.ttf", 40, 100, halfH / 2 + 40 * (i % 10), 155, 225, 255, 255, 0, 0.5));
        if(record[i].score>10000) {
            AddNewObject(new Engine::Label(S2, "OpenSans-ExtraBold.ttf", 40, 800, halfH / 2 + 40 * (i % 10), (record[i].score-10000)/1000+155>255?255:(record[i].score-10000)/1000+155, (record[i].score-10000)/1000+225>255?255:(record[i].score-10000)/1000+225, (record[i].score-10000)/1000>255?0:255-(record[i].score-10000)/1000, 255, 1, 0.5));
        }
        else {
            AddNewObject(new Engine::Label(S2, "OpenSans-ExtraBold.ttf", 40, 800, halfH / 2 + 40 * (i % 10), 155, 225, 255, 255, 1, 0.5));
        }
        AddNewObject(new Engine::Label(S3, "OpenSans-ExtraBold.ttf", 40, 1000, halfH / 2 + 40 * (i % 10), 155, 225, 255, 255, 0, 0.5));


    }
}
void ScoreboardScene::Terminate() {
    IScene::Terminate();
}
void ScoreboardScene::ChangeOnClick(int changeway) {
    if(page +changeway< 0|| page+changeway>=maxpage){
        return;
    }
    page+=changeway;
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}

void ScoreboardScene::BackOnClick(int stage) {
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}