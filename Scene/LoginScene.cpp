#include <fstream>
#include "LoginScene.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"

void LoginScene::Initialize(){
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    username.clear(), password.clear();
    std::string namae, pa55word;
    std::ifstream fin("Resource/accountlist.txt");
    while(fin >> namae >> pa55word) mamamia[namae] = pa55word;
    fin.close();
    shift = 0;
    Engine::ImageButton* btn;
    btn->SetOnClickCallback(std::bind(&LoginScene::LoginOnClick, this));
    btn->SetOnClickCallback(std::bind(&LoginScene::InsertOnClick, this, 1));
    btn->SetOnClickCallback(std::bind(&LoginScene::InsertOnClick, this, 2));
    usr = new Engine::Label("", "pirulen.ttf", 48, halfW, halfH - 100, 255, 255, 255, 255, 0.5, 0.5);
    pwd = new Engine::Label("", "pirulen.ttf", 48, halfW, halfH + 100, 255, 255, 255, 255, 0.5, 0.5);
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
    if(mamamia[username] != hash(password)) AddNewObject(new Engine::Label("User name or password incorrect", "pirulen.ttf", 30, 500, 900, 255, 255, 255, 255, 0.5, 0.5));
    else{
        int n, san, fullsan, score;
        float acc;
        std::string file = "Resource/account-status/" + username + ".txt", name, songname;
        std::ifstream fin(file);
        user = User(username, password);
        fin >> n;
        while(n--){
            if(!(fin >> name >> san >> fullsan)) break;
            user.AddCharacter(Loli(name, san, fullsan));
        }
        while(fin >> songname >> score >> acc){
            user.UpdateRecord(songname, score, acc);
        }
        fin.close();
        Engine::GameEngine::GetInstance().ChangeScene("main");
    }
}
void LoginScene::InsertOnClick(int t){
    on = t;
}