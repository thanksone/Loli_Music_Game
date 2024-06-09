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
    sancheck="good";
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
    if(sancheck=="bad")
        AddNewObject(new Engine::Label("Abyssal Tunes", "WOODCUTTER-BCN-Style-1.ttf", 120, halfW, halfH / 3 + 50, 125,30,32, 255, 0.5, 0.5));
    else
        AddNewObject(new Engine::Label("Abyssal Tunes", "WOODCUTTER-BCN-Style-1.ttf", 120, halfW, halfH / 3 + 50, 125,30,32, 255, 0.5, 0.5));



    btn = new Engine::ImageButton(dirtimg, floorimg, halfW - 200, halfH / 2 + 200 , 400, 150);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW, halfH / 2 + 275, 125, 30, 32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(dirtimg, floorimg, halfW - 200, halfH * 3 / 2 - 50, 400, 150);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Settings", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW, halfH * 3 / 2+25, 125, 30, 32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(dirtimg, floorimg,  w- 450, halfH * 3 / 2 +50, 400, 150);
    btn->SetOnClickCallback(std::bind(&StartScene::EditOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Edit mode", "WOODCUTTER-BCN-Style-1.ttf", 48, w-250, halfH * 3 / 2+100+25, 125, 30, 32, 255, 0.5, 0.5));

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
