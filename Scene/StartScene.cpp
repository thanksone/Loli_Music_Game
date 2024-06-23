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
#include "MainScene.hpp"

std::string sancheck;

void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    std::string account, name, songname;
    int llcnt, san, fullsan, wind, score;
    float acc, bgm, sfx, speed;
    std::ifstream fout("../Resource/account-status/guest.loli");
    if(fout >> account && account != "guest"){
        user = User(account);
        fout.close();
        std::ifstream fin("../Resource/account-status/" + account + ".loli");
        fin >> bgm >> sfx >> speed;
        fin >> llcnt;
        while(llcnt--){
            if(!(fin >> name >> san >> fullsan >> wind)) break;
            user.AddCharacter(Loli(name, san, fullsan, wind));
        }
        fin >> name;
        user.ChangeOnField(name);
        while(fin >> songname >> score >> acc){
            user.UpdateRecord(songname, score, acc);
        }
        fin.close();
        user.setting = {bgm, sfx, speed};
    }else{
        user = Guest();
        fout.close();
    }
    sancheck = user.wind? "bad" : "good";
    //std::cout<<account<<" "<<sanity<<" "<<maxsan<<"\n";
    //std::cout<<account<<" "<<stoi(sanity)<<" "<<maxsan<<"\n";
    std::string backgroundimg="scenes/san"+sancheck+"start.png";
    std::string dirtimg="stage-select/san" +sancheck+"dirt.png";
    std::string floorimg="stage-select/san" +sancheck+"floor.png";
    std::string fontname;
    Engine::Image* img;
    img=new Engine::Image(backgroundimg, halfW, halfH,1800,1020,0.5,0.5);
    addObject(1,img);
    img=new Engine::Image("scenes/san"+sancheck+"board.png", halfW, 210,1400,500,0.5,0.5);
    addObject(1,img);

    if(sancheck=="bad"){
        fontname="Raslani-Kavaliar-Kaiser-1.ttf";
        AddNewObject(new Engine::Label("Abyssal Tunes", fontname, 120, halfW, halfH / 3 + 50, 125,30,32, 255, 0.5, 0.5));
        bgmInstance = AudioHelper::PlaySample("sanbadstartscene.ogg", true, user.setting.BGMVolume);
    }
    else{
        fontname="Black-Magic-2.ttf";
        AddNewObject(new Engine::Label("Heartbeat Rhythmo", fontname, 120, halfW, halfH / 3 + 50, 225,130,132, 255, 0.5, 0.5));
        bgmInstance = AudioHelper::PlaySample("startscene.ogg", true, user.setting.BGMVolume);
    }


    btn = new Engine::ImageButton(dirtimg, floorimg, halfW -150, h-195 , 300, 180);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", fontname, 48, halfW, h-105, 125, 30, 32, 255, 0.5, 0.5));



    btn = new Engine::ImageButton(dirtimg, floorimg,  halfW - 150, h-390, 300, 180);
    btn->SetOnClickCallback(std::bind(&StartScene::LoginOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Login", fontname, 48, halfW, h-300, 125, 30, 32, 255, 0.5, 0.5));

}
void StartScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void StartScene::PlayOnClick(int stage) {
    MainScene* scene = dynamic_cast<MainScene*>(Engine::GameEngine::GetInstance().GetScene("main"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("main");
}


void StartScene::LoginOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("login");
}
