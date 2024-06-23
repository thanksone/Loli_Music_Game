#include <functional>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "WinScene.hpp"
#include "MainScene.hpp"

std::string otstring(int x){
    std::string S;
    for(int i = 1e6; i; i /= 10){
        S += x / i + 48;
        x %= i;
    }
    return S;
}
std::string totostring(float x){
    std::string S;
    std::stringstream SS;
    SS << std::fixed << std::setprecision(2) << x;
    SS >> S;
    return S;
}
void WinScene::Initialize() {
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;

	Engine::ImageButton* btn;
	btn = new Engine::ImageButton(user.dirt, user.floor, w - 320, h - 120, 300, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this));
	AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Continue", user.font, 48, w - 170, h - 70, 0, 0, 0, 255, 0.5, 0.5));
    Engine::Image* img;
    img=new Engine::Image("songs/"+give.filename+".png", 600, halfH,720,720,0.5,0.5);
    AddNewObject(img);
    if(give.songlan=="english") {
        AddNewObject(new Engine::Label(give.songname, user.font, 60, 1500, halfH -360 , 225,180,182, 255, 1, 0));
    }
    else AddNewObject(new Engine::Label(give.songname, user.wind? "07ReallyScaryMinchotai.ttf" : "hanazomefont.ttf", 60, 1500, halfH - 360, 225,180,182, 255, 1, 0));
    AddNewObject(new Engine::Label(otstring(score), user.font, 108, 1500, halfH - 240, 255, 255, 255, 255, 1, 0));
    AddNewObject(new Engine::Label(totostring(acc * 100.0) + "%", user.font, 48, 1500, halfH - 90, 255, 255, 255, 255, 1, 0));
    AddNewObject(new Engine::Label(std::to_string(maxcombo), user.font, 48, 1500, halfH - 20, 255, 255, 255, 255, 1, 0));
    AddNewObject(new Engine::Label(std::to_string(perfect), user.font, 48, 1500, halfH + 60, 255, 255, 255, 255, 1, 0));
    AddNewObject(new Engine::Label(std::to_string(good), user.font, 48, 1500, halfH + 140, 255, 255, 255, 255, 1, 0));
    AddNewObject(new Engine::Label(std::to_string(bad), user.font, 48, 1500, halfH + 220, 255, 255, 255, 255, 1, 0));
    AddNewObject(new Engine::Label(std::to_string(miss), user.font, 48, 1500, halfH + 300, 255, 255, 255, 255, 1, 0));
    AddNewObject(new Engine::Label("MaxCombo : ", user.font, 48, 1100, halfH - 20, 255, 255, 255, 255, 0, 0));
    AddNewObject(new Engine::Label("Perfect : ", user.font, 48, 1100, halfH + 60, 255, 255, 255, 255, 0, 0));
    AddNewObject(new Engine::Label("Good : ", user.font, 48, 1100, halfH + 140, 255, 255, 255, 255, 0, 0));
    AddNewObject(new Engine::Label("Bad : ", user.font, 48, 1100, halfH + 220, 255, 255, 255, 255, 0, 0));
    AddNewObject(new Engine::Label("Miss : ", user.font, 48, 1100, halfH + 300, 255, 255, 255, 255, 0, 0));
    user.UpdateRecord(give.songname, score, acc);
    int total = perfect + good + bad + miss;
    user.ChangeSan((float)user.loli->san * (1.0 + (float)maxcombo / (float)total) * acc - (float)user.loli->san);
}
void WinScene::Terminate() {
	IScene::Terminate();
	AudioHelper::StopBGM(bgmId);
}
void WinScene::BackOnClick() {
    MainScene* scene = dynamic_cast<MainScene*>(Engine::GameEngine::GetInstance().GetScene("main"));
    scene->user = user;
	Engine::GameEngine::GetInstance().ChangeScene("main");
}