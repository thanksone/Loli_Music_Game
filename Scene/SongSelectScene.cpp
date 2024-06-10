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
#include "SongSelectScene.hpp"

void SongSelectScene::Initialize() {
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
    btn->SetOnClickCallback(std::bind(&SongSelectScene::SettingsOnClick, this));
    AddNewControlObject(btn);


    /*btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 150, halfH / 2 - 50, 300, 150);
    btn->SetOnClickCallback(std::bind(&MainScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage 1", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW, halfH / 2+25, 125,30,32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 150, halfH /2 + 100, 300, 150);
    btn->SetOnClickCallback(std::bind(&MainScene::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Stage 2", "WOODCUTTER-BCN-Style-1.ttf", 48, halfW, halfH / 2 +175, 125,30,32, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/sanbaddirt.png", "stage-select/sanbadfloor.png", halfW - 200, halfH / 2 + 250, 400, 150);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::ScoreboardOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Scoreboard", "WOODCUTTER-BCN-Style-1.ttf", 36, halfW, halfH / 2 + 300, 125,30,32, 255, 0.5, 0.5));*/

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
	bgmInstance = AudioHelper::PlaySample("mainscene.ogg", true, AudioHelper::BGMVolume);
}
void SongSelectScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void SongSelectScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("main");
}
void SongSelectScene::PlayOnClick(int stage) {
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    scene->MapId = stage;
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void SongSelectScene::ScoreboardOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}
void SongSelectScene::SettingsOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

