#ifndef SONGSELECTSCENE_HPP
#define SONGSELECTSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "Account/User.hpp"

struct song {
    std::string songname,filename, songlan;
    int ez,hd,in;
};

class SongSelectScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::vector<song> songlist;
    int page=0;
    int maxpage;
public:
    User user;
    explicit SongSelectScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(std::string songname, std::string diff);
    void ScoreboardOnClick();
    void BackOnClick(int stage);
    void SettingsOnClick();
    void ChangeOnClick(int changeway);
};

#endif // SONGSELECTTSCENE_HPP
