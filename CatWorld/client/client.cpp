#include"../thirdparty/httplib.h"
#include"RssourcesManager.h"
#include"Player.h"
#include"Path.h"
#include<graphics.h>
#include<atomic>
#include<chrono>
#include<thread>
#include<codecvt>

enum class GameType
{
	wait,
	ready,
	begin
};
int val_start = 4;
GameType current_type = GameType::wait;

bool is_hajimi = false;
int id_player = 0;
std::atomic<int> prograss_1 = -1;
std::atomic<int> prograss_2 = -1;
int num_string = 0;

std::string str_address;//服务器地址
httplib::Client* client = nullptr;//HTTP客户端对象 
int idx_line = 0;
int idx_char = 0;
int num_total_char = 0;
std::string str_text;
std::vector<std::string> str_line_list;//文本单行文本

static void login_to_server(HWND hwnd)
{
	client = new httplib::Client(str_address);
	client->set_keep_alive(true);
	httplib::Result result = client->Post("/login");
	if (!result || result->status != 200)
	{
		MessageBox(hwnd, _T("无法连接启动器"), _T(" 启动失败"), MB_OK | MB_ICONERROR);
		exit(-1);
	}
	id_player = std::stoi(result->body);
	if (id_player < 0)
	{
		MessageBox(hwnd, _T("玩家已满"), _T(" 稍后再试"), MB_OK | MB_ICONERROR);
		exit(-1);
	}
	if (id_player == 1)
	{
		is_hajimi = true;
		prograss_1 = 0;
	}else
	{
		is_hajimi = false;
		prograss_2 = 0;
	}
	str_text = client->Post("/query_text")->body;

	std::stringstream str_stream(str_text);
	std::string str_line;
	while (std::getline(str_stream, str_line))
	{
		str_line_list.push_back(str_line);
		num_total_char += (int)str_line.length();
	}

	std::thread([&]() {
		while (true)
		{
			std::string route = (id_player == 1) ? "/update_1" : "/update_2";
			std::string body = std::to_string((id_player == 1) ? prograss_1 : prograss_2);
			httplib::Result result = client->Post(route, body, "text/plain");
			if (result && result->status == 200)
			{
				int prograss_new = std::stoi(result->body);
				(id_player == 1) ? prograss_2 = prograss_new : prograss_1 = prograss_new;
			}
			std::this_thread::sleep_for(chrono::nanoseconds(1000000000 / 10));
		}
		}).detach();
}

int main(int argc,char** argv)
{
	try {
		RssourcesManager::instance()->on_load();
	}
	catch (LPCTSTR& id) {
		TCHAR err_msg[256];
		_stprintf_s(err_msg, L"资源%s无法加载", id);
		MessageBox(GetHWnd(), err_msg, _T("资源加载错误"),MB_OK|MB_ICONERROR);
	}
	HWND hwnd = initgraph(1280, 720, EW_SHOWCONSOLE);
	settextstyle(28, 0, _T("IPix"));

	bool is_gameover = false;
	ExMessage msg;
	setbkmode(TRANSPARENT);
	Path path({
	{842,842},{1322,842},{1322,442},
	{2762,442},{2762,842},{3162,842},
	{3162,1722},{2122,1722},{2122,1562},
	{842,1562}});

	login_to_server(hwnd);

	Player player1(true);
	Player player2(false);
	player1.set_pos({ 842,842 });
	player2.set_pos({ 842,842 });

	Camera camera, camera_ui;
	camera_ui.reset();
	camera.set_size({ 1280,720 });
	camera_ui.set_size({ 1280,720 });
	Timer timer;

	timer.set_wait(1.0f);
	timer.set_one_shot(false);
	timer.set_callback([&]() {
		val_start--;
		switch (val_start)
		{
		case 3:
			play_audio(_T("ui_3"), false);
			break;
		case 2:
			play_audio(_T("ui_2"), false);
			break;
		case 1:
			play_audio(_T("ui_1"), false);
			break;
		case -1:
			play_audio(_T("bgm"), false);
			current_type = GameType::begin;
			break;
		}
		
		});
	const chrono::nanoseconds frame_during(1000000000 / 144);
	chrono::steady_clock::time_point last_tick = chrono::steady_clock::now();
	BeginBatchDraw();
	
	while (!is_gameover)
	{
		while (peekmessage(&msg))
		{
			//input
			if (current_type != GameType::begin)
				continue;
			if (msg.message == WM_CHAR && idx_line <= str_line_list.size())
			{
				std::string& line_string = str_line_list[idx_line];
				if (msg.ch == line_string[idx_char])
				{
					switch (range_random(4, 1))
					{
					case 1:
						play_audio(_T("click_1"), false);
						break;
					case 2:
						play_audio(_T("click_2"), false);
						break;
					case 3:
						play_audio(_T("click_3"), false);
						break;
					case 4:
						play_audio(_T("click_4"), false);
						break;
					}
					(id_player == 1) ? prograss_1++ : prograss_2++;
					idx_char++;
					if (idx_char >= line_string.size())
					{
						idx_char = 0;
						idx_line++;
					}
				}
			}
		}
		//on_update
		chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
		chrono::duration<float> delta = chrono::duration<float>(start_time - last_tick);
		if (current_type == GameType::wait)
		{
			if (prograss_1 >= 0 && prograss_2 >= 0)
			{
				printf("切换状态");
				current_type = GameType::ready;
			}
		}
		else
		{
			if (current_type == GameType::ready)
				timer.on_update(delta.count());
			//胜利结算
			if ((id_player == 1 && prograss_1 >= num_total_char)|| (id_player == 2 && prograss_2 >= num_total_char))
			{
				stop_audio(_T("bgm"));
				play_audio((id_player == 1) ? _T("1p_win") : _T("2p_win"), false);
				MessageBox(hwnd, _T("棋高一筹"), _T("你赢了"), MB_OK | MB_ICONERROR);
			}
			else if((id_player == 1 && prograss_2 >= num_total_char) || (id_player == 2 && prograss_1 >= num_total_char))
			{
				stop_audio(_T("bgm"));
				play_audio((id_player == 1) ? _T("1p_win") : _T("2p_win"), false);
				MessageBox(hwnd, _T("再接再厉"), _T("你输了"), MB_OK | MB_ICONERROR);
			}
			player1.set_real_pos(path.get_prograss_pos((float)prograss_1/ num_total_char));
			player2.set_real_pos(path.get_prograss_pos((float)prograss_2/ num_total_char));

			player1.on_update(delta.count());
			player2.on_update(delta.count());

			camera.look_at((id_player == 1) ? player1.get_pos() : player2.get_pos());
		}
		settextcolor(RGB(0, 0, 0));
		cleardevice();
		//on_render
		if (current_type == GameType::wait)
		{
			settextcolor(RGB(195, 195, 195));
			outtextxy(15, 675, _T("比赛即将开始，等待玩家加入"));
		}
		else
		{
			//背景
			static Rect rect_bg =
			{
				0,0,
				RssourcesManager::instance()->find_image("background")->getwidth(),
				RssourcesManager::instance()->find_image("background")->getheight(),
			};
			putimage_ex(camera, RssourcesManager::instance()->find_image("background"), &rect_bg);
			//玩家
			if (player1.get_pos().y >= player2.get_pos().y)
			{
				player2.on_render(camera);
				player1.on_render(camera);
			}
			else
			{
				player1.on_render(camera);
				player2.on_render(camera);
			}
			//倒计时
			switch (val_start)
			{
			case 3:
			{
				static Rect rect_ui_3 =
				{
					1280 / 2 - RssourcesManager::instance()->find_image("ui_3")->getwidth() / 2,
					720 / 2 - RssourcesManager::instance()->find_image("ui_3")->getheight() / 2,
					 RssourcesManager::instance()->find_image("ui_3")->getwidth(),
					 RssourcesManager::instance()->find_image("ui_3")->getheight(),
				};
				putimage_ex(camera_ui, RssourcesManager::instance()->find_image("ui_3"), &rect_ui_3);
			}
			break;
			case 2:
			{
				static Rect rect_ui_2 =
				{
					1280 / 2 - RssourcesManager::instance()->find_image("ui_2")->getwidth() / 2,
					720 / 2 - RssourcesManager::instance()->find_image("ui_2")->getheight() / 2,
					 RssourcesManager::instance()->find_image("ui_2")->getwidth(),
					 RssourcesManager::instance()->find_image("ui_2")->getheight(),
				};
				putimage_ex(camera_ui, RssourcesManager::instance()->find_image("ui_2"), &rect_ui_2);
			}
			break;
			case 1:
			{
				static Rect rect_ui_1 =
				{
					1280 / 2 - RssourcesManager::instance()->find_image("ui_1")->getwidth() / 2,
					720 / 2 - RssourcesManager::instance()->find_image("ui_1")->getheight() / 2,
					 RssourcesManager::instance()->find_image("ui_1")->getwidth(),
					 RssourcesManager::instance()->find_image("ui_1")->getheight(),
				};
				putimage_ex(camera_ui, RssourcesManager::instance()->find_image("ui_1"), &rect_ui_1);
			}
			break;
			case 0:
			{
				static Rect rect_ui_0 =
				{
					1280 / 2 - RssourcesManager::instance()->find_image("ui_fight")->getwidth() / 2,
					720 / 2 - RssourcesManager::instance()->find_image("ui_fight")->getheight() / 2,
					 RssourcesManager::instance()->find_image("ui_fight")->getwidth(),
					 RssourcesManager::instance()->find_image("ui_fight")->getheight(),
				};
				putimage_ex(camera_ui, RssourcesManager::instance()->find_image("ui_fight"), &rect_ui_0);
			}
			break;
			default:break;
			}

			//绘制文本界面
			if (current_type == GameType::begin)
			{
				static Rect rect_text_box =
				{
					0,
					720 - RssourcesManager::instance()->find_image("ui_textbox")->getheight(),
					 RssourcesManager::instance()->find_image("ui_textbox")->getwidth(),
					 RssourcesManager::instance()->find_image("ui_textbox")->getheight(),
				};
				static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
				std::wstring wstr_line = convert.from_bytes(str_line_list[idx_line]);
				std::wstring wstr_completed = convert.from_bytes(str_line_list[idx_line].substr(0, idx_char));
				putimage_ex(camera_ui, RssourcesManager::instance()->find_image("ui_textbox"), &rect_text_box);
				settextcolor(RGB(125, 125, 125));
				outtextxy(185 + 2, rect_text_box.y + 65 + 2, wstr_line.c_str());
				settextcolor(RGB(25, 25, 25));
				outtextxy(185, rect_text_box.y + 65, wstr_line.c_str());
				settextcolor(RGB(0, 149, 217));
				outtextxy(185, rect_text_box.y + 65, wstr_completed.c_str());

			}
		}
		
		FlushBatchDraw();
		last_tick = start_time;
		chrono::nanoseconds  sleep_dur = frame_during - (chrono::steady_clock::now() - start_time);
		if (sleep_dur > chrono::nanoseconds(0))
			std::this_thread::sleep_for(sleep_dur);
	}
	EndBatchDraw();
	return 0;
}