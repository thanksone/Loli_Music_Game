#include <fstream>
#include "RegisterScene.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "MainScene.hpp"
#include "LoginScene.hpp"
void RegisterScene::Initialize(){
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    username.clear(), password1.clear(), password2.clear();
    std::string namae;
    long long pa55word;
    std::ifstream fin("Resource/accountlist.txt");
    while(fin >> namae >> pa55word) ExistUser.insert(namae);
    fin.close();
    shift = 0;
    message = new Engine::Label("", "Black-Magic-2.ttf", 48, halfW, halfH - 400, 255, 255, 255, 255, 0.5, 0.5);
    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png", halfW - 100, halfH - 295 , 500, 180);
    btn->SetOnClickCallback(std::bind(&RegisterScene::InsertOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("User Name: ", "Black-Magic-2.ttf", 48, halfW - 120, halfH - 205, 255, 255, 255, 255, 1, 0.5));
    usr = new Engine::Label("", "Black-Magic-2.ttf", 48, halfW - 40, halfH - 205, 0, 0, 0, 255, 0, 0.5);
    AddNewObject(usr);
    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png", halfW - 100, halfH - 90 , 500, 180);
    btn->SetOnClickCallback(std::bind(&RegisterScene::InsertOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Password: ", "Black-Magic-2.ttf", 48, halfW - 120, halfH, 255, 255, 255, 255, 1, 0.5));
    pwd1 = new Engine::Label("", "Black-Magic-2.ttf", 48, halfW - 40, halfH, 0, 0, 0, 255, 0, 0.5);
    AddNewObject(pwd1);
    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png", halfW - 100, halfH + 115 , 500, 180);
    btn->SetOnClickCallback(std::bind(&RegisterScene::InsertOnClick, this, 3));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Confirm Password: ", "Black-Magic-2.ttf", 48, halfW - 120, halfH + 205, 255, 255, 255, 255, 1, 0.5));
    pwd2 = new Engine::Label("", "Black-Magic-2.ttf", 48, halfW - 40, halfH + 205, 0, 0, 0, 255, 0, 0.5);
    AddNewObject(pwd2);
    btn = new Engine::ImageButton("stage-select/blueleft.png", "stage-select/pinkleft.png", 10, 10,75, 75);
    btn->SetOnClickCallback(std::bind(&RegisterScene::BackOnClick, this));
    AddNewControlObject(btn);
    btn = new Engine::ImageButton("stage-select/sangooddirt.png", "stage-select/sangoodfloor.png", halfW - 150, halfH + 310 , 300, 180);
    btn->SetOnClickCallback(std::bind(&RegisterScene::RegisterOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Register", "Black-Magic-2.ttf", 48, halfW, halfH + 400, 0, 0, 0, 255, 0.5, 0.5));
    AddNewObject(message);
}
void RegisterScene::Terminate(){
    IScene::Terminate();
}
void RegisterScene::OnMouseDown(int button, int mx, int my){
    on = 0;
    IScene::OnMouseDown(button, mx, my);
}
void RegisterScene::OnKeyDown(int keyCode){
    std::string &S = (on == 1? username : (on == 2? password1 : password2));
    if(keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) S += (char)(keyCode - ALLEGRO_KEY_A + 'a' + shift * ('A' - 'a'));
    else if(keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) S += (char)(keyCode - ALLEGRO_KEY_0 + '0');
    else if(keyCode == ALLEGRO_KEY_LSHIFT || keyCode == ALLEGRO_KEY_RSHIFT) shift = 1;
    else if(keyCode == ALLEGRO_KEY_MINUS) S += (shift? '_' : '-');
    else if(keyCode == ALLEGRO_KEY_FULLSTOP) S += '.';
    else if(keyCode == ALLEGRO_KEY_BACKSPACE){
        if(S.size()) S.pop_back();
    }else if(keyCode == ALLEGRO_KEY_ENTER){
        if(on && on < 3) on++;
        else if(on == 3) RegisterOnClick();
    }
    usr->Text = username;
    if(password1.size() >  pwd1->Text.size()) pwd1->Text += '*';
    else if(password1.size() < pwd1->Text.size()) pwd1->Text.pop_back();
    if(password2.size() >  pwd2->Text.size()) pwd2->Text += '*';
    else if(password2.size() < pwd2->Text.size()) pwd2->Text.pop_back();
}
void RegisterScene::OnKeyUp(int keyCode){
    if(keyCode == ALLEGRO_KEY_LSHIFT || keyCode == ALLEGRO_KEY_RSHIFT) shift = 0;
}
void RegisterScene::RegisterOnClick(){
    if(ExistUser.find(username) != ExistUser.end()) message->Text = "User name crash";
    else if(username.empty() || password1.empty()) message->Text = "Empty name or password";
    else if(password1 != password2) message->Text = "Password different";
    else{
        //TODO loli status
        std::string file = "../Resource/account-status/" + username + ".loli", name, songname;
        std::ofstream fout("../Resource/accountlist.txt", std::ios_base::app);
        fout << username << " " << hash(password1) << "\n";
        user = User(username);
        user.AddCharacter(Loli("Mio", 90, 90, 0));
        user.ChangeOnField("Mio");
        fout.close();
        std::ofstream fin(file, std::ios::trunc);
        fin << 1 << " " << 1 << " " << 10 << "\n" << 1 << "\n";
        fin << "Mio" << " " << 90 << " " << 90 << " " << 0 << "\nMio\n";
        fin.close();
        MainScene* scene = dynamic_cast<MainScene*>(Engine::GameEngine::GetInstance().GetScene("main"));
        scene->user = user;
        Engine::GameEngine::GetInstance().ChangeScene("main");
    }
}
void RegisterScene::InsertOnClick(int t){
    on = t;
}
void RegisterScene::BackOnClick(){
    Engine::GameEngine::GetInstance().ChangeScene("login");
}