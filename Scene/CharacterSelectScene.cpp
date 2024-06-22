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

bool cmpn(character &a,character &b){
    return a.charactername<b.charactername;
}
bool cmps(character &a,character &b){
    return a.san>b.san;
}

int f=0,fl=1;

void CharacterSelectScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/blueleft.png", "stage-select/pinkleft.png", 10, 10,75, 75);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);

    int cmptype=1;
    characterlist.clear();
    std::string charactername,san,maxsan;
    std::ifstream fin("Resource/images/characters/characterlist.txt");
    while(fin>>charactername && fin>>san && fin>>maxsan){
        characterlist.push_back(
            {charactername,std::stoi(san),std::stoi(maxsan)});
        //std::cout<<characterlist[0].charactername<<" "<<characterlist[0].san<<" "<<characterlist[0].maxsan<<"\n";
    }
    maxpage= characterlist.size();
    fin.close();
    if(cmptype==0)
        std::sort(characterlist.begin(),characterlist.end(),cmpn);
    else
        std::sort(characterlist.begin(),characterlist.end(),cmps);

    btn = new Engine::ImageButton("stage-select/blueleft.png", "stage-select/pinkleft.png", 10, 10,75, 75);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);


    btn = new Engine::ImageButton("stage-select/blueright.png", "stage-select/pinkright.png", w-150 , halfH-50, 100, 100);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::ChangeOnClick, this, 1));
    AddNewControlObject(btn);
    Engine::Image* img;
    img=new Engine::Image("characters/"+characterlist[page].charactername+".png", halfW, halfH-100,720,720,0.5,0.5);
    addObject(1,img);

    btn = new Engine::ImageButton("stage-select/blueleft.png", "stage-select/pinkleft.png", 50 , halfH-50, 100, 100);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::ChangeOnClick, this, -1));
    AddNewControlObject(btn);
    if(fl==1) bgmInstance = AudioHelper::PlaySample("characterselect.ogg", true, AudioHelper::BGMVolume);
    fl=0;
    std::string sanality=std::to_string(characterlist[page].san)+"/"+std::to_string(characterlist[page].maxsan);
    AddNewObject(new Engine::Label(characterlist[page].charactername, "Black-Magic-2.ttf", 60, halfW, halfH +300, 225,180,182, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label(sanality, "Black-Magic-2.ttf", 60, halfW, halfH +380, 225,180,182, 255, 0.5, 0.5));
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

