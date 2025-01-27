#include "config.h"
#include "objectgraphics.h"
#include "soundhandler.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <fstream>

namespace Config
{
	static bool zombiemassacremode = false;
	static std::string selectedGame = "";

	static int configkeys[KEY_END];
	static int renderwidth;
	static int renderheight;
	static int windowwidth;
	static int windowheight;
	static int32_t focallength;
	static int mousesens;
	static bool autofire;
	static bool godmode;		// cheatmode
	static bool unlimitedlives; // cheatmode
	static bool maxweapon;		// cheatmode
	static int bloodsize;
	static bool debug = false;
	static uint32_t FPS;
	static bool multithread = false;
	static bool vsync = false;
	static bool fullscreen = false;
	static bool switchsticks = false;

	static int sfxvol;
	static int musvol;
	static xmp_context musctx;

	// needed to toggle fullscreen
	static SDL_Window *win;

	static SDL_GameController *controller = nullptr;

	void SetGame(std::string game)
	{
		selectedGame = game;
	}

	std::string GetGamePath()
	{
		return selectedGame;
	}

	void SetDebug(bool b)
	{
		debug = b;
	}

	bool GetDebug()
	{
		return debug;
	}

	void SetFPS(uint32_t f)
	{
		FPS = f;
	}

	uint32_t GetFPS()
	{
		return FPS;
	}

	void SetFullscreen(int f)
	{
		fullscreen = f ? 1 : 0;

		if (fullscreen)
		{
			SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
		}
		else
		{
			SDL_SetWindowFullscreen(win, 0);
		}
	}

	int GetFullscreen()
	{
		return fullscreen ? 1 : 0;
	}

	int GetSwitchSticks()
	{
		return switchsticks ? 1 : 0;
	}

	void SetSwitchSticks(int s)
	{
		switchsticks = (s != 0);
	}

	void SetZM(bool zm)
	{
		zombiemassacremode = zm;
	}

	std::string GetScriptName()
	{
		if (zombiemassacremode)
		{
			std::string result = selectedGame + "/stuf/stages";
			return result;
		}
		else
		{
			std::string result = selectedGame + "/misc/script";
			return result;
		}
	}

	std::string GetMiscDir()
	{
		if (zombiemassacremode)
		{
			std::string result = selectedGame + "/stuf/";
			return result;
		}
		else
		{
			std::string result = selectedGame + "/misc/";
			return result;
		}
	}

	std::string GetPicsDir()
	{
		if (zombiemassacremode)
		{
			std::string result = selectedGame + "/pixs/";
			return result;
		}
		else
		{
			std::string result = selectedGame + "/pics/";
			return result;
		}
	}

	std::string GetLevelDir()
	{
		if (zombiemassacremode)
		{
			std::string result = selectedGame + "/lvls/";
			return result;
		}
		else
		{
			std::string result = selectedGame + "/maps/";
			return result;
		}
	}

	std::string objectfilenames[ObjectGraphics::OGT_END];
	std::string soundfilenames[SoundHandler::SOUND_END];

	std::string GetObjectFilename(ObjectGraphics::ObjectGraphicType i)
	{
		return objectfilenames[i];
	}

	std::string GetGoreFilename(ObjectGraphics::ObjectGraphicType i)
	{
		return objectfilenames[i] + "2";
	}

	std::string GetSoundFilename(SoundHandler::Sounds i)
	{
		return soundfilenames[i];
	}

	std::string GetMusicFilename(int i)
	{
		std::string result;
		if (zombiemassacremode)
		{
			if (i == 0)
				result = selectedGame + "/musi/meda";
			else
				result = selectedGame + "/musi/medb";
		}
		else
		{
			if (i == 0)
				result = selectedGame + "/sfxs/med1";
			else
				result = selectedGame + "/sfxs/med2";
		}

		return result;
	}

	std::string GetMusicDir()
	{
		std::string result;
		if (zombiemassacremode)
		{
			result = selectedGame + "/musi/";
		}
		else
		{
			result = selectedGame + "/sfxs/";
		}

		return result;
	}

	void RegisterMusContext(xmp_context ctx)
	{
		musctx = ctx;
	}

	void RegisterWin(SDL_Window *_win)
	{
		win = _win;
	}

	void Init()
	{
		if (zombiemassacremode)
		{
			// some of this is guesswork, need to check
			objectfilenames[ObjectGraphics::OGT_TOKENS] = selectedGame + "/char/pwrups";
			objectfilenames[ObjectGraphics::OGT_MARINE] = selectedGame + "/char/troopr";
			objectfilenames[ObjectGraphics::OGT_BALDY] = selectedGame + "/char/zombi";
			objectfilenames[ObjectGraphics::OGT_TERRA] = selectedGame + "/char/fatzo";
			objectfilenames[ObjectGraphics::OGT_PHANTOM] = selectedGame + "/char/zomboid";
			objectfilenames[ObjectGraphics::OGT_GHOUL] = selectedGame + "/char/ghost";
			objectfilenames[ObjectGraphics::OGT_DRAGON] = selectedGame + "/char/zombie";
			objectfilenames[ObjectGraphics::OGT_LIZARD] = selectedGame + "/char/skinny";
			objectfilenames[ObjectGraphics::OGT_DEMON] = selectedGame + "/char/zocom";
			objectfilenames[ObjectGraphics::OGT_DEATHHEAD] = selectedGame + "/char/dows-head";
			objectfilenames[ObjectGraphics::OGT_TROLL] = selectedGame + "/char/james";

			// double check these
			soundfilenames[SoundHandler::SOUND_SHOOT] = selectedGame + "/musi/shoot.bin";
			soundfilenames[SoundHandler::SOUND_SHOOT2] = selectedGame + "/musi/shoot2.bin";
			soundfilenames[SoundHandler::SOUND_SHOOT3] = selectedGame + "/musi/shoot3.bin";
			soundfilenames[SoundHandler::SOUND_SHOOT4] = selectedGame + "/musi/shoot4.bin";
			soundfilenames[SoundHandler::SOUND_SHOOT5] = selectedGame + "/musi/shoot5.bin";
			soundfilenames[SoundHandler::SOUND_GRUNT] = selectedGame + "/musi/groan.bin";
			soundfilenames[SoundHandler::SOUND_GRUNT2] = selectedGame + "/musi/groan2.bin";
			soundfilenames[SoundHandler::SOUND_GRUNT3] = selectedGame + "/musi/groan3.bin";
			soundfilenames[SoundHandler::SOUND_GRUNT4] = selectedGame + "/musi/groan4.bin";
			soundfilenames[SoundHandler::SOUND_TOKEN] = selectedGame + "/musi/pwrup.bin";
			soundfilenames[SoundHandler::SOUND_DOOR] = selectedGame + "/musi/door.bin";
			soundfilenames[SoundHandler::SOUND_FOOTSTEP] = selectedGame + "/musi/footstep.bin";
			soundfilenames[SoundHandler::SOUND_DIE] = selectedGame + "/musi/die.bin";
			soundfilenames[SoundHandler::SOUND_SPLAT] = selectedGame + "/musi/splat.bin";
			soundfilenames[SoundHandler::SOUND_TELEPORT] = selectedGame + "/musi/teleport.bin";
			soundfilenames[SoundHandler::SOUND_GHOUL] = selectedGame + "/musi/ghost.bin";
			soundfilenames[SoundHandler::SOUND_LIZARD] = selectedGame + "/musi/skinny.bin";
			soundfilenames[SoundHandler::SOUND_LIZHIT] = selectedGame + "/musi/skihit.bin";
			soundfilenames[SoundHandler::SOUND_TROLLMAD] = selectedGame + "/musi/jamesmad.bin";
			soundfilenames[SoundHandler::SOUND_TROLLHIT] = selectedGame + "/musi/jameshit.bin";
			soundfilenames[SoundHandler::SOUND_ROBOT] = selectedGame + "/musi/fatzo.bin";
			soundfilenames[SoundHandler::SOUND_ROBODIE] = selectedGame + "/musi/fatzdie.bin";
			soundfilenames[SoundHandler::SOUND_DRAGON] = selectedGame + "/musi/zombie.bin";
		}
		else
		{
			objectfilenames[ObjectGraphics::OGT_TOKENS] = selectedGame + "/objs/tokens";
			objectfilenames[ObjectGraphics::OGT_MARINE] = selectedGame + "/objs/marine";
			objectfilenames[ObjectGraphics::OGT_BALDY] = selectedGame + "/objs/baldy";
			objectfilenames[ObjectGraphics::OGT_TERRA] = selectedGame + "/objs/terra";
			objectfilenames[ObjectGraphics::OGT_PHANTOM] = selectedGame + "/objs/phantom";
			objectfilenames[ObjectGraphics::OGT_GHOUL] = selectedGame + "/objs/ghoul";
			objectfilenames[ObjectGraphics::OGT_DRAGON] = selectedGame + "/objs/dragon";
			objectfilenames[ObjectGraphics::OGT_LIZARD] = selectedGame + "/objs/lizard";
			objectfilenames[ObjectGraphics::OGT_DEMON] = selectedGame + "/objs/demon";
			objectfilenames[ObjectGraphics::OGT_DEATHHEAD] = selectedGame + "/objs/deathhead";
			objectfilenames[ObjectGraphics::OGT_TROLL] = selectedGame + "/objs/troll";

			soundfilenames[SoundHandler::SOUND_SHOOT] = selectedGame + "/sfxs/shoot.bin";
			soundfilenames[SoundHandler::SOUND_SHOOT2] = selectedGame + "/sfxs/shoot2.bin";
			soundfilenames[SoundHandler::SOUND_SHOOT3] = selectedGame + "/sfxs/shoot3.bin";
			soundfilenames[SoundHandler::SOUND_SHOOT4] = selectedGame + "/sfxs/shoot4.bin";
			soundfilenames[SoundHandler::SOUND_SHOOT5] = selectedGame + "/sfxs/shoot5.bin";
			soundfilenames[SoundHandler::SOUND_GRUNT] = selectedGame + "/sfxs/grunt.bin";
			soundfilenames[SoundHandler::SOUND_GRUNT2] = selectedGame + "/sfxs/grunt2.bin";
			soundfilenames[SoundHandler::SOUND_GRUNT3] = selectedGame + "/sfxs/grunt3.bin";
			soundfilenames[SoundHandler::SOUND_GRUNT4] = selectedGame + "/sfxs/grunt4.bin";
			soundfilenames[SoundHandler::SOUND_TOKEN] = selectedGame + "/sfxs/token.bin";
			soundfilenames[SoundHandler::SOUND_DOOR] = selectedGame + "/sfxs/door.bin";
			soundfilenames[SoundHandler::SOUND_FOOTSTEP] = selectedGame + "/sfxs/footstep.bin";
			soundfilenames[SoundHandler::SOUND_DIE] = selectedGame + "/sfxs/die.bin";
			soundfilenames[SoundHandler::SOUND_SPLAT] = selectedGame + "/sfxs/splat.bin";
			soundfilenames[SoundHandler::SOUND_TELEPORT] = selectedGame + "/sfxs/teleport.bin";
			soundfilenames[SoundHandler::SOUND_GHOUL] = selectedGame + "/sfxs/ghoul.bin";
			soundfilenames[SoundHandler::SOUND_LIZARD] = selectedGame + "/sfxs/lizard.bin";
			soundfilenames[SoundHandler::SOUND_LIZHIT] = selectedGame + "/sfxs/lizhit.bin";
			soundfilenames[SoundHandler::SOUND_TROLLMAD] = selectedGame + "/sfxs/trollmad.bin";
			soundfilenames[SoundHandler::SOUND_TROLLHIT] = selectedGame + "/sfxs/trollhit.bin";
			soundfilenames[SoundHandler::SOUND_ROBOT] = selectedGame + "/sfxs/robot.bin";
			soundfilenames[SoundHandler::SOUND_ROBODIE] = selectedGame + "/sfxs/robodie.bin";
			soundfilenames[SoundHandler::SOUND_DRAGON] = selectedGame + "/sfxs/dragon.bin";
		}

		configkeys[KEY_SHOOT] = SDL_SCANCODE_LCTRL;
		configkeys[KEY_UP] = SDL_SCANCODE_UP;
		configkeys[KEY_DOWN] = SDL_SCANCODE_DOWN;
		configkeys[KEY_LEFT] = SDL_SCANCODE_LEFT;
		configkeys[KEY_RIGHT] = SDL_SCANCODE_RIGHT;
		configkeys[KEY_SLEFT] = SDL_SCANCODE_A;
		configkeys[KEY_SRIGHT] = SDL_SCANCODE_D;
		configkeys[KEY_STRAFEMOD] = SDL_SCANCODE_LALT;

		renderwidth = 320;
		renderheight = 256;
		windowwidth = 960;
		windowheight = 768;

		focallength = 128;

		mousesens = 5;
		bloodsize = 2;

		multithread = false;
		debug = false;
		vsync = false;
		fullscreen = false;
		switchsticks = false;

		musvol = 5;
		sfxvol = 5;

		autofire = false;

		godmode = false;		// cheatmode
		unlimitedlives = false; // cheatmode
		maxweapon = false;		// cheatmode

		for (int i = 0; i < SDL_NumJoysticks(); ++i)
		{
			if (SDL_IsGameController(i))
			{
				controller = SDL_GameControllerOpen(i);
				break;
			}
		}

		std::ifstream file;

		file.open("config.txt");

		if (file.is_open())
		{
			while (!file.eof())
			{
				std::string line;

				std::getline(file, line);

				if (line.size() && (line[0] != ';'))
				{
					std::string command = line.substr(0, line.find(" "));
					line = line.substr(line.find(" ") + 1, std::string::npos);

					// std::cout << "\"" << line << "\"" << std::endl;

					if (command == "keys")
					{
						for (int i = 0; i < KEY_END; i++)
						{
							std::string val = line.substr(0, line.find(" "));

							configkeys[i] = std::stoi(val);

							if ((i + 1) < KEY_END)
							{
								line = line.substr(line.find(" ") + 1, std::string::npos);
							}
						}
					}
					if (command == "rendersize")
					{
						renderwidth = std::stoi(line.substr(0, line.find(" ")));
						renderheight = std::stoi(line.substr(line.find(" ") + 1, std::string::npos));
					}
					if (command == "windowsize")
					{
						windowwidth = std::stoi(line.substr(0, line.find(" ")));
						windowheight = std::stoi(line.substr(line.find(" ") + 1, std::string::npos));
					}
					if (command == "focallength")
					{
						focallength = std::stoi(line);
					}
					if (command == "mousesensitivity")
					{
						mousesens = std::stoi(line);
					}
					if (command == "bloodsize")
					{
						bloodsize = std::stoi(line);
					}
					if (command == "sfxvol")
					{
						sfxvol = std::stoi(line);
					}
					if (command == "musvol")
					{
						musvol = std::stoi(line);
					}
					if (command == "multithread")
					{
						multithread = std::stoi(line) != 0;
					}
					if (command == "vsync")
					{
						vsync = std::stoi(line) != 0;
					}
					if (command == "fullscreen")
					{
						fullscreen = std::stoi(line) != 0;
					}
					if (command == "autofire")
					{
						autofire = std::stoi(line) != 0;
					}

					// cheatmode
					if (command == "godmode")
					{
						godmode = std::stoi(line) != 0;
					}
					if (command == "unlimitedlives")
					{
						unlimitedlives = std::stoi(line) != 0;
					}
					if (command == "maxweapon")
					{
						maxweapon = std::stoi(line) != 0;
					}
					// ---
				}
			}

			file.close();
		}
	}

	int GetKey(keyenum k)
	{
		return configkeys[k];
	}

	void SetKey(keyenum k, int newval)
	{
		configkeys[k] = newval;
	}

	int GetMouseSens()
	{
		return mousesens;
	}

	void SetMouseSens(int sens)
	{
		mousesens = sens;
	}

	int GetBlood()
	{
		return bloodsize;
	}

	void SetBlood(int b)
	{
		bloodsize = b;
	}

	int GetMT()
	{
		return multithread ? 1 : 0;
	}

	void SetMT(int s)
	{
		multithread = (s != 0);
	}

	bool GetVSync()
	{
		return vsync;
	}

	int GetSFXVol()
	{
		return sfxvol;
	}

	void SetSFXVol(int vol)
	{
		sfxvol = vol;
		Mix_Volume(-1, vol * 12);
	}

	int GetMusicVol()
	{
		return musvol;
	}

	int GetAutoFire()
	{
		return autofire ? 1 : 0;
	}

	void SetAutoFire(int a)
	{
		autofire = (a != 0);
	}

	// cheatmode
	int GetGM()
	{
		return godmode ? 1 : 0;
	}
	int GetUL()
	{
		return unlimitedlives ? 1 : 0;
	}
	int GetMW()
	{
		return maxweapon ? 1 : 0;
	}

	void SetGM(int s)
	{
		godmode = (s != 0);
	}
	void SetUL(int s)
	{
		unlimitedlives = (s != 0);
	}
	void SetMW(int s)
	{
		maxweapon = (s != 0);
	}
	// ---

	bool HaveController()
	{
	    return controller != nullptr;
	}

	Sint16 GetControllerRot()
	{
		return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
	}

	Sint16 GetControllerY()
	{
		return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
	}

	Sint16 GetControllerX()
	{
		return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
	}

	bool GetControllerFire()
	{
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
			return true;
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
			return true;
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X))
			return true;
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y))
			return true;

		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSTICK))
			return true;
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
			return true;
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
			return true;
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
			return true;

		if (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 8000)
			return true;
		if (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 8000)
			return true;

		return false;
	}

	// just for menus
	bool GetControllerDown()
	{
		return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) != 0;
	}
	bool GetControllerUp()
	{
		return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) != 0;
	}
	bool GetControllerLeft()
	{
		return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) != 0;
	}
	bool GetControllerRight()
	{
		return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) != 0;
	}
	bool GetControllerStart()
	{
		return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START) != 0;
	}
	bool GetControllerBack()
	{
		return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK) != 0;
	}

	void SetMusicVol(int vol)
	{
		musvol = vol;
		// this does not seem to work with Hook'ed audio? Can't find any documentation explicitly forbidding it
		// Mix_VolumeMusic(vol * 12);
		for (int i = 0; i < XMP_MAX_CHANNELS; i++)
		{
			xmp_channel_vol(musctx, i, vol * 7);
		}
	}

	void Save()
	{
		if (controller)
		{
			SDL_GameControllerClose(controller);
		}

		std::ofstream file;

		file.open("config.txt");

		if (file.is_open())
		{
			file << ";ZGloom config\n\n";

			file << ";SDL keyvals, up/down/left/right/strafeleft/straferight/strafemod/shoot\n";
			file << "keys ";

			for (int i = 0; i < KEY_END; i++)
			{
				file << configkeys[i];

				if ((i + 1) != KEY_END)
				{
					file << " ";
				}
			}

			file << "\n";

			file << ";The size of the game render bitmap. Bumping this up may lead to more overflow issues in the renderer. But you can get, say, 16:9 by using 460x256 or something in a larger window\n";
			file << "rendersize " << renderwidth << " " << renderheight << "\n";

			file << ";The size of the actual window/fullscreen res. Guess this should be a multiple of the above for pixel perfect\n";
			file << "windowsize " << windowwidth << " " << windowheight << "\n";

			file << ";vsync on or off?\n";
			file << "vsync " << (vsync ? 1 : 0) << "\n";

			file << ";fullscreen on or off?\n";
			file << "fullscreen " << (fullscreen ? 1 : 0) << "\n";

			file << ";focal length. Original used 128 for a 320x256 display, bump this up for higher resolution. Rule of thumb: for 90degree fov, = renderwidth/2\n";
			file << "focallength " << focallength << "\n";

			file << ";Mouse sensitivity\n";
			file << "mousesensitivity " << mousesens << "\n";

			file << ";size of blood splatters in pixels\n";
			file << "bloodsize " << bloodsize << "\n";

			file << ";audio volumes\n";
			file << "sfxvol " << sfxvol << "\n";
			file << "musvol " << musvol << "\n";

			file << ";multithreaded renderer (somewhat experimental)\n";
			file << "multithread " << (multithread ? 1 : 0) << "\n";

			file << ";rapidfire?\n";
			file << "autofire " << (autofire ? 1 : 0) << "\n";

			// cheatmode
			file << "\n;Cheatmode?\n";
			file << "godmode " << (godmode ? 1 : 0) << "\n";
			file << "unlimitedlives " << (unlimitedlives ? 1 : 0) << "\n";
			file << "maxweapon " << (maxweapon ? 1 : 0) << "\n";
			// ---

			file.close();
		}
	}

	void GetRenderSizes(int &rw, int &rh, int &ww, int &wh)
	{
		rw = renderwidth;
		rh = renderheight;
		ww = windowwidth;
		wh = windowheight;
	}

	int32_t GetFocalLength()
	{
		return focallength;
	}
}
