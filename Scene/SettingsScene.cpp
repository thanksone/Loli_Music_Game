#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Slider.hpp"
#include "SettingsScene.hpp"

void SettingsScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 150, halfH * 3 / 2 - 50,
                                  300, 150);
    btn->SetOnClickCallback(std::bind(&SettingsScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW, halfH * 3 / 2+25, 125, 30, 32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 150, halfH * 3 / 2 +70,
                              300, 150);
    btn->SetOnClickCallback(std::bind(&SettingsScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("back to start", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW, halfH * 3 / 2+145, 125, 30, 32, 255, 0.5, 0.5));

    Slider *sliderBGM, *sliderSFX;
    sliderBGM = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
    sliderBGM->SetOnValueChangedCallback(
            std::bind(&SettingsScene::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    AddNewObject(
            new Engine::Label("BGM: ", "WOODCUTTER-BCN-Style-1.ttf", 28, 40 + halfW - 60 - 95, halfH - 50, 255, 255, 255, 255, 0.5,
                              0.5));
    sliderSFX = new Slider(40 + halfW - 95, halfH + 50 - 2, 190, 4);
    sliderSFX->SetOnValueChangedCallback(
            std::bind(&SettingsScene::SFXSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderSFX);
    AddNewObject(
            new Engine::Label("SFX: ", "WOODCUTTER-BCN-Style-1.ttf", 28, 40 + halfW - 60 - 95, halfH + 50, 255, 255, 255, 255, 0.5,
                              0.5));
    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("mainscene.ogg", true, AudioHelper::BGMVolume);
    sliderBGM->SetValue(AudioHelper::BGMVolume);
    sliderSFX->SetValue(AudioHelper::SFXVolume);
}

void SettingsScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void SettingsScene::BackOnClick(int stage) {
    if(stage==1) {
        Engine::GameEngine::GetInstance().ChangeScene("start");
    }
    else {
        Engine::GameEngine::GetInstance().ChangeScene("main");
    }

}

void SettingsScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}

void SettingsScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
