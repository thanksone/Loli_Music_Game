#include <functional>
#include <string>
#include <fstream>
#include <ctime>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "WinScene.hpp"


std::string current() {
	time_t now = time(0);
	char buf[80];
	tm  ts = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y/%m/%d,%X", &ts);
	return buf;
}

Engine::Label *namae;
void WinScene::Initialize() {
	ticks = 0;
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;

	AddNewObject(new Engine::Image("stage-select/bar.png", halfW-100, halfH/4+80, 400, 0, 0.5, 1));
	AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 -10, 255, 255, 255, 255, 0.5, 0.5));
	AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	
	namae= new Engine::Label ("Your name?", "pirulen.ttf", 48, halfW-200, halfH / 4 +50, 200, 200, 200, 255, 0, 0.5);
	AddNewObject(namae);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
	name.clear();
	bgmId = AudioHelper::PlayAudio("win.wav");
}
void WinScene::Terminate() {
	IScene::Terminate();
	AudioHelper::StopBGM(bgmId);
}
void WinScene::BackOnClick(int stage) {
	// Change to select scene.
	std::ofstream fout;
	fout.open("Resource/scoreboard.txt", std::ios_base::app);
	if(name.size()) {
		fout<<name<<" "<<score<<" "<<current()<<"\n";
	}
	fout.close();
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void WinScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
		name+=('0'+keyCode-ALLEGRO_KEY_0);
	}
	else if(keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) {
		name+=('A'+keyCode-ALLEGRO_KEY_A);
	}
	else if(keyCode == ALLEGRO_KEY_BACKSPACE) {
		if(name.size()) {
			name.pop_back();
		}
	}
	namae->Text = name;
}