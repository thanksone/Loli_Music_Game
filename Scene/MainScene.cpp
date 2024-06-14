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
#include "MainScene.hpp"

void MainScene::Initialize() {

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    /*btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 150, halfH * 3 / 2 - 50, 300, 150);
    btn->SetOnClickCallback(std::bind(&MainScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW, halfH * 3 / 2+25, 125,30,32, 255, 0.5, 0.5));*/

    btn = new Engine::ImageButton("stage-select/sangoodsettingdirt.png", "stage-select/sangoodsettingfloor.png", w-80, 10, 70, 70);
    btn->SetOnClickCallback(std::bind(&MainScene::SettingsOnClick, this));
    AddNewControlObject(btn);

    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png",  w- 320, h-175, 300, 180);
    btn->SetOnClickCallback(std::bind(&MainScene::EditOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Edit mode", "Black-Magic-2.ttf", 48, w-170, h-85, 125, 30, 32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png",  halfW-150, h-175, 300, 180);
    btn->SetOnClickCallback(std::bind(&MainScene::SongSelectOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Song", "Black-Magic-2.ttf", 48, halfW, h-85, 125, 30, 32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png",  300, h-175, 300, 180);
    btn->SetOnClickCallback(std::bind(&MainScene::CharacterSelectOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("character", "Black-Magic-2.ttf", 48, 450, h-85, 125, 30, 32, 255, 0.5, 0.5));

    /*btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 200, halfH / 2 + 250, 400, 150);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::ScoreboardOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Scoreboard", "WOODCUTTER-BCN-Style-1.ttf", 36, halfW, halfH / 2 + 300, 125,30,32, 255, 0.5, 0.5));*/

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
	bgmInstance = AudioHelper::PlaySample("mainscene.ogg", true, AudioHelper::BGMVolume);
}
void MainScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void MainScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void MainScene::PlayOnClick(int stage) {
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    scene->MapId = stage;
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void MainScene::ScoreboardOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}
void MainScene::SettingsOnClick() {

    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

void MainScene::EditOnClick() {

    Engine::GameEngine::GetInstance().ChangeScene("file-select");
}

void MainScene::SongSelectOnClick() {

    Engine::GameEngine::GetInstance().ChangeScene("song-select");
}

void MainScene::CharacterSelectOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("character-select");
}

std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> MainScene::Getbgm() {
    return bgmInstance;
}

