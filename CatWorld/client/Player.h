#pragma once
#include"RssourcesManager.h"
#include"Vector2.h"
#include"Animation.h"
#include"Camera.h"
class Player
{
	enum class Facing
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
public:
	Player(bool is_hajimi)
	{
		{
			idle_facing_up.set_interval(.3f);
			idle_facing_up.set_loop(true);
			idle_facing_up.add_atlas(is_hajimi ? RssourcesManager::instance()->find_atlas("hajimi_idle_back")
				: RssourcesManager::instance()->find_atlas("manbo_idle_back"));
			idle_facing_down.set_interval(.3f);
			idle_facing_down.set_loop(true);
			idle_facing_down.add_atlas(is_hajimi ? RssourcesManager::instance()->find_atlas("hajimi_idle_from")
				: RssourcesManager::instance()->find_atlas("manbo_idle_from"));
			idle_facing_left.set_interval(.3f);
			idle_facing_left.set_loop(true);
			idle_facing_left.add_atlas(is_hajimi ? RssourcesManager::instance()->find_atlas("hajimi_idle_left")
				: RssourcesManager::instance()->find_atlas("manbo_idle_left"));
			idle_facing_right.set_interval(.3f);
			idle_facing_right.set_loop(true);
			idle_facing_right.add_atlas(is_hajimi ? RssourcesManager::instance()->find_atlas("hajimi_idle_right")
				: RssourcesManager::instance()->find_atlas("manbo_idle_right"));
		}
		{
			run_facing_up.set_interval(.3f);
			run_facing_up.set_loop(true);
			run_facing_up.add_atlas(is_hajimi ? RssourcesManager::instance()->find_atlas("hajimi_run_back")
				: RssourcesManager::instance()->find_atlas("manbo_run_back"));
			run_facing_down.set_interval(.3f);
			run_facing_down.set_loop(true);
			run_facing_down.add_atlas(is_hajimi ? RssourcesManager::instance()->find_atlas("hajimi_run_from")
				: RssourcesManager::instance()->find_atlas("manbo_run_from"));
			run_facing_left.set_interval(.3f);
			run_facing_left.set_loop(true);
			run_facing_left.add_atlas(is_hajimi ? RssourcesManager::instance()->find_atlas("hajimi_run_left")
				: RssourcesManager::instance()->find_atlas("manbo_run_left"));
			run_facing_right.set_interval(.3f);
			run_facing_right.set_loop(true);
			run_facing_right.add_atlas(is_hajimi ? RssourcesManager::instance()->find_atlas("hajimi_run_right")
				: RssourcesManager::instance()->find_atlas("manbo_run_right"));
		}
	}
	~Player() = default;

	void on_update(const float delta)
	{
		//∆Ωª¨≤Â÷µ“∆∂Ø
		if (!position.approx(real_pos))
			velocity = (real_pos - position).normalize() * SPEED;
		else
			velocity = { 0,0 };
		if ((real_pos - position).length() <= (velocity * delta).length())
			position = real_pos;
		else
		{
			position += velocity * delta;
		}
		if (velocity.approx(Vector2({ 0,0 })))
		{
			switch (current_face)
			{
			case Facing::UP:
				current_ani = &idle_facing_up;
				break;
			case Facing::DOWN:
				current_ani = &idle_facing_down;
				break;
			case Facing::LEFT:
				current_ani = &idle_facing_left;
				break;
			case Facing::RIGHT:
				current_ani = &idle_facing_right;
				break;
			}
		}
		else
		{
			if (abs(velocity.x)>=0.0001f) (velocity.x > 0) ? current_face = Facing::RIGHT : current_face = Facing::LEFT;
			if (abs(velocity.y)>=0.0001f) (velocity.y > 0) ? current_face = Facing::DOWN : current_face = Facing::UP;

			switch (current_face)
			{
			case Facing::UP:
				current_ani = &run_facing_up;
				break;
			case Facing::DOWN:
				current_ani = &run_facing_down;
				break;
			case Facing::LEFT:
				current_ani = &run_facing_left;
				break;
			case Facing::RIGHT:
				current_ani = &run_facing_right;
				break;
			}
		}
		current_ani->set_position(position);
		current_ani->on_update(delta);
	}
	void on_render(const Camera& camera)
	{
		if (current_ani)
		{
			current_ani->on_render(camera);
		}
	}
	Vector2 get_pos() const
	{
		return position;
	}
	void set_pos(const Vector2& position)
	{
		this->position = position;
	}
	Vector2 get_real_pos() const
	{
		return real_pos;
	}
	void set_real_pos(const Vector2& real_pos)
	{
		this->real_pos = real_pos;
	}

private:
	const float SPEED = 100.0f;
	Vector2 position;
	Vector2 velocity = { 0,0 };
	Vector2 real_pos;

	Animation idle_facing_up;
	Animation idle_facing_down;
	Animation idle_facing_left;
	Animation idle_facing_right;

	Animation run_facing_up;
	Animation run_facing_down;
	Animation run_facing_left;
	Animation run_facing_right;

	Animation* current_ani = nullptr;

	Facing current_face = Facing::LEFT;
};
