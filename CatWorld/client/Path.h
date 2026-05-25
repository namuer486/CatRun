#pragma once
#include"Vector2.h"
#include<vector>
class Path
{
public:
	Path(const std::vector<Vector2>& point_list)
	{
		this->point_list = point_list;
		for (size_t i = 1; i < point_list.size(); i++)
		{
			float point_len = (point_list[i - 1] - point_list[i]).length();
			world_length += point_len;
			length_list.push_back(point_len);
		}

	}
	~Path() = default;

	Vector2 get_prograss_pos(float prograss) const
	{
		if (prograss <= 0)	return point_list.front();
		if (prograss >= 1)	return point_list.back();
		float prograss_len = prograss * world_length;
		float temp_len = 0;
		for (size_t i = 1; i < point_list.size(); i++)
		{
			temp_len += length_list[i-1];
			if (temp_len >= prograss_len)
			{
				float temp_prograss = (prograss_len - (temp_len - length_list[i - 1])) / length_list[i - 1];
				return (point_list[i] - point_list[i - 1]) * temp_prograss + point_list[i - 1];
			}
		}
		return point_list.back();
	}



private:
	float world_length = 0;
	std::vector<Vector2> point_list;
	std::vector<float> length_list;


};