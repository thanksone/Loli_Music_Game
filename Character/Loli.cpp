#include "Loli.hpp"
#include "Scene/MainScene.hpp"
#include <fstream>
#include <iostream>


Loli::Loli(std::string namae, int san, int fullsan, bool wind): name(namae), san(san), fullsan(fullsan), wind(wind){}
void Loli::draw(Engine::IScene *scene, float x, float y, float w, float h, float anchorX, float anchorY){
    std::string tachie = (wind? "characters/sanbad" : "characters/sangood") + name + ".png";
    scene->AddNewObject(new Engine::Image(tachie, x, y, w, h, anchorX, anchorY));
}

Engine::ImageButton* Loli::touch( Engine::IScene *scene, float x, float y, float w, float h){
    std::string tachie = (wind? "characters/sanbad" : "characters/sangood") + name + ".png";
    Engine::ImageButton* btn;
    btn=new Engine::ImageButton(tachie, tachie, x, y, w, h);
    return btn;
}

std::string Loli::talk() {
    /*std::cout<<"ouob";
    std::ifstream fgetc("Resource/"+name+".txt");
    std::cout<<"douo";
    std::string sent;
    char c;
    while(fgetc>>c){
        std::cout<<c;
        if(c!='\n') {
            sent+=c;
        }
        else {
            std::cout<<sent<<"\n";
            commu.push_back(sent);
            std::cout<<commu[commu.size()-1];
            sent="";
        }

    }
    fgetc.close();*/
    if(name=="ALice") {
        std::string sent="Hello, my name is ALice, I will be responsible for your life.";
        commu.push_back(sent);
        sent="This book is about the ways philosophers pursuing the truth.";
        commu.push_back(sent);
    }
    else {
        std::string sent="I am Mio! Really nice to meet you.";
        commu.push_back(sent);
        sent="I like cakes! But if you also like it, I will give you half.";
        commu.push_back(sent);
    }
    /*for(int i=0;i<commu.size();i++) {
        std::cout<<commu[i]<<"\n";
    }
    std::cout<<"ouob";*/
    return commu[rand()%2];
}
