#ifndef EDITSCENE_HPP
#define EDITSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Group.hpp"
#include "Engine/IControl.hpp"
#include "Engine/IObject.hpp"
struct note{
    int type, ghost;
    float len, at, speed;
    note(int t, int g, float l, float a, float s): type(t), ghost(g), len(l), at(a), speed(s){}
    bool operator==(note N){
        return type == N.type && ghost == N.ghost && len == N.len && at == N.at && speed == N.speed;
    }
};
class EditScene final : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_ID bgmId;
protected:
	int SpeedMult;
    int gon, pi, on, total, lpm;
    float ghostW, lineH;
    std::vector<std::vector<note>> Note;
    std::vector<std::vector<int>> State; // bpm, meters
    std::vector<std::vector<Engine::Label*>> Word;
    std::vector<std::pair<int, note>> onField;
    std::vector<Engine::IControl*> NoteButtonCtrl;
    std::vector<Engine::IObject*> NoteButtonObj;
    std::vector<Engine::Label*> Line;
    Engine::Image* imgTarget;
public:
    std::string filename;
    int halfW, halfH;
	explicit EditScene() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(float deltatime) override;
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
    void FindPos(int pos);
    void ReadScore();
    void ConstructUI();
    void ConstructNote(int g, note N);
    void DisplayNote();
    void DisplayLine();
    void DeleteNoteClick(int n);
    void DeleteNoteButton(int n);
    void AddNoteButton(int g, note N);
    void ClearNote();
    void ClearLine();

};
#endif // EDITSCENE_HPP
