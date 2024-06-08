#ifndef EDITSCENE_HPP
#define EDITSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"

class EditScene final : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_ID bgmId;
protected:
	int SpeedMult;
    int gon, pi, on;
    std::vector<std::vector<int>> state;
    std::vector<std::vector<Engine::Label*>> word;
public:
    int halfW, halfH;
	explicit EditScene() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void OnMouseDown(int button, int mx, int my) override;
	void OnMouseMove(int mx, int my) override;
	void OnMouseUp(int button, int mx, int my) override;
	void OnKeyDown(int keyCode) override;
    void BackOnClick();
};
#endif // EDITSCENE_HPP
