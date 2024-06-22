#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include "Engine/IScene.hpp"
#include "Account/User.hpp"

class WinScene final : public Engine::IScene {
private:
	float ticks;
	ALLEGRO_SAMPLE_ID bgmId;
public:
    User user;
	std::string name;
	int score;
	explicit WinScene() = default;
	void Initialize() override;
	void Terminate() override;
	void BackOnClick(int stage);
	void OnKeyDown(int keyCode) override;
};

#endif // WINSCENE_HPP