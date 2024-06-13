#include "FileSelectScene.hpp"
#include "EditScene.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
void FileSelectScene::Initialize(){
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    filename.clear(), songname.clear();
    on = 0;
    Engine::ImageButton *btn;
    btn->SetOnClickCallback(std::bind(&FileSelectScene::EditOnClick, this));
    btn->SetOnClickCallback(std::bind(&FileSelectScene::InsertOnClick, this, 1));
    btn->SetOnClickCallback(std::bind(&FileSelectScene::InsertOnClick, this, 2));
    File = new Engine::Label("", "Black-Magic-2.ttf", 48, w-170, h-85, 125, 30, 32, 255, 0.5, 0.5));
    Song = new Engine::Label("", "Black-Magic-2.ttf", 48, w-170, h-85, 125, 30, 32, 255, 0.5, 0.5));
    AddNewObject(File), AddNewObject(Song);
    AddNewObject(new Engine::Label("Score : ", "Black-Magic-2.ttf", 48, w-170, h-85, 125, 30, 32, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("Audio : ", "Black-Magic-2.ttf", 48, w-170, h-85, 125, 30, 32, 255, 0.5, 0.5));
}
void FileSelectScene::Terminate(){
    IScene::Terminate();
}
void FileSelectScene::EditOnClick() {
    EditScene* scene = dynamic_cast<EditScene*>(Engine::GameEngine::GetInstance().GetScene("edit"));
    scene->filename = filename, scene->songname = songname;
    Engine::GameEngine::GetInstance().ChangeScene("edit");
}
void FileSelectScene::InsertOnClick(int t){
    on = t;
}
void FileSelectScene::OnKeyDown(int keyCode){
    IScene::OnKeyDown(keyCode);
}
void FileSelectScene::OnMouseDown(int button, int mx, int my) {
    on = 0;
    IScene::OnMouseDown(button, mx, my);
}