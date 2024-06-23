#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>


#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "SongSelectScene.hpp"
#include "MainScene.hpp"
#include "ScoreboardScene.h"
#include "SettingsScene.hpp"

bool cmp1(song &a,song &b){
    return a.songname<b.songname;
}
std::string ototstring(int x){
    std::string S;
    for(int i = 1e6; i; i /= 10){
        S += x / i + 48;
        x %= i;
    }
    return S;
}
std::string otototstring(float x){
    std::string S;
    std::stringstream SS;
    SS << std::fixed << std::setprecision(2) << x;
    SS >> S;
    return S;
}
int cmptype = 1;
void SongSelectScene::Initialize() {

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;

    songlist.clear();
    std::string songname, filename, songlan;
    std::ifstream fin("Resource/audios/songs/songlist.txt");
    //std::cout<<"ouob\n";
    while(fin>>songname && fin >> filename && fin>>songlan){
        //std::cout<<"douo\n";
        songlist.push_back(
            {songname,filename, songlan});
        std::cout<<songname<<"\n";
    }
    maxpage= songlist.size();
    fin.close();
    if(cmptype==1) {
        std::sort(songlist.begin(),songlist.end(),cmp1);
    }

    btn = new Engine::ImageButton(user.leftdirt, user.leftfloor, 10, 10,75, 75);
    btn->SetOnClickCallback(std::bind(&SongSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);

    if(user.wind) btn = new Engine::ImageButton("stage-select/sanbadsettingdirt.png", "stage-select/sanbadsettingfloor.png", w-80, 10, 70, 70);
    else btn = new Engine::ImageButton("stage-select/sangoodsettingdirt.png", "stage-select/sangoodsettingfloor.png", w-80, 10, 70, 70);
    btn->SetOnClickCallback(std::bind(&SongSelectScene::SettingsOnClick, this));
    AddNewControlObject(btn);

    btn = new Engine::ImageButton(user.rightdirt, user.rightfloor, w-150 , halfH-50, 100, 100);
    btn->SetOnClickCallback(std::bind(&SongSelectScene::ChangeOnClick, this, 1));
    AddNewControlObject(btn);

    btn = new Engine::ImageButton(user.leftdirt, user.leftfloor, 50 , halfH-50, 100, 100);
    btn->SetOnClickCallback(std::bind(&SongSelectScene::ChangeOnClick, this, -1));
    AddNewControlObject(btn);

    btn = new Engine::ImageButton(user.dirt, user.floor, halfW - 550, h - 175, 300, 180);
    btn->SetOnClickCallback(std::bind(&SongSelectScene::PlayOnClick, this, songlist[page], "ez"));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("EZ", user.font, 48, halfW - 400, h - 85, 125,30,32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(user.dirt, user.floor, halfW - 150, h - 175, 300, 180);
    btn->SetOnClickCallback(std::bind(&SongSelectScene::PlayOnClick, this, songlist[page], "hd"));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("HD", user.font, 48, halfW, h - 85, 125,30,32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(user.dirt, user.floor, halfW + 250, h - 175, 300, 180);
    btn->SetOnClickCallback(std::bind(&SongSelectScene::PlayOnClick, this, songlist[page], "in"));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("IN", user.font, 48, halfW + 400, h - 85, 125,30,32, 255, 0.5, 0.5));

    AddNewObject(new Engine::Label(ototstring(user.BestRecord[songlist[page].songname].first), user.font, 60, w - 100, h - 400, 255, 255, 255, 255, 1, 0));
    AddNewObject(new Engine::Label(otototstring(user.BestRecord[songlist[page].songname].second * 100.0) + "%", user.font, 30, w - 100, h - 320, 255, 255, 255, 255, 1, 0));
    /*btn = new Engine::ImageButton(user.dirt, user.floor, halfW - 200, halfH / 2 + 250, 400, 150);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::ScoreboardOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Scoreboard", user.font, 36, halfW, halfH / 2 + 300, 125,30,32, 255, 0.5, 0.5));*/

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
	bgmInstance = AudioHelper::PlaySample("songs/"+songlist[page].filename+".ogg", true, user.setting.BGMVolume);
    Engine::Image* img;
    img=new Engine::Image("songs/"+songlist[page].filename+".png", halfW, halfH-100,720,720,0.5,0.5);
    addObject(1,img);
    if(songlist[page].songlan=="english") {
        AddNewObject(new Engine::Label(songlist[page].songname, user.font, 60, halfW, halfH +300, 225,180,182, 255, 0.5, 0.5));
    }
    else AddNewObject(new Engine::Label(songlist[page].songname, user.wind? "07ReallyScaryMinchotai.ttf" : "hanazomefont.ttf", 60, halfW, halfH +300, 225,180,182, 255, 0.5, 0.5));
}
void SongSelectScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void SongSelectScene::BackOnClick(int stage) {
    MainScene* scene = dynamic_cast<MainScene*>(Engine::GameEngine::GetInstance().GetScene("main"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("main");
}
void SongSelectScene::PlayOnClick(song Song, std::string diff) {
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    scene->give = Song, scene->diff = diff, scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void SongSelectScene::ScoreboardOnClick() {
    ScoreboardScene* scene = dynamic_cast<ScoreboardScene*>(Engine::GameEngine::GetInstance().GetScene("scoreboard"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}
void SongSelectScene::SettingsOnClick() {
    SettingsScene* scene = dynamic_cast<SettingsScene*>(Engine::GameEngine::GetInstance().GetScene("settings"));
    scene->user = user, scene->pre = 0;
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

void SongSelectScene::ChangeOnClick(int changeway) {
    if(page +changeway< 0){
        page=maxpage-1;
    }
    else if(page+changeway>=maxpage) {
        page=0;
    }
    else page+=changeway;
    Engine::GameEngine::GetInstance().ChangeScene("song-select");
}