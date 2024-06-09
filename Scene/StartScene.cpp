//
// Created by Hsuan on 2024/4/10.
//

#include "StartScene.h"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "Scene/StartScene.h"

std::string sancheck;

void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    std::string account,sanity,maxsan;
    std::string filename="Resource/account-status/guest.txt";
    std::ifstream fin(filename);

    fin>>account>>sanity>>maxsan;
    if(std::stoi(sanity)<std::stoi(maxsan)/5) {
        sancheck="bad";
    }
    else sancheck="good";
    //std::cout<<account<<" "<<sanity<<" "<<maxsan<<"\n";
    //std::cout<<account<<" "<<stoi(sanity)<<" "<<maxsan<<"\n";

    fin.close();
    std::string dirtimg="stage-select/san" +sancheck+"dirt.png";
    std::string floorimg="stage-select/san" +sancheck+"floor.png";
    std::string fontname;
    if(sancheck=="bad"){
        fontname="Raslani-Kavaliar-Kaiser-1.ttf";
        AddNewObject(new Engine::Label("Abyssal Tunes", fontname, 120, halfW, halfH / 3 + 50, 125,30,32, 255, 0.5, 0.5));
    }
    else{
        fontname="Black-Magic-2.ttf";
        AddNewObject(new Engine::Label("Heartbeat Rhythmo", fontname, 120, halfW, halfH / 3 + 50, 225,180,182, 255, 0.5, 0.5));
    }


    btn = new Engine::ImageButton(dirtimg, floorimg, halfW -150, halfH / 2 + 200 , 300, 150);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", fontname, 48, halfW, halfH / 2 + 275, 125, 30, 32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(dirtimg, floorimg, w-300, 0, 300, 150);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Settings", fontname, 48, w-150, 75, 125, 30, 32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(dirtimg, floorimg,  w- 300, halfH * 3 / 2 +50, 300, 150);
    btn->SetOnClickCallback(std::bind(&StartScene::EditOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Edit mode", fontname, 48, w-150, halfH * 3 / 2+125, 125, 30, 32, 255, 0.5, 0.5));

}
void StartScene::Terminate() {
    IScene::Terminate();
}
void StartScene::PlayOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void StartScene::SettingsOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

void StartScene::EditOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("edit");
}
