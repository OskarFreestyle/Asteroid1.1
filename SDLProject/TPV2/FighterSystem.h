#pragma once
#include "System.h"
#include "Entity.h"
#include "Transform.h"
#include "Manager.h"
#include "ImageComponent.h"
#include "BulletSystem.h"

const int IMPULSE = 10;
const int ANGLE_VEL = 10;
const double DECREASE_VEL = 0.97;

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
		tr_ = fighter_->addComponent<Transform>(Vector2D(100.0, 100), Vector2D(), 50, 50, 0);
		fighter_->addComponent<ImageComponent>(game_->getTextureMngr()->getTexture(Resources::Fighter));
		mngr_->setHandler(ecs::_hdlr_Fighter, fighter_);
	}

	// - poner el caza en el centro con velocidad 0 y rotación 0. No hace falta
	// desactivar la entidad (no dibujarla si el juego está parado en RenderSystem).
	void onCollisionWithAsteroid() {
		tr_->position_.set({ (double)game_->getWindowWidth() / 2, (double)game_->getWindowHeight() / 2 });
		tr_->velocity_.set({ 0,0 });
		//fighter_->setActive(false);
	}

	void update() override {
		InputHandler* ih = InputHandler::instance();
		assert(tr_ != nullptr);
		if (ih->keyDownEvent()) {
			if (ih->isKeyDown(SDLK_RIGHT)) {
				tr_->rotation_ += ANGLE_VEL;
			}
			else if (ih->isKeyDown(SDLK_LEFT)) {
				tr_->rotation_ -= ANGLE_VEL;
			}
			else if (ih->isKeyDown(SDLK_UP)) {	//Impulso * cos(ang) = x y sin = y
				tr_->velocity_.set(Vector2D({ IMPULSE * sin(M_PI * tr_->rotation_ / 180.0f), -(IMPULSE * cos(M_PI * tr_->rotation_ / 180.0)) }));
			}
		}
		else {
			if ((tr_->velocity_.getX() != 0.0) || (tr_->velocity_.getY() != 0.0)) {
				tr_->velocity_.set(Vector2D({ DECREASE_VEL * tr_->velocity_.getX(), DECREASE_VEL * tr_->velocity_.getY() }));
			}
		}
		
		//Sale por los laterales
		if (tr_->position_.getX() < 0.0 || tr_->position_.getX() + tr_->width_ > game_->getWindowWidth()) {
			tr_->velocity_.set(-tr_->velocity_.getX(), tr_->velocity_.getY());		//Se cambia la velocidad
			tr_->rotation_ = -tr_->rotation_;										//Se cambia la rotación	
			//Se evita que se quede fuera de los límites de la pantalla (BUG)
			if (tr_->position_.getX() < 0.0) tr_->position_.set(0.0, tr_->position_.getY());
			else if (tr_->position_.getX() > game_->getWindowWidth()) tr_->position_.set(game_->getWindowWidth(), tr_->position_.getY());
		}

		//Sale por los extremos
		if (tr_->position_.getY() < 0.0 || tr_->position_.getY() + tr_->height_ > game_->getWindowHeight()) {
			tr_->velocity_.set(tr_->velocity_.getX(), -tr_->velocity_.getY());		//Se cambia la velocidad
			tr_->rotation_ = (180 - tr_->rotation_);								//Se cambia la rotación
			//Se evita que se quede fuera de los límites de la pantalla (BUG)
			if (tr_->position_.getY() < 0.0) tr_->position_.set(tr_->position_.getX(), 0.0);
			else if (tr_->position_.getY() > game_->getWindowHeight()) tr_->position_.set(tr_->position_.getX(), game_->getWindowHeight());
		}

		tr_->position_.setX(tr_->position_.getX() + tr_->velocity_.getX());
		tr_->position_.setY(tr_->position_.getY() + tr_->velocity_.getY());
	}
	const Transform* getTranform() { return tr_; };
private:
	Entity* fighter_;
	Transform* tr_;
};

