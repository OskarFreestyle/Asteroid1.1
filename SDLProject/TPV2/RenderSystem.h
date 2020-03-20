#pragma once
#include "ecs.h"
#include "Manager.h"
#include "ImageComponent.h"
#include "SDL_macros.h"
#include "SDLGame.h"
#include "System.h"
#include "Transform.h"
#include "Entity.h"
#include "Manager.h"
#include "Score.h"
#include "SDLGame.h"
#include "Texture.h"

class RenderSystem: public System {
public:
	RenderSystem() :
			System(ecs::_sys_Render) {
	}

	void draw(Entity *e) {
		Transform *tr = e->getComponent<Transform>(ecs::Transform);
		ImageComponent *img = e->getComponent<ImageComponent>(ecs::ImageComponent);
		SDL_Rect dest =
		RECT(tr->position_.getX(), tr->position_.getY(), tr->width_, tr->height_);
		img->tex_->render(dest, tr->rotation_);
	}

	void drawAsteroid(Entity* e) {
		Transform* tr = e->getComponent<Transform>(ecs::Transform);
		ImageComponent* img = e->getComponent<ImageComponent>(ecs::ImageComponent);
		SDL_Rect dest = RECT(tr->position_.getX(), tr->position_.getY(), tr->width_, tr->height_);
		img->tex_->render(dest, tr->rotation_);

		//CONDICION 1 (sale por la derecha de la pantalla)
		if (tr->position_.getX() + tr->width_ > game_->getWindowWidth()) {
			SDL_Rect rectCond1 = RECT(tr->position_.getX() - game_->getWindowWidth(), tr->position_.getY(), tr->width_, tr->height_);
			img->tex_->render(rectCond1, tr->rotation_);
		}

		//CONDICION 2 (sale por abajo)
		if (tr->position_.getY() + tr->height_ > game_->getWindowHeight()) {
			SDL_Rect rectCond2 = RECT(tr->position_.getX(), tr->position_.getY() - game_->getWindowHeight(), tr->width_, tr->height_);
			img->tex_->render(rectCond2, tr->rotation_);
		}

		//CONDICION 3 (1 y 2 a la vez)
		if ((tr->position_.getX() + tr->width_ > game_->getWindowWidth()) && (tr->position_.getY() + tr->height_ > game_->getWindowHeight())) {
			SDL_Rect rectCond3 = RECT(tr->position_.getX() - game_->getWindowWidth(), tr->position_.getY() - game_->getWindowHeight(), tr->width_, tr->height_);
			img->tex_->render(rectCond3, tr->rotation_);
		}

	}

	void draw(Entity* e,SDL_Rect clip) {
		Transform* tr = e->getComponent<Transform>(ecs::Transform);
		ImageComponent* img = e->getComponent<ImageComponent>(ecs::ImageComponent);
		SDL_Rect dest =
			RECT(tr->position_.getX(), tr->position_.getY(), tr->width_, tr->height_);
		img->tex_->render(dest, tr->rotation_, clip);
	}

	void drawScore() {
		auto sc =
				mngr_->getHandler(ecs::_hdlr_GameState)->getComponent<Score>(ecs::Score);
		Texture scoreMsg(game_->getRenderer(), std::to_string(sc->points_),
				game_->getFontMngr()->getFont(Resources::ARIAL24),
				{ COLOR(0x0000ffff) });
		scoreMsg.render(game_->getWindowWidth() / 2 - scoreMsg.getWidth() / 2,
				10);

	}

	void update() override {
		// draw asteroids
		for (auto& e : mngr_->getGroupEntities(ecs::_grp_Asteroid)) {
			drawAsteroid(e);
		}

		//Draw bullets
		for (auto& e : mngr_->getGroupEntities(ecs::_grp_Bullet)) {
			draw(e);
		}
		
		//Draw fighter
		SDL_Rect clipRect =
			RECT(SPRITESHEET_PLANE_X, SPRITESHEET_PLANE_Y, SPRITESHEET_PLANE_W, SPRITESHEET_PLANE_H);
		draw(mngr_->getHandler(ecs::_hdlr_Fighter), clipRect);
		

		/*// draw pacman
		draw(mngr_->getHandler(ecs::_hdlr_PacMan));*/

		// draw score
		drawScore();

		// info message
		Texture msg(game_->getRenderer(),"Press ENTER to add More Stars", game_->getFontMngr()->getFont(Resources::ARIAL24),{COLOR(0xff0000ff)});
		msg.render(game_->getWindowWidth()/2-msg.getWidth()/2,game_->getWindowHeight()-msg.getHeight()-10);
	}
private:
	const int SPRITESHEET_PLANE_X = 47;
	const int SPRITESHEET_PLANE_Y = 90;
	const int SPRITESHEET_PLANE_W = 207;
	const int SPRITESHEET_PLANE_H = 250;
};

