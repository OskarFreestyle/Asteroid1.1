#pragma once
#include "System.h"
#include "Entity.h"
#include "Transform.h"
#include "Manager.h"
#include "ImageComponent.h"
#include "BulletSystem.h"

class FighterSystem :
	public System
{

public:
	FighterSystem() :
		System(ecs::_sys_Fighter),
		fighter_(nullptr),
		tr_(nullptr) {};
	
	//Inicializa al fighter
	void init() override {
		fighter_ = mngr_->addEntity();
		tr_ = fighter_->addComponent<Transform>(Vector2D(100.0, 100), Vector2D(),
			50, 50, 0);
		fighter_->addComponent<ImageComponent>(
			game_->getTextureMngr()->getTexture(Resources::Fighter));
		mngr_->setHandler(ecs::_hdlr_Fighter, fighter_);
	}

	// - poner el caza en el centro con velocidad 0 y rotaci�n 0. No hace falta
	// desactivar la entidad (no dibujarla si el juego est� parado en RenderSystem).
	void onCollisionWithAsteroid() {
		tr_->position_.set({ (double)game_->getWindowWidth() / 2, (double)game_->getWindowHeight() / 2 });
		tr_->velocity_.set({ 0,0 });
		//fighter_->setActive(false);
	}

	void update() override {
		auto ih = InputHandler::instance();
		assert(tr_ != nullptr);
		if (ih->keyDownEvent()) {
			if (ih->isKeyDown(SDLK_RIGHT)) {
				tr_->rotation_ = tr_->rotation_ + 10;
				tr_->velocity_ = tr_->velocity_.rotate(10);
			}
			else if (ih->isKeyDown(SDLK_LEFT)) {
				tr_->rotation_ = tr_->rotation_ - 10;
				tr_->velocity_ = tr_->velocity_.rotate(-10);
			}
			else if (ih->isKeyDown(SDLK_UP)) {
				auto nv = Vector2D(0, -1).rotate(tr_->rotation_);
				tr_->velocity_ = nv * (tr_->velocity_.magnitude() + 0.5);
			}
			else if (ih->isKeyDown(SDLK_DOWN)) {
				auto nv = Vector2D(0, -1).rotate(tr_->rotation_);
				tr_->velocity_ = nv
					* std::max(0.0, (tr_->velocity_.magnitude() - 0.5));
			}

		}
		//Aceleraci�n del fighter
		tr_->position_ = tr_->position_ + tr_->velocity_;
		int x = tr_->position_.getX();
		int y = tr_->position_.getY();

		//Para el rebote con los bordes del juego
		if (x <= 0 || x + tr_->width_ >= game_->getWindowWidth() || y <= 0
			|| y + tr_->height_ >= game_->getWindowHeight()) {
			tr_->rotation_ = fmod(tr_->rotation_ + 180.0, 360.0);
			tr_->velocity_ = tr_->velocity_.rotate(180);
		}

	}
	const Transform* getTranform() { return tr_; };
private:
	Entity* fighter_;
	Transform* tr_;
};

