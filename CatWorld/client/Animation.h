#pragma once
#include"utill.h"

class Animation
{
public:
	enum class AnchorMode
	{
		Centered,//居中锚点
		BottomCentered//底部居中锚点
	};
public:
	Animation()
	{
		timer.set_one_shot(false);
		timer.set_callback(
			[&]()
			{
				idx_frame++;
				if (idx_frame >= frame_list.size())
				{
					idx_frame = is_loop ? 0 : frame_list.size() - 1;
					if (!is_loop && on_finished)
						on_finished();
				}
			});
	}
	~Animation() = default;

	void reset()
	{
		timer.restart();
		idx_frame = 0;
	}

	void set_anchor_mode(AnchorMode mode)
	{
		anchor_mode = mode;
	}
	void set_position(const Vector2& position)
	{
		this->position.x = position.x;
		this->position.y = position.y;
	}

	void set_loop(bool is_loop)
	{
		this->is_loop = is_loop;
	}

	void set_interval(float interval)
	{
		timer.set_wait(interval);
	}

	void set_on_finished(std::function<void()> on_finished)
	{
		this->on_finished = on_finished;
	}

	int get_idx_frame() const
	{
		return idx_frame;
	}

	void add_frame(IMAGE* img, int num_h)
	{
		int width = img->getwidth();
		int height = img->getheight();
		int frame_width = width / num_h;

		for (int i = 0; i < num_h; i++)
		{
			Rect rect_src{};
			rect_src.x = i * frame_width, rect_src.y = 0;
			rect_src.w = frame_width, rect_src.h = height;
			frame_list.emplace_back(img, rect_src);
		}
	}
	void add_atlas(Atlas* atlas)
	{
		for (int i = 0; i < atlas->get_size(); i++)
		{
			IMAGE* image = atlas->get_image(i);
			Rect rect_src{};
			rect_src.x = 0, rect_src.y = 0;
			rect_src.w = image->getwidth(), rect_src.h = image->getheight();
			frame_list.emplace_back(image, rect_src);
		}
	}
	void on_update(float delta)
	{
		timer.on_update(delta);
	}
	void on_render(const Camera& camera)
	{
		const Frame& frame = frame_list[idx_frame];
		Rect rect_dst{};
		rect_dst.x = (int)position.x - frame.rect_src.w / 2;
		rect_dst.y = (anchor_mode == AnchorMode::Centered)
			? (int)position.y - frame.rect_src.h / 2 : (int)position.y - frame.rect_src.h;
		rect_dst.w = frame.rect_src.w, rect_dst.h = frame.rect_src.h;
		putimage_ex(camera, frame.image, &rect_dst, &frame.rect_src);
	}
private:
	struct Frame
	{
		Rect rect_src;
		IMAGE* image = nullptr;
		Frame() = default;
		Frame(IMAGE* image, const Rect& rect_src)
			:image(image), rect_src(rect_src) {
		}

		~Frame() = default;
	};
private:
	Timer timer;
	Vector2 position;
	bool is_loop = false;
	size_t idx_frame = 0;
	std::vector<Frame> frame_list;
	std::function<void()> on_finished;
	AnchorMode anchor_mode = AnchorMode::Centered;
};

