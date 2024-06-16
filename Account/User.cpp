#include <algorithm>
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
    User guest = User("guest", 0);
    guest.AddCharacter(Loli("loli", 0, 0));
    guest.ChangeOnField("loli");
    return guest;
}
User::User(std::string namae, long long pa55word): name(namae), password(pa55word){}
bool User::cmp(long long pa55word){
    return pa55word == password;
}
void User::UpdateRecord(std::string song, int score, float acc){
    BestRecord[song].ff = std::max(score, BestRecord[song].ff);
    BestRecord[song].ss = std::max(acc, BestRecord[song].ss);
}
void User::update(){
    font = wind? "WOODCUTTER-BCN-Style-1.ttf" : "Black-Magic-2.ttf";
    dirt = wind? "stage-select/sanbaddirt" : "stage-select/sangooddirt";
    floor = wind? "stage-select/sanbadfloor" : "stage-select/sangoodfloor";
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
    update();
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