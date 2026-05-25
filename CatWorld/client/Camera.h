#pragma once
#include"vector2.h"
#include"timer.h"

class Camera
{
public:
	Camera() {
		//抖动效果计时器初始化
		shack_timer.set_one_shot(true);
		shack_timer.set_callback(
			[&]() {
				is_shack = false;
				reset();
			}
		);
	}
	~Camera() = default;

	const Vector2& get_position() const {
		return position;
	}
	const Vector2& get_size() const {
		return size;
	}
	void set_position(const Vector2& position) {
		this->position = position;
	}
	void set_size(const Vector2& size) {
		this->size = size;
	}


	void reset() {
		position.x = 0;
		position.y = 0;
	}

	void on_update(int delta) {
		shack_timer.on_update(delta);
		if (is_shack) {
			position.x = (-50 + rand() % 100) / 50.0f * shack_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shack_strength;
			printf("%lf %lf\n", position.x, position.y);
		}
	}

	void shack(float strength, float shacke_time) {
		is_shack = true;
		shack_strength = strength;

		shack_timer.set_wait((int)shacke_time);
		shack_timer.restart();
	}
	void look_at(const Vector2& pos)
	{
		this->position = pos - (size / 2.0f);
	}

private:
	Vector2 position;
	Vector2 size;
	Timer shack_timer;
	bool is_shack = false;//是否抖动
	float shack_strength = 0;
};
