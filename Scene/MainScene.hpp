#ifndef MAINSCENE_HPP
#define MAINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "Account/User.hpp"
#include "UI/Component/Label.hpp"


class MainScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    User user;
    Engine::Label *talk;
    int f=0;
    explicit MainScene() = default;
    void Initialize() override;
    void Terminate() override;
    void ScoreboardOnClick();
    void BackOnClick(int stage);
    void SettingsOnClick();
    void EditOnClick();
    void SongSelectOnClick();
    void CharacterSelectOnClick();
    void TouchOnClick();
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> Getbgm();

};

#endif // STAGESELECTSCENE_HPP
