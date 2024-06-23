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
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "CharacterSelectScene.hpp"
#include "MainScene.hpp"
#include "Character/Loli.hpp"

bool cmpn(Loli &a, Loli &b){
    return a.name<b.name;
}
bool cmps(Loli &a,Loli &b){
    return a.san>b.san;
}

int f=0,fl=1;

void CharacterSelectScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    std::string backgroundimg=user.wind?"scenes/sanbadcharacter.png":"scenes/sangoodcharacter.png";
    std::string boardimg=user.wind?"scenes/sanbadboard.png":"scenes/sangoodboard.png";
    std::string dirtimg=user.wind?"stage-select/sanbaddirt.png":"stage-select/sangooddirt.png";
    std::string floorimg=user.wind?"stage-select/sanbadfloor.png":"stage-select/sangoodfloor.png";
    Engine::Image* img;
    img=new Engine::Image(backgroundimg, halfW, halfH,1800,1020,0.5,0.5);
    addObject(1,img);


    Engine::ImageButton* btn;
    btn = new Engine::ImageButton(user.leftdirt, user.leftfloor, 10, 10,75, 75);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);

    int cmptype=1;
    characterlist.clear();
    std::string charactername,san,maxsan;
    std::ifstream fin("Resource/images/characters/characterlist.txt");
    while(fin>>charactername && fin>>san && fin>>maxsan){
        characterlist.push_back(
            Loli(charactername,std::stoi(san),std::stoi(maxsan), 0));
        //std::cout<<characterlist[0].charactername<<" "<<characterlist[0].san<<" "<<characterlist[0].maxsan<<"\n";
    }
    characterlist = user.Character;
    maxpage= characterlist.size();
    fin.close();

    if(cmptype==0)
        std::sort(characterlist.begin(),characterlist.end(),cmpn);
    else
        std::sort(characterlist.begin(),characterlist.end(),cmps);

    btn = new Engine::ImageButton(user.leftdirt, user.leftfloor, 10, 10,75, 75);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);


    btn = new Engine::ImageButton(user.rightdirt, user.rightfloor, w-150 , halfH-50, 100, 100);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::ChangeOnClick, this, 1));
    AddNewControlObject(btn);

    characterlist[page].draw(this,halfW,halfH-225,608,1064,0.5,0.5);

    btn = new Engine::ImageButton(user.leftdirt, user.leftfloor, 50 , halfH-50, 100, 100);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::ChangeOnClick, this, -1));
    AddNewControlObject(btn);
    if(fl==1) bgmInstance = AudioHelper::PlaySample("characterselect.ogg", true, AudioHelper::BGMVolume);
    fl=0;
    img=new Engine::Image(boardimg, halfW, halfH+225,400,400,0.5,0.5);
    addObject(1,img);
    std::string sanality=std::to_string(characterlist[page].san)+"/"+std::to_string(characterlist[page].fullsan);
    AddNewObject(new Engine::Label(characterlist[page].name, "Black-Magic-2.ttf", 60, halfW, halfH +200, 225,180,182, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label(sanality, "Black-Magic-2.ttf", 60, halfW, halfH +280, 225,180,182, 255, 0.5, 0.5));

    btn = new Engine::ImageButton(dirtimg, floorimg, halfW -150, h-195 , 300, 180);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::ChooseOnClick, this));
    AddNewControlObject(btn);
    if(characterlist[page].name==user.loli->name) {
        status = new Engine::Label("Using", "Black-Magic-2.ttf", 48, halfW, h-105, 125, 30, 32, 255, 0.5, 0.5);
        addObject(1,status);
    }
    else {
        status = new Engine::Label("Choose!", "Black-Magic-2.ttf", 48, halfW, h-105, 125, 30, 32, 255, 0.5, 0.5);
        addObject(1,status);
    }
    // Not safe if release resource while playing, however we only free while change scene, so it's fine.

}
void CharacterSelectScene::Terminate() {
    if(f==1) {
        AudioHelper::StopSample(bgmInstance);
        bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    }
	IScene::Terminate();
}
void CharacterSelectScene::BackOnClick(int stage) {
    f=1;
    fl=1;
    MainScene* scene = dynamic_cast<MainScene*>(Engine::GameEngine::GetInstance().GetScene("main"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("main");
}

void CharacterSelectScene::ChangeOnClick(int changeway) {
    f=0;
    if(page +changeway< 0){
        page=maxpage-1;
    }
    else if(page+changeway>=maxpage) {
        page=0;
    }
    else page+=changeway;
    Engine::GameEngine::GetInstance().ChangeScene("character-select");
}

void CharacterSelectScene::ChooseOnClick() {
    if(characterlist[page].name!=user.loli->name) {
        user.ChangeOnField(characterlist[page].name);
        status->Text = "Using";
    }

}


