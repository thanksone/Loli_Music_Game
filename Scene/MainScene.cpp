#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "CharacterSelectScene.hpp"
#include "MainScene.hpp"
#include "FileSelectScene.hpp"
#include "SettingsScene.hpp"
#include "StartScene.h"
#include "ScoreboardScene.h"

void MainScene::Initialize() {
    f=0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    std::string backgroundimg=user.wind?"scenes/sanbadmain.png":"scenes/sangoodmain.png";
    Engine::Image* img;
    img=new Engine::Image(backgroundimg, halfW, halfH,1800,1020,0.5,0.5);
    addObject(0,img);
    //user.loli->draw(this,halfW+350,halfH-50,684,1197,0.5,0.5);
    btn=user.loli->touch(this,halfW+150,halfH-600,684,1197);
    btn->SetOnClickCallback(std::bind(&MainScene::TouchOnClick, this));
    AddNewControlObject(btn);

    if(user.wind) btn = new Engine::ImageButton("stage-select/sanbadsettingdirt.png", "stage-select/sanbadsettingfloor.png", w-80, 10, 70, 70);
    else btn = new Engine::ImageButton("stage-select/sangoodsettingdirt.png", "stage-select/sangoodsettingfloor.png", w-80, 10, 70, 70);
    btn->SetOnClickCallback(std::bind(&MainScene::SettingsOnClick, this));
    AddNewControlObject(btn);



    btn = new Engine::ImageButton(user.dirt, user.floor,  w- 320, h-175, 300, 180);
    btn->SetOnClickCallback(std::bind(&MainScene::EditOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Edit mode", user.font, 48, w-170, h-85, 125, 30, 32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(user.dirt, user.floor,  halfW-150, h-175, 300, 180);
    btn->SetOnClickCallback(std::bind(&MainScene::SongSelectOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Song", user.font, 48, halfW, h-85, 125, 30, 32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(user.dirt, user.floor,  300, h-175, 300, 180);
    btn->SetOnClickCallback(std::bind(&MainScene::CharacterSelectOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("character", user.font, 48, 450, h-85, 125, 30, 32, 255, 0.5, 0.5));

    /*btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 200, halfH / 2 + 250, 400, 150);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::ScoreboardOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Scoreboard", "WOODCUTTER-BCN-Style-1.ttf", 36, halfW, halfH / 2 + 300, 125,30,32, 255, 0.5, 0.5));*/

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
	bgmInstance = AudioHelper::PlaySample(user.wind? "sanbadmainscene.ogg" : "mainscene.ogg", true, user.setting.BGMVolume);
}
void MainScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void MainScene::BackOnClick(int stage) {
    StartScene* scene = dynamic_cast<StartScene*>(Engine::GameEngine::GetInstance().GetScene("start"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void MainScene::ScoreboardOnClick() {
    ScoreboardScene* scene = dynamic_cast<ScoreboardScene*>(Engine::GameEngine::GetInstance().GetScene("scoreboard"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}
void MainScene::SettingsOnClick() {
    SettingsScene* scene = dynamic_cast<SettingsScene*>(Engine::GameEngine::GetInstance().GetScene("settings"));
    scene->user = user, scene->pre = 1;
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

void MainScene::EditOnClick() {
    FileSelectScene* scene = dynamic_cast<FileSelectScene*>(Engine::GameEngine::GetInstance().GetScene("file-select"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("file-select");
}

void MainScene::SongSelectOnClick() {
    SongSelectScene* scene = dynamic_cast<SongSelectScene*>(Engine::GameEngine::GetInstance().GetScene("song-select"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("song-select");
}

void MainScene::CharacterSelectOnClick() {
    CharacterSelectScene* scene = dynamic_cast<CharacterSelectScene*>(Engine::GameEngine::GetInstance().GetScene("character-select"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("character-select");
}

void MainScene::TouchOnClick() {
    Engine::Image* img;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;


    if(f==0) {
        img=new Engine::Image(user.dirt, halfW-300, halfH,1200,600,0.5,0.5);
        addObject(1,img);
        talk = new Engine::Label(" ", "Black-Magic-2.ttf", 48, halfW-300, halfH, 125, 30, 32, 255, 0.5, 0.5);
        addObject(1,talk);
        f=1;
    }
    talk->Text=user.loli->talk();

}


std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> MainScene::Getbgm() {
    return bgmInstance;
}

