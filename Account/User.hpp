#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <map>
#include <utility>
#include <vector>
#include "Character/Loli.hpp"
struct settings{
    float BGMVolume, SFXVolume, fallspeed;
};
class User{
private:
public:
    Loli *loli;
    std::vector<Loli> Character;
    std::map<std::string, std::pair<int, float>> BestRecord;
    std::string name;
    std::string font, dirt, floor, leftdirt, leftfloor, rightdirt, rightfloor;
    bool wind;
    float seal;
    settings setting;
    User() = default;
    User(std::string namae);
    void update();
    void UpdateRecord(std::string song, int score, float acc);
    void AddCharacter(Loli ll);
    bool ChangeOnField(std::string name);
    void ChangeSan(int val);
};
long long hash(std::string pa55word);
User Guest();
#endif //USER_HPP
