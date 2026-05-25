#pragma once
#include<functional>
using namespace std;

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void restart() {
		pass_time = 0;
		shotted = false;
	}
	void set_wait(float time) {
		wait_time = time;
	}
	void set_pause(bool flag) {
		paused = flag;
	}
	void set_one_shot(bool flag) {
		one_shot = flag;
	}
	void set_callback(function<void()> callback) {
		this->callback = callback;
	}
	void on_update(float delta) {
		if (paused)
			return;
		pass_time += delta;
		if (pass_time >= wait_time) {
			if (!one_shot || (one_shot && !shotted) || callback)
				callback();
			shotted = true;
			pass_time = 0;
		}
	}
private:
	float pass_time = 0;//已过时间
	float wait_time = 0;//等待时间
	bool shotted = false;//是否触发
	bool paused = false;//是否暂停
	bool one_shot = false;//是否单次触发
	function<void()> callback;
};