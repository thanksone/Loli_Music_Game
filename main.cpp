// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/CharacterSelectScene.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.h"
#include "Scene/ScoreboardScene.h"
#include "Scene/SettingsScene.hpp"
#include "Scene/MainScene.hpp"
#include "Scene/SongSelectScene.hpp"
#include "Scene/FileSelectScene.hpp"
#include "Scene/EditScene.hpp"
int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // TODO: [HACKATHON-1-SCENE] (3/4): Register Scenes here
	game.AddNewScene("settings", new SettingsScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());
	game.AddNewScene("start", new StartScene());
	game.AddNewScene("scoreboard", new ScoreboardScene());
	game.AddNewScene("stage-select", new StageSelectScene());
    game.AddNewScene("edit", new EditScene());
	game.AddNewScene("main", new MainScene());
	game.AddNewScene("character-select", new CharacterSelectScene());
	game.AddNewScene("song-select", new SongSelectScene());
    game.AddNewScene("file-select", new FileSelectScene());


    // TODO: [HACKATHON-1-SCENE] (4/4): Change the start scene
	game.Start("start", 60, 1800, 1020);
	return 0;
}
