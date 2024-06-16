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
    Engine::ImageButton* btn;
    btn->SetOnClickCallback(std::bind(&RegisterScene::RegisterOnClick, this));
    btn->SetOnClickCallback(std::bind(&RegisterScene::InsertOnClick, this, 1));
    btn->SetOnClickCallback(std::bind(&RegisterScene::InsertOnClick, this, 2));
    btn->SetOnClickCallback(std::bind(&RegisterScene::BackOnClick, this));
    usr = new Engine::Label("", "pirulen.ttf", 48, halfW, halfH - 100, 255, 255, 255, 255, 0.5, 0.5);
    pwd1 = new Engine::Label("", "pirulen.ttf", 48, halfW, halfH + 100, 255, 255, 255, 255, 0.5, 0.5);
    pwd2 = new Engine::Label("", "pirulen.ttf", 48, halfW, halfH + 200, 255, 255, 255, 255, 0.5, 0.5);
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
    if(ExistUser.find(username) != ExistUser.end()) AddNewObject(new Engine::Label("User name crash", "pirulen.ttf", 30, 500, 900, 255, 255, 255, 255, 0.5, 0.5));
    else if(password1 != password2){
        AddNewObject(new Engine::Label("Password different", "pirulen.ttf", 30, 500, 900, 255, 255, 255, 255, 0.5, 0.5));
    }else{
        //TODO loli status
        std::string file = "Resource/account-status/" + username + ".txt", name, songname;
        std::ofstream fout("Resource/accountlist.txt", std::ios_base::app);
        fout << username << " " << hash(password1) << "\n";
        user = User(username, hash(password1));
        user.AddCharacter(Loli("loli", 0, 0));
        user.ChangeOnField("loli");
        fout.close();
        std::ofstream fin(file);
        fin << 1 << "\n";
        fin << "loli" << " " << "san" << " " << "fullsan" << "\n";
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