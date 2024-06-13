#ifndef SELECTFILE_HPP
#define SELECTFILE_HPP
#include <allegro5/allegro_audio.h>
#include <string>
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"

class FileSelectScene final : public Engine::IScene {
private:
    int on;
    std::string filename, songname;
    Engine::Label *File, *Song;
public:
    explicit FileSelectScene() = default;
    void Initialize() override;
    void Terminate() override;
    void EditOnClick();
    void InsertOnClick(int t);
    void OnKeyDown(int keyCode) override;
    void OnMouseDown(int button, int mx, int my) override;
};
#endif //SELECTFILE_HPP
