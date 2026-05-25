#pragma once
#include"Animation.h"
#include<unordered_map>
#include<String>
class RssourcesManager
{
public:
	static RssourcesManager* instance();

	void on_load();
	Atlas* find_atlas(const std::string& id);
	IMAGE* find_image(const std::string& id);

private:
	static RssourcesManager* manager;
	std::unordered_map<std::string, Atlas*> atlas_pool;
	std::unordered_map<std::string, IMAGE*> image_pool;
private:
	RssourcesManager();
	~RssourcesManager();

};
