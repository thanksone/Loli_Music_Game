#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <vector>
#include <utility>
#include "Character/Loli.hpp"
class User{
private:
    std::string name, password;
    std::vector<Loli> Character;
    std::vector<std::pair<int, float>> BestRecord;
    int onField;
public:
    User(std::string namae, std::string pa55word);
    std::string GetName();
    bool cmp(std::string pa55word);
    void UpdateRecord(int song, int score, float acc);
    void AddCharacter(Loli ll);
    void ChangeOnField(int p);
    void ChangeSan(int val);
};
#endif //_USER_HPP
