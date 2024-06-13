#include <algorithm>
#include "User.hpp"
#define ff first
#define ss second

std::string hash(std::string S){

}
User::User(std::string namae, std::string pa55word): name(namae), password(pa55word){}
bool User::cmp(std::string pa55word){
    return pa55word == password;
}
void User::UpdateRecord(std::string song, int score, float acc){
    BestRecord[song].ff = std::max(score, BestRecord[song].ff);
    BestRecord[song].ss = std::max(acc, BestRecord[song].ss);
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
}