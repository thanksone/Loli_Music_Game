#include <allegro5/base.h>
#include <allegro5/color.h>
#include <random>
#include <iostream>

#include "OnlineJudge.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Scene/PlayScene.hpp"

PlayScene* OnlineJudge::getPlayScene() {
    return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
OnlineJudge::OnlineJudge(std::string text, std::string font, float timeSpan, float x, float y) : timeSpan(timeSpan), alpha(1) {
    if(text == "Perfect") L = new Engine::Label(text, font, 30, x, y, 255, 255, 0, 255, 0.5, 0.5);
    else if(text == "Good") L = new Engine::Label(text, font, 30, x, y, 0, 255, 255, 255, 0.5, 0.5);
    else if(text == "Bad") L = new Engine::Label(text, font, 30, x, y, 255, 0, 0, 255, 0.5, 0.5);
    else if(text == "Miss") L = new Engine::Label(text, font, 30, x, y, 255, 255, 255, 255, 0.5, 0.5);
    this->AddNewObject(L);
}
void OnlineJudge::Update(float deltaTime) {
    alpha -= deltaTime / timeSpan;
    if (alpha <= 0) {
        getPlayScene()->EffectGroup->RemoveObject(objectIterator);
        return;
    }
    unsigned char r, g ,b, a = alpha * 255;
    al_unmap_rgb(L->Color, &r, &g, &b);
    L->Color = al_map_rgba(r, g, b, a);
}
