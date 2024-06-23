#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <queue>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Account/User.hpp"
#include "EditScene.hpp"
#include "Turret/Turret.hpp"
#include "SongSelectScene.hpp"

struct noto{
    int type, ghost;
    float at, em, speed;
};
class PlayScene final : public Engine::IScene {
private:
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> Song, tap, hold;
protected:
    int perfect, good, bad, miss, combo, total, maxcombo;
    float last, curtime, pause, score, acc;
    Engine::Label *Score, *Combo, *Left;
    Engine::ImageButton *Pause, *Back, *Restart, *Continue;
    std::queue<noto> Note;
    bool Hold[12], boing;
public:
    User user;
	std::string diff;
    song give;
    int deadline, x0, ghostW;
    Group *NoteGroup[6][4], *EffectGroup;
    Turret *Line[6];
	explicit PlayScene() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
	void ReadScore();
	void Construct();
    void PauseOnClick();
    void BackOnClick();
    void RestartOnClick();
    void ContinueOnClick();
    void Hit(int type);
};
#endif // PLAYSCENE_HPP
