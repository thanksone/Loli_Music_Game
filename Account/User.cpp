#include <algorithm>
#include <fstream>
#include "User.hpp"
#define ff first
#define ss second

long long hash(std::string S){
    const long long huh = 1145143ll, beast = 191981071145147ll;
    long long huhhuhhuhhuuuuh = 1145143ll, sum = 0;
    for(char s : S){
        sum = (sum + s * huhhuhhuhhuuuuh) % beast;
        huhhuhhuhhuuuuh = huhhuhhuhhuuuuh * huh % beast;
    }
    return sum;
}
User Guest(){
    User guest = User("guest");
    guest.AddCharacter(Loli("loli", 1, 1, 0));
    guest.ChangeOnField("loli");
    return guest;
}
User::User(std::string namae): name(namae){
    setting.fallspeed = 900;
}
void User::UpdateRecord(std::string song, int score, float acc){
    BestRecord[song].ff = std::max(score, BestRecord[song].ff);
    BestRecord[song].ss = std::max(acc, BestRecord[song].ss);
}
void User::update(){
    font = wind? "Raslani-Kavaliar-Kaiser-1.ttf" : "Black-Magic-2.ttf";
    dirt = wind? "stage-select/sanbaddirt.png" : "stage-select/sangooddirt.png";
    floor = wind? "stage-select/sanbadfloor.png" : "stage-select/sangoodfloor.png";
    leftdirt = wind? "stage-select/darkleft.png" : "stage-select/blueleft.png";
    leftfloor = wind? "stage-select/redleft.png" : "stage-select/pinkleft.png";
    rightdirt = wind? "stage-select/darkright.png" : "stage-select/blueright.png";
    rightfloor = wind? "stage-select/redright.png" : "stage-select/pinkright.png";
    std::ofstream fout("../Resource/account-status/" + name + ".loli", std::ios::trunc);
    fout << Character.size() << "\n";
    for(Loli ll : Character){
        fout << ll.name << " " << ll.san << " " << ll.fullsan << " " << (ll.wind? "1\n" : "0\n");
    }
    fout << loli->name << "\n";
    for(auto [song, score] : BestRecord){
        fout << song << " " << score.ff << " " << score.ss << "\n";
    }
    fout.close();
}
void User::AddCharacter(Loli avocado){
    for(Loli ll : Character){
        if(avocado.name == ll.name) return;
    }
    Character.push_back(avocado);
}
bool User::ChangeOnField(std::string name){
    for(int i = 0; i < Character.size(); i++){
        if(name == Character[i].name){
            loli = &Character[i];
            wind = loli->wind;
            update();
            return 1;
        }
    }
    return 0;
}
void User::ChangeSan(int val){
    if(val < 0 && val >= loli->san / 5){
        loli->wind = wind = 1;
    }else if(val >= 0) loli->wind = wind = 0;
    loli->san = std::min(std::max(loli->san + val, 0), loli->fullsan);
    if(!loli->san) loli->wind = wind = 1;
    update();
}