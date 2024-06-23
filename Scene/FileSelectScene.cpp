#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
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
#include "FileSelectScene.hpp"
#include "EditScene.hpp"
#include "SettingsScene.hpp"
#include "MainScene.hpp"

bool comp(song &a,song &b){
    return a.songname<b.songname;
}
void FileSelectScene::Initialize() {

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
        songlist.push_back({songname,filename, songlan});
        //std::cout<<songname<<"\n";
    }
    maxpage= songlist.size();
    fin.close();
    sort(songlist.begin(), songlist.end(), comp);


    btn = new Engine::ImageButton(user.leftdirt, user.leftfloor, 10, 10,75, 75);
    btn->SetOnClickCallback(std::bind(&FileSelectScene::BackOnClick, this));
    AddNewControlObject(btn);

    if(user.wind) btn = new Engine::ImageButton("stage-select/sanbadsettingdirt.png", "stage-select/sanbadsettingfloor.png", w-80, 10, 70, 70);
    else btn = new Engine::ImageButton("stage-select/sangoodsettingdirt.png", "stage-select/sangoodsettingfloor.png", w-80, 10, 70, 70);
    btn->SetOnClickCallback(std::bind(&FileSelectScene::SettingsOnClick, this));
    AddNewControlObject(btn);

    btn = new Engine::ImageButton(user.rightdirt, user.rightfloor, w-150 , halfH-50, 100, 100);
    btn->SetOnClickCallback(std::bind(&FileSelectScene::AboriginalOnClick, this, 1));
    AddNewControlObject(btn);

    btn = new Engine::ImageButton(user.leftdirt, user.leftfloor, 50 , halfH-50, 100, 100);
    btn->SetOnClickCallback(std::bind(&FileSelectScene::AboriginalOnClick, this, -1));
    AddNewControlObject(btn);

    btn = new Engine::ImageButton(user.dirt, user.floor, halfW - 550, h - 175, 300, 180);
    btn->SetOnClickCallback(std::bind(&FileSelectScene::EditOnClick, this, songlist[page].filename, "ez"));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("EZ", user.font, 48, halfW - 400, h - 85, 125,30,32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(user.dirt, user.floor, halfW - 150, h - 175, 300, 180);
    btn->SetOnClickCallback(std::bind(&FileSelectScene::EditOnClick, this, songlist[page].filename, "hd"));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("HD", user.font, 48, halfW, h - 85, 125,30,32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(user.dirt, user.floor, halfW + 250, h - 175, 300, 180);
    btn->SetOnClickCallback(std::bind(&FileSelectScene::EditOnClick, this, songlist[page].filename, "in"));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("IN", user.font, 48, halfW + 400, h - 85, 125,30,32, 255, 0.5, 0.5));

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("songs/" + songlist[page].filename+".ogg", true, user.setting.BGMVolume);
    Engine::Image* img;
    img=new Engine::Image("songs/"+songlist[page].filename+".png", halfW, halfH-100,720,720,0.5,0.5);
    addObject(1,img);
    if(songlist[page].songlan=="english") {
        AddNewObject(new Engine::Label(songlist[page].songname, user.font, 60, halfW, halfH +300, 225,180,182, 255, 0.5, 0.5));
    }
    else AddNewObject(new Engine::Label(songlist[page].songname, user.wind? "07ReallyScaryMinchotai.ttf" : "hanazomefont.ttf", 60, halfW, halfH +300, 225,180,182, 255, 0.5, 0.5));
}
void FileSelectScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void FileSelectScene::BackOnClick() {
    MainScene* scene = dynamic_cast<MainScene*>(Engine::GameEngine::GetInstance().GetScene("main"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("main");
}
void FileSelectScene::EditOnClick(std::string songname, std::string diff) {
    EditScene* scene = dynamic_cast<EditScene*>(Engine::GameEngine::GetInstance().GetScene("edit"));
    scene->songname = songname, scene->filename = songname + "_" + diff, scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("edit");
}
void FileSelectScene::SettingsOnClick() {
    SettingsScene* scene = dynamic_cast<SettingsScene*>(Engine::GameEngine::GetInstance().GetScene("settings"));
    scene->user = user, scene->pre = 0;
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

void FileSelectScene::AboriginalOnClick(int square) {
    if(page +square< 0){
        page=maxpage-1;
    }
    else if(page+square>=maxpage) {
        page=0;
    }
    else page+=square;
    Engine::GameEngine::GetInstance().ChangeScene("file-select");
}