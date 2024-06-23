#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <sstream>
#include <iomanip>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Slider.hpp"
#include "SettingsScene.hpp"
#include "MainScene.hpp"
#include "SongSelectScene.hpp"
#include "StartScene.h"

std::string tototostring(float f){
    std::stringstream SS;
    std::string S;
    SS << std::fixed << std::setprecision(1) << f;
    SS >> S;
    return S;
}
void SettingsScene::Initialize() {

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    btn = new Engine::ImageButton(user.leftdirt, user.leftfloor, 10, 10,75, 75);
    btn->SetOnClickCallback(std::bind(&SettingsScene::BackOnClick, this, pre));
    AddNewControlObject(btn);

    Slider *sliderBGM, *sliderSFX;
    sliderBGM = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4, 190);
    sliderBGM->SetOnValueChangedCallback(
            std::bind(&SettingsScene::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    AddNewObject(
            new Engine::Label("BGM: ", user.font, 28, 40 + halfW - 60 - 95, halfH - 50, 255, 255, 255, 255, 0.5,
                              0.5));
    sliderSFX = new Slider(40 + halfW - 95, halfH + 50 - 2, 190, 4, 190);
    sliderSFX->SetOnValueChangedCallback(
            std::bind(&SettingsScene::SFXSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderSFX);
    AddNewObject(
            new Engine::Label("SFX: ", user.font, 28, 40 + halfW - 60 - 95, halfH + 50, 255, 255, 255, 255, 0.5,
                              0.5));
    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    btn = new Engine::ImageButton(user.dirt, user.floor, halfW - 180, halfH + 200, 360, 180);
    btn->SetOnClickCallback(std::bind(&SettingsScene::StartOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back To Start", user.font, 48, halfW, halfH + 290, 125, 30, 32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, halfW - 300, halfH - 150, 80, 80);
    btn->SetOnClickCallback(std::bind(&SettingsScene::SpeedAddOnClick, this, -1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("-1", user.font, 36, halfW - 260, halfH - 110, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, halfW - 200, halfH - 150, 80, 80);
    btn->SetOnClickCallback(std::bind(&SettingsScene::SpeedAddOnClick, this, -0.1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("-0.1", user.font, 36, halfW - 160, halfH - 110, 125,30,32, 255, 0.5, 0.5));
    SPEED = new Engine::Label("Speed :" + tototostring(user.setting.fallspeed), user.font, 36, halfW - 100, halfH - 110, 125,30,32, 255, 0, 0.5);
    AddNewObject(SPEED);
    btn = new Engine::ImageButton(user.dirt, user.floor, halfW + 120, halfH - 150, 80, 80);
    btn->SetOnClickCallback(std::bind(&SettingsScene::SpeedAddOnClick, this, 0.1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("+0.1", user.font, 36, halfW + 160, halfH - 110, 125,30,32, 255, 0.5, 0.5));
    btn = new Engine::ImageButton(user.dirt, user.floor, halfW + 220, halfH - 150, 80, 80);
    btn->SetOnClickCallback(std::bind(&SettingsScene::SpeedAddOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("+1", user.font, 36, halfW + 260, halfH - 110, 125,30,32, 255, 0.5, 0.5));

    bgmInstance = AudioHelper::PlaySample("mainscene.ogg", true, user.setting.BGMVolume);
    sliderBGM->SetValue(user.setting.BGMVolume);
    sliderSFX->SetValue(user.setting.SFXVolume);
}

void SettingsScene::Terminate() {
    user.update();
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void SettingsScene::BackOnClick(int prescene) {
    if(!prescene){
        SongSelectScene* scene = dynamic_cast<SongSelectScene*>(Engine::GameEngine::GetInstance().GetScene("song-select"));
        scene->user = user;
        Engine::GameEngine::GetInstance().ChangeScene("song-select");
    }else {
        MainScene* scene = dynamic_cast<MainScene*>(Engine::GameEngine::GetInstance().GetScene("main"));
        scene->user = user;
        Engine::GameEngine::GetInstance().ChangeScene("main");
    }
}
void SettingsScene::StartOnClick(){
    StartScene* scene = dynamic_cast<StartScene*>(Engine::GameEngine::GetInstance().GetScene("start"));
    scene->user = user;
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void SettingsScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    user.setting.BGMVolume = value;
}

void SettingsScene::SFXSlideOnValueChanged(float value) {
    user.setting.SFXVolume = value;
}
void SettingsScene::SpeedAddOnClick(float val){
    if(user.setting.fallspeed + val <= 0 || user.setting.fallspeed + val > 30) return;
    user.setting.fallspeed += val;
    SPEED->Text = "Speed :" + tototostring(user.setting.fallspeed);
}
