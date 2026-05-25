#include"RssourcesManager.h"
#include<fstream>
#include<sstream>
#include<string>
extern std::string str_address;//服务器地址
RssourcesManager* RssourcesManager::manager = nullptr;
RssourcesManager* RssourcesManager::instance()
{
	if (manager == nullptr)
	{
		manager = new RssourcesManager();
	}
	return manager;
}
RssourcesManager::RssourcesManager() = default;
RssourcesManager::~RssourcesManager() = default;

struct imageResinfo
{
	std::string id;
	LPCTSTR path;
};
struct atlasResinfo
{
	std::string id;
	LPCTSTR path;
	int number;
};
const vector < atlasResinfo > atlas_list = {
	{"hajimi_idle_from", _T("resources/hajimi_idle_from_%d.png"), 4},
	{"hajimi_idle_back", _T("resources/hajimi_idle_back_%d.png"), 4},
	{"hajimi_idle_left", _T("resources/hajimi_idle_left_%d.png"), 4},
	{"hajimi_idle_right", _T("resources/hajimi_idle_right_%d.png"), 4},
	{"hajimi_run_from", _T("resources/hajimi_run_from_%d.png"), 4},
	{"hajimi_run_back", _T("resources/hajimi_run_back_%d.png"), 4},
	{"hajimi_run_left", _T("resources/hajimi_run_left_%d.png"), 4},
	{"hajimi_run_right", _T("resources/hajimi_run_right_%d.png"), 4},
	{"manbo_idle_from", _T("resources/manbo_idle_from_%d.png"), 4},
	{"manbo_idle_back", _T("resources/manbo_idle_back_%d.png"), 4},
	{"manbo_idle_left", _T("resources/manbo_idle_left_%d.png"), 4},
	{"manbo_idle_right", _T("resources/manbo_idle_right_%d.png"), 4},
	{"manbo_run_from", _T("resources/manbo_run_from_%d.png"), 4},
	{"manbo_run_back", _T("resources/manbo_run_back_%d.png"), 4},
	{"manbo_run_left", _T("resources/manbo_run_left_%d.png"), 4},
	{"manbo_run_right", _T("resources/manbo_run_right_%d.png"), 4}
};
const vector < imageResinfo > image_list = {
	{"ui_textbox", _T("resources/ui_textbox.png")},
	{"ui_1", _T("resources/ui_1.png")},
	{"ui_2", _T("resources/ui_2.png")},
	{"ui_3", _T("resources/ui_3.png")},
	{"ui_fight", _T("resources/ui_fight.png")},
	{"background", _T("resources/background.png")}
};
static inline bool checkimage(IMAGE* img)
{
	return GetImageBuffer(img);
}
void RssourcesManager::on_load()
{
	for (imageResinfo val : image_list)
	{
		IMAGE* image = new IMAGE();
		loadimage(image, val.path);
		if (!checkimage(image))
			throw val.path;
		image_pool[val.id] = image;
	}

	for (atlasResinfo val : atlas_list)
	{
		Atlas* atlas = new Atlas();
		atlas->load_img_list(val.path, val.number);
		for (int i = 0; i < atlas->get_size(); i++)
		{
			if (!checkimage(atlas->get_image(i)))
				throw val.path;
		}
		atlas_pool[val.id] = atlas;
	}
	load_audio(_T("resources/bgm.mp3"), _T("bgm"));
	load_audio(_T("resources/1p_win.mp3"), _T("1p_win"));
	load_audio(_T("resources/2p_win.mp3"), _T("2p_win"));
	load_audio(_T("resources/click_1.mp3"), _T("click_1"));
	load_audio(_T("resources/click_2.mp3"), _T("click_2"));
	load_audio(_T("resources/click_3.mp3"), _T("click_3"));
	load_audio(_T("resources/click_4.mp3"), _T("click_4"));
	load_audio(_T("resources/ui_1.mp3"), _T("ui_1"));
	load_audio(_T("resources/ui_2.mp3"), _T("ui_2"));
	load_audio(_T("resources/ui_3.mp3"), _T("ui_3"));
	load_audio(_T("resources/ui_fight.mp3"), _T("ui_fight"));
	std::fstream file("config.cfg");
	if (!file.good())
	{
		MessageBox(nullptr, L"无法打开config.cfg", L"启动失败", MB_OK | MB_ICONERROR);
	}
	std::stringstream str_stream;
	str_stream << file.rdbuf();
	str_address = str_stream.str();
	file.close();
}
Atlas* RssourcesManager::find_atlas(const std::string& id)
{
	const auto& atlas_img = atlas_pool.find(id);
	if (atlas_img == atlas_pool.end())
	{
		return NULL;
	}
	return atlas_img->second;
}
IMAGE* RssourcesManager::find_image(const std::string& id)
{
	const auto& img = image_pool.find(id);
	if (img == image_pool.end())
	{
		return NULL;
	}
	return img->second;
}