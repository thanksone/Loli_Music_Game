#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <map>
#include <utility>
#include <vector>
#include "Character/Loli.hpp"
std::string hash(std::string pa55word);
struct settings{
    float BGMVolume, SFXVolume, fallspeed;
};
class User{
private:
    std::string password;
    std::vector<Loli> Character;
    std::map<std::string, std::pair<int, float>> BestRecord;
    Loli *loli;
public:
    std::string name;
    bool wind;
    float seal;
    User(){};
    User(std::string namae, std::string pa55word);
    bool cmp(std::string pa55word);
    void UpdateRecord(std::string song, int score, float acc);
    void AddCharacter(Loli ll);
    bool ChangeOnField(std::string name);
    void ChangeSan(int val);
};
#endif //USER_HPP
