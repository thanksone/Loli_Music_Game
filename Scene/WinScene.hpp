#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include "Engine/IScene.hpp"
#include "Account/User.hpp"
#include "SongSelectScene.hpp"

class WinScene final : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_ID bgmId;
public:
    User user;
	int score, perfect, good, bad, miss, maxcombo;
    float acc;
    song give;
	explicit WinScene() = default;
	void Initialize() override;
	void Terminate() override;
	void BackOnClick();
};

#endif // WINSCENE_HPP