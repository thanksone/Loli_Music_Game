#ifndef SONGSELECTSCENE_HPP
#define SONGSELECTSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"

class SongSelectScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::vector<std::string> songlist;
    int page=0;
    int maxpage;
public:
    explicit SongSelectScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void ScoreboardOnClick();
    void BackOnClick(int stage);
    void SettingsOnClick();
    void ChangeOnClick(int changeway);
};

#endif // SONGSELECTTSCENE_HPP
