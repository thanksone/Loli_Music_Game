#include <allegro5/base.h>
#include <allegro5/color.h>
#include <random>
#include <iostream>

#include "DirtyEffect.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Scene/PlayScene.hpp"

PlayScene* DirtyEffect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
DirtyEffect::DirtyEffect(std::string img, float timeSpan, float x, float y) : Sprite(img, x, y, 180, 180, 0.5, 0.5), timeSpan(timeSpan), alpha(1) {}
void DirtyEffect::Update(float deltaTime) {
	alpha -= deltaTime / timeSpan;
	if (alpha <= 0) {
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	unsigned char r, g ,b, a = alpha * 255;
	al_unmap_rgb(Tint, &r, &g, &b);
	Tint = al_map_rgba(r, g, b, a);
	Sprite::Update(deltaTime);
}
