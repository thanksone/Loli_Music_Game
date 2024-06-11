#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "CharacterSelectScene.hpp"

bool cmp(character &a,character &b){
    return a.charactername<b.charactername;
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


    characterlist.clear();
    std::string charactername;
    std::ifstream fin("Resource/images/characters/characterlist.txt");
    //std::cout<<"ouob\n";
    while(fin>>charactername ){
        //std::cout<<"douo\n";
        characterlist.push_back(
            {charactername});
        std::cout<<charactername<<"\n";
    }
    maxpage= characterlist.size();
    fin.close();
    std::sort(characterlist.begin(),characterlist.end(),cmp);


    btn = new Engine::ImageButton("stage-select/blueleft.png", "stage-select/pinkleft.png", 10, 10,75, 75);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);


    btn = new Engine::ImageButton("stage-select/blueright.png", "stage-select/pinkright.png", w-150 , halfH-50, 100, 100);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::ChangeOnClick, this, 1));
    AddNewControlObject(btn);

    btn = new Engine::ImageButton("stage-select/blueleft.png", "stage-select/pinkleft.png", 50 , halfH-50, 100, 100);
    btn->SetOnClickCallback(std::bind(&CharacterSelectScene::ChangeOnClick, this, -1));
    AddNewControlObject(btn);
    if(fl==1) bgmInstance = AudioHelper::PlaySample("characterselect.ogg", true, AudioHelper::BGMVolume);
    fl=0;
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
void CharacterSelectScene::PlayOnClick(int stage) {
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    scene->MapId = stage;
    Engine::GameEngine::GetInstance().ChangeScene("play");
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

