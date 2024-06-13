#ifndef REGISTERSCENE_HPP
#define REGISTERSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include "Account/User.hpp"
#include <string>
#include <set>
class RegisterScene final : public Engine::IScene {
private:
    std::set<std::string> ExistUser;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::string username, password1, password2;
    Engine::Label *usr, *pwd1, *pwd2;
    int shift, on;
public:
    explicit RegisterScene() = default;
    void Initialize() override;
    void Terminate() override;
    void RegisterOnClick();
    void InsertOnClick(int t);
    void OnMouseDown(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    User user;
};
#endif //REGISTERSCENE_HPP
