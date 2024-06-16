#ifndef SELECTFILE_HPP
#define SELECTFILE_HPP
#include <allegro5/allegro_audio.h>
#include <vector>
#include "Engine/IScene.hpp"
#include "SongSelectScene.hpp"
#include "Account/User.hpp"

class FileSelectScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::vector<song> songlist;
    int page, maxpage;
public:
    User user;
    explicit FileSelectScene() = default;
    void Initialize() override;
    void Terminate() override;
    void EditOnClick(std::string songname, std::string diff);
    void BackOnClick();
    void SettingsOnClick();
    void AboriginalOnClick(int square);
};
#endif //SELECTFILE_HPP
