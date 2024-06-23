#ifndef ONLINEJUDGE_HPP
#define ONLINEJUDGE_HPP
#include <string>

#include "Engine/Sprite.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/Label.hpp"

class PlayScene;

class OnlineJudge : public Engine::Group {
protected:
    float timeSpan;
    float alpha;
    Engine::Label* L;
    PlayScene* getPlayScene();
public:
    OnlineJudge(std::string text, std::string font, float timeSpan, float x, float y);
    void Update(float deltaTime) override;
};
#endif //ONLINEJUDGE_HPP
