
#ifndef CHARACTERSELECTSCENE_HPP
#define CHARACTERSELECTSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include <vector>
#include "Engine/IScene.hpp"

struct character {
    std::string charactername;
    int san, maxsan;
};

class CharacterSelectScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::vector<character> characterlist;
    int page=0;
    int maxpage;
public:
    explicit CharacterSelectScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void BackOnClick(int stage);
    void ChangeOnClick(int changeway);

};


#endif //CHARACTERSELECTSCENE_HPP
