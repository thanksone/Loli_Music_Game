#ifndef SettingsScene_HPP
#define SettingsScene_HPP

#include <allegro5/allegro_audio.h>
#include <vector>

#include <memory>
#include "Engine/IScene.hpp"
#include "Account/User.hpp"

class SettingsScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    int pre, page;
    User user;
    explicit SettingsScene() = default;

    void Initialize() override;

    void Terminate() override;

    void BackOnClick(int stage);
    void StartOnClick();

    void BGMSlideOnValueChanged(float value);

    void SFXSlideOnValueChanged(float value);

};

#endif // SettingsScene_HPP
