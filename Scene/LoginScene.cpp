#include <fstream>
#include "LoginScene.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "MainScene.hpp"

void LoginScene::Initialize(){
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    username.clear(), password.clear();
    std::string namae;
    long long pa55word;
    std::ifstream fin("../Resource/accountlist.txt");
    while(fin >> namae >> pa55word) mamamia[namae] = pa55word;
    fin.close();
    shift = 0;
    message = new Engine::Label("", "Black-Magic-2.ttf", 48, halfW, halfH - 250, 255, 255, 255, 255, 0.5, 0.5);
    usr = new Engine::Label("", "Black-Magic-2.ttf", 48, halfW - 40, halfH - 105, 0, 0, 0, 255, 0, 0.5);
    pwd = new Engine::Label("", "Black-Magic-2.ttf", 48, halfW - 40, halfH + 105, 0, 0, 0, 255, 0, 0.5);
    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png", halfW - 100, halfH - 195 , 500, 180);
    btn->SetOnClickCallback(std::bind(&LoginScene::InsertOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("User Name: ", "Black-Magic-2.ttf", 48, halfW - 120, halfH - 105, 255, 255, 255, 255, 1, 0.5));
    AddNewObject(usr);
    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png", halfW - 100, halfH + 15 , 500, 180);
    btn->SetOnClickCallback(std::bind(&LoginScene::InsertOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Password: ", "Black-Magic-2.ttf", 48, halfW - 120, halfH + 105, 255, 255, 255, 255, 1, 0.5));
    AddNewObject(pwd);
    btn = new Engine::ImageButton("stage-select/blueleft.png", "stage-select/pinkleft.png", 10, 10,75, 75);
    btn->SetOnClickCallback(std::bind(&LoginScene::BackOnClick, this));
    AddNewControlObject(btn);
    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png", halfW - 320, halfH + 225 , 300, 180);
    btn->SetOnClickCallback(std::bind(&LoginScene::RegisterOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Register", "Black-Magic-2.ttf", 48, halfW - 170, halfH + 315, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png", halfW + 20, halfH + 225 , 300, 180);
    btn->SetOnClickCallback(std::bind(&LoginScene::LoginOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Login", "Black-Magic-2.ttf", 48, halfW + 170, halfH + 315, 0, 0, 0, 255, 0.5, 0.5));
    AddNewObject(message);
}
void LoginScene::Terminate(){
    IScene::Terminate();
}
void LoginScene::OnMouseDown(int button, int mx, int my){
    on = 0;
    IScene::OnMouseDown(button, mx, my);
}
void LoginScene::OnKeyDown(int keyCode){
    std::string &S = (on == 1? username : password);
    if(keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) S += (char)(keyCode - ALLEGRO_KEY_A + 'a' + shift * ('A' - 'a'));
    else if(keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) S += (char)(keyCode - ALLEGRO_KEY_0 + '0');
    else if(keyCode == ALLEGRO_KEY_LSHIFT || keyCode == ALLEGRO_KEY_RSHIFT) shift = 1;
    else if(keyCode == ALLEGRO_KEY_MINUS) S += (shift? '_' : '-');
    else if(keyCode == ALLEGRO_KEY_FULLSTOP) S += '.';
    else if(keyCode == ALLEGRO_KEY_BACKSPACE){
        if(S.size()) S.pop_back();
    }else if(keyCode == ALLEGRO_KEY_ENTER){
        if(on == 1) on = 2;
        else if(on == 2) LoginOnClick();
    }
    usr->Text = username;
    if(password.size() >  pwd->Text.size()) pwd->Text += '*';
    else if(password.size() < pwd->Text.size()) pwd->Text.pop_back();
}
void LoginScene::OnKeyUp(int keyCode){
    if(keyCode == ALLEGRO_KEY_LSHIFT || keyCode == ALLEGRO_KEY_RSHIFT) shift = 0;
}
void LoginScene::LoginOnClick(){
    if(mamamia.find(username) == mamamia.end()) message->Text = "User does not exist";
    else if(mamamia[username] != hash(password)) message->Text = "User name or password incorrect";
    else{
        int n, san, fullsan, score, wind;
        float acc;
        std::string file = "../Resource/account-status/" + username + ".loli", name, songname;
        std::ifstream fin(file);
        user = User(username);
        fin >> n;
        while(n--){
            if(!(fin >> name >> san >> fullsan >> wind)) break;
            user.AddCharacter(Loli(name, san, fullsan, wind));
        }
        fin >> name;
        user.ChangeOnField(name);
        while(fin >> songname >> score >> acc){
            user.UpdateRecord(songname, score, acc);
        }
        fin.close();
        std::ofstream fout("../Resource/account-status/guest.loli", std::ios::trunc);
        fout << username;
        fout.close();
        MainScene* scene = dynamic_cast<MainScene*>(Engine::GameEngine::GetInstance().GetScene("main"));
        scene->user = user;
        Engine::GameEngine::GetInstance().ChangeScene("main");
    }
}
void LoginScene::InsertOnClick(int t){
    on = t;
}
void LoginScene::BackOnClick(){
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void LoginScene::RegisterOnClick(){
    Engine::GameEngine::GetInstance().ChangeScene("register");
}