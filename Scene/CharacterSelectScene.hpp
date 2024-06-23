
#ifndef CHARACTERSELECTSCENE_HPP
#define CHARACTERSELECTSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <memory>
#include <vector>
#include "Engine/IScene.hpp"
#include "Account/User.hpp"
#include "Character/Loli.hpp"
#include "UI/Component/Label.hpp"


class CharacterSelectScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::vector<Loli> characterlist;
    int page=0;
    int maxpage;
    Engine::Label *status;
public:
    User user;
    explicit CharacterSelectScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
    void ChangeOnClick(int changeway);
    void ChooseOnClick();

};


#endif //CHARACTERSELECTSCENE_HPP
