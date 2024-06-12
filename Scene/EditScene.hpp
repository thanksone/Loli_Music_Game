#ifndef EDITSCENE_HPP
#define EDITSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include <queue>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Group.hpp"
#include "Engine/IControl.hpp"
#include "Engine/IObject.hpp"
struct note{
    int type, ghost, len;
    float at, speed;
    note(int t, int g, int l, float a, float s): type(t), ghost(g), len(l), at(a), speed(s){}
    bool operator==(note N){
        return type == N.type && ghost == N.ghost && len == N.len && at == N.at && speed == N.speed;
    }
};
class EditScene final : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_ID bgmId;
protected:
    int pi, on, total, lpm, x0, hold, len, front, now;
    float ghostW, lineH, speed, last, past;
    std::vector<std::vector<note>> Note;
    std::vector<int> BPM;
    std::vector<Engine::Label*> Word;
    std::vector<Engine::IControl*> NoteButtonCtrl;
    std::vector<std::vector<Engine::IObject*>> NoteButtonObj;
    std::vector<Engine::Label*> Line;
    std::vector<std::pair<int, note>> onField;
    std::vector<Engine::IObject*> imgTarget;
    std::vector<Engine::IObject*> HoldNote;
    Engine::Label *LPM, *LEN, *SPEED, *RunningLine;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> audio, tapsound, holdsound;
    std::vector<note> Boing;
    std::queue<float> Time;
public:
    std::string filename, songname;
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
    void DeleteOnClick();
    void LineAddOnClick(int val);
    void LenAddOnClick(int val);
    void SpeedAddOnClick(float val);
    void PiAddOnClick(int val);
    void PlayOnClick();
    void PlayHeadOnClick();
    void StopOnClick();
    void POSSliderOnValueChanged(float value);
    void ReadScore();
    void DisplayNote();
    void DisplayLine();
    void DeleteNoteClick(int n);
    void DeleteNoteButton(int n);
    void AddNoteButton(note N, int x, int y);
    void ClearNote();
    void ClearLine();
    bool CheckSpaceValid(note N, int y);
    void ConstructUI();
};
#endif // EDITSCENE_HPP
