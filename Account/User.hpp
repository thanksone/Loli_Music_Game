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
    long long password;
    std::vector<Loli> Character;
    std::map<std::string, std::pair<int, float>> BestRecord;
    Loli *loli;
public:
    std::string name;
    std::string font, dirt, floor;
    bool wind;
    float seal;
    settings setting;
    User() = default;
    User(std::string namae, long long pa55word);
    bool cmp(long long pa55word);
    void update();
    void UpdateRecord(std::string song, int score, float acc);
    void AddCharacter(Loli ll);
    bool ChangeOnField(std::string name);
    void ChangeSan(int val);
};
long long hash(std::string pa55word);
User Guest();
#endif //USER_HPP
