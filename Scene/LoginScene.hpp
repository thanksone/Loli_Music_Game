#ifndef LOGINSCENE_HPP
#define LOGINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include "Account/User.hpp"
#include <string>
#include <map>
class LoginScene final : public Engine::IScene {
private:
    std::map<std::string, long long> mamamia;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::string username, password;
    Engine::Label *usr, *pwd, *message;
    int shift, on;
public:
    explicit LoginScene() = default;
    void Initialize() override;
    void Terminate() override;
    void LoginOnClick();
    void InsertOnClick(int t);
    void OnMouseDown(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void BackOnClick();
    void RegisterOnClick();
    User user;
};
#endif //LOGINSCENE_HPP
