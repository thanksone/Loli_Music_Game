
#ifndef CHARACTERSELECTSCENE_HPP
#define CHARACTERSELECTSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"

class CharacterSelectScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit CharacterSelectScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void BackOnClick(int stage);

};


#endif //CHARACTERSELECTSCENE_HPP
