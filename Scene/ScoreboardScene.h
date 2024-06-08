//
// Created by Hsuan on 2024/4/10.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_SCOREBOARDSCENE_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_SCOREBOARDSCENE_H

#include <allegro5/allegro_audio.h>
#include <memory>
#include <string>
#include "Engine/IScene.hpp"

struct people{

    std::string name;
    int score;
    std::string time;
};

class ScoreboardScene final : public Engine::IScene {
public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void Terminate() override;
    void ChangeOnClick(int changeway);
	void BackOnClick(int stage);
private:
    int page=0;
    int maxpage;
    std::vector<people> record;
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_SCOREBOARDSCENE_H
