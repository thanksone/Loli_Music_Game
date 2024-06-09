#ifndef EDITSCENE_HPP
#define EDITSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"

struct note{
    int type, ghost;
    float len, at, speed;
};
class EditScene final : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_ID bgmId;
protected:
	int SpeedMult;
    int gon, pi, on, total, lpm;
    std::vector<std::vector<note>> Note;
    std::vector<std::vector<int>> State;
    std::vector<std::vector<Engine::Label*>> Word;
public:
    std::string filename;
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
    void SaveOnClick();
    void InsertOnClick(int type);
    void AddOnClick();
    void LPMOnClick(int val);
    void POSSliderOnValueChanged(float value);
};
#endif // EDITSCENE_HPP
