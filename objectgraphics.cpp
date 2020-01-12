#include "objectgraphics.h"
#include "crmfile.h"
#include "binresources.h"

#include <vector>

static uint16_t Get16(const uint8_t* p)
{
	return (static_cast<uint16_t>(p[0])) << 8 | static_cast<uint16_t>(p[1]);
}

static uint32_t Get32(const uint8_t* p)
{
	return (static_cast<uint16_t>(p[0])) << 24 | (static_cast<uint16_t>(p[1]) << 16) | (static_cast<uint16_t>(p[2])) << 8 | (static_cast<uint16_t>(p[3]) << 0);
}

void Shape::Load(const uint8_t* indata, const uint8_t* palette)
{
	// first 2 entries are handles
	w = Get16(indata + 4);
	h = Get16(indata + 6);

	data.resize(w*h);

	for (uint32_t p = 0; p < w*h; p++)
	{
		uint32_t col = palette[2 * indata[p + 8]] << 8;

		col |= palette[2 * indata[p+8] + 1];

		if (indata[p + 8] == 0)
		{
			//tranparent - pick a colour that can't exist as 444 expanded up
			data[p] = 1;
		}
		else
		{
			uint32_t col32 = (col & 0xf) | ((col & 0xf0) << 4) | ((col & 0xf00) << 8);
			col32 |= col32 << 4;
			data[p] = col32;
		}
	}
}

void Shape::DumpDebug(const char* name)
{
	//dump a ppm file
	FILE* file = fopen(name, "wb");

	fprintf(file, "P6\n%i %i\n255\n", w, h);

	for (uint32_t y = 0; y < h; y++)
	{
		for (uint32_t x = 0; x < w; x++)
		{
			uint32_t col = data[y + x*h];
			fputc((col >>16) & 0xFF, file);
			fputc((col >> 8) & 0xFF, file);
			fputc((col >> 0) & 0xFF, file);
		}
	}

	fclose(file);
}

void ObjectGraphics::LoadGraphicBin(const uint8_t* bindata, std::vector<Shape>& shape)
{
	uint32_t frames;

	frames = Get16(bindata + 2) << Get16(bindata + 0);

	uint32_t maxw, maxh;

	maxw = Get16(bindata + 4);
	maxh = Get16(bindata + 6);

	uint32_t paletteoffset = Get32(bindata + 8);

	std::vector<uint32_t> frameoffsets;

	frameoffsets.resize(frames);

	for (uint32_t f = 0; f < frames; f++)
	{
		frameoffsets[f] = Get32(bindata + 12 + f * 4);
	}

	shape.resize(frames);

	for (uint32_t f = 0; f < frames; f++)
	{
		shape[f].Load(bindata + frameoffsets[f], bindata + paletteoffset);
	}
}


void ObjectGraphics::LoadGraphic(const char* name, std::vector<Shape>& shape)
{
	CrmFile file;

	file.Load(name);

	if (!file.data) return;

	uint32_t frames;

	frames = Get16(file.data + 2) << Get16(file.data + 0);

	uint32_t maxw, maxh;

	maxw = Get16(file.data + 4);
	maxh = Get16(file.data + 6);

	uint32_t paletteoffset = Get32(file.data + 8);

	std::vector<uint32_t> frameoffsets;

	frameoffsets.resize(frames);

	for (uint32_t f = 0; f < frames; f++)
	{
		frameoffsets[f] = Get32(file.data + 12 + f * 4);
	}

	shape.resize(frames);

	for (uint32_t f = 0; f < frames; f++)
	{
		shape[f].Load(file.data + frameoffsets[f], file.data + paletteoffset);
	}
}

ObjectGraphics::ObjectGraphics()
{
	LoadGraphic("objs/tokens", TokenShapes);
	LoadGraphic("objs/marine", MarineShapes);
	LoadGraphic("objs/baldy", BaldyShapes);
	LoadGraphic("objs/terra", TerraShapes);
	LoadGraphic("objs/phantom", PhantomShapes);
	LoadGraphic("objs/ghoul", GhoulShapes);
	LoadGraphic("objs/dragon", DragonShapes);
	LoadGraphic("objs/lizard", LizardShapes);
	LoadGraphic("objs/demon", DemonShapes);
	LoadGraphic("objs/deathhead", DeathheadShapes);
	LoadGraphic("objs/troll", TrollShapes);
	LoadGraphicBin(BinResource::bullet1, BulletShapes[0]);
	LoadGraphicBin(BinResource::bullet2, BulletShapes[1]);
	//
	//for (uint32_t s = 0; s < MarineShapes.size(); s++)
	//{
	//	char name[100];
	//	sprintf(name, "marine%i.ppm", s);
	//
	//	MarineShapes[s].DumpDebug(name);
	//}

	//objectlogic[OLT_PLAYER1].shape = player;
	//objectlogic[OLT_PLAYER2].shape = player;
	objectlogic[OLT_HEALTH].shape = &TokenShapes;
	//objectlogic[OLT_WEAPON].shape = weapon2;
	objectlogic[OLT_THERMO].shape = &TokenShapes;
	objectlogic[OLT_INFRA].shape = &TokenShapes;
	objectlogic[OLT_INVISI].shape = &TokenShapes;
	objectlogic[OLT_INVINC].shape = &TokenShapes;
	objectlogic[OLT_DRAGON].shape = &DragonShapes;
	objectlogic[OLT_BOUNCY].shape = &TokenShapes;
	objectlogic[OLT_MARINE].shape = &MarineShapes;
	objectlogic[OLT_BALDY].shape = &BaldyShapes;
	objectlogic[OLT_TERRA].shape = &TerraShapes;
	objectlogic[OLT_GHOUL].shape = &GhoulShapes;
	objectlogic[OLT_PHANTOM].shape = &PhantomShapes;
	objectlogic[OLT_DEMON].shape = &DemonShapes;
	objectlogic[OLT_WEAPON1].shape = &(BulletShapes[0]); // TODO: sparks and that
	objectlogic[OLT_WEAPON2].shape = &(BulletShapes[1]); // TODO: sparks and that;
	objectlogic[OLT_WEAPON3].shape = &(BulletShapes[0]); // TODO: sparks and that;
	objectlogic[OLT_WEAPON4].shape = &(BulletShapes[0]); // TODO: sparks and that;
	objectlogic[OLT_WEAPON5].shape = &(BulletShapes[0]); // TODO: sparks and that;
	objectlogic[OLT_LIZARD].shape = &LizardShapes;
	objectlogic[OLT_DEATHHEAD].shape = &DeathheadShapes;
	objectlogic[OLT_TROLL].shape = &TrollShapes;

	objectlogic[OLT_PLAYER1].frame	=0x0	  ;
	objectlogic[OLT_PLAYER2].frame	=0x0	  ;
	objectlogic[OLT_HEALTH].frame = 0x20000  ;
	objectlogic[OLT_WEAPON].frame	=0x0	  ;
	objectlogic[OLT_THERMO].frame = 0x00000  ;
	objectlogic[OLT_INFRA].frame = 0x00000  ;
	objectlogic[OLT_INVISI].frame = 0x10000  ;
	objectlogic[OLT_INVINC].frame = 0x20000  ;
	objectlogic[OLT_DRAGON].frame = 0x0	  ;
	objectlogic[OLT_BOUNCY].frame = 0x30000  ;
	objectlogic[OLT_MARINE].frame = 0x0	  ;
	objectlogic[OLT_BALDY].frame = 0x0	  ;
	objectlogic[OLT_TERRA].frame = 0x0	  ;
	objectlogic[OLT_GHOUL].frame = 0x0	  ;
	objectlogic[OLT_PHANTOM].frame = 0x0	  ;
	objectlogic[OLT_DEMON].frame = 0x0	  ;
	objectlogic[OLT_WEAPON1].frame = 0x0	  ;
	objectlogic[OLT_WEAPON2].frame = 0x0	  ;
	objectlogic[OLT_WEAPON3].frame = 0x0	  ;
	objectlogic[OLT_WEAPON4].frame = 0x0	  ;
	objectlogic[OLT_WEAPON5].frame = 0x0	  ;
	objectlogic[OLT_LIZARD].frame = 0x0	  ;
	objectlogic[OLT_DEATHHEAD].frame = 0x8000;
	objectlogic[OLT_TROLL].frame = 0x0	  ;

	// how many rotation frames
	objectlogic[OLT_PLAYER1].render = 8;
	objectlogic[OLT_PLAYER2].render = 8;
	objectlogic[OLT_HEALTH].render = 1;
	objectlogic[OLT_WEAPON].render = 1;
	objectlogic[OLT_THERMO].render = 1;
	objectlogic[OLT_INFRA].render = 1;
	objectlogic[OLT_INVISI].render = 1;
	objectlogic[OLT_INVINC].render = 1;
	objectlogic[OLT_DRAGON].render = 8;
	objectlogic[OLT_BOUNCY].render = 1;
	objectlogic[OLT_MARINE].render = 8;
	objectlogic[OLT_BALDY].render = 8;
	objectlogic[OLT_TERRA].render = 8;
	objectlogic[OLT_GHOUL].render = 8;
	objectlogic[OLT_PHANTOM].render = 8;
	objectlogic[OLT_DEMON].render = 8;
	objectlogic[OLT_WEAPON1].render = 1;
	objectlogic[OLT_WEAPON2].render = 1;
	objectlogic[OLT_WEAPON3].render = 1;
	objectlogic[OLT_WEAPON4].render = 1;
	objectlogic[OLT_WEAPON5].render = 1;
	objectlogic[OLT_LIZARD].render = 8;
	objectlogic[OLT_DEATHHEAD].render = 8;
	objectlogic[OLT_TROLL].render = 8;

	objectlogic[OLT_PLAYER1].movspeed = 0xd0000	 ;
	objectlogic[OLT_PLAYER2].movspeed = 0xd0000	 ;
	objectlogic[OLT_HEALTH].movspeed = 0		 ;
	objectlogic[OLT_WEAPON].movspeed = 0		 ;
	objectlogic[OLT_THERMO].movspeed = 0		 ;
	objectlogic[OLT_INFRA].movspeed = 0			 ;
	objectlogic[OLT_INVISI].movspeed = 0		 ;
	objectlogic[OLT_INVINC].movspeed = 0		 ;
	objectlogic[OLT_DRAGON].movspeed = 0xc0000	 ;
	objectlogic[OLT_BOUNCY].movspeed = 0		 ;
	objectlogic[OLT_MARINE].movspeed = 0x60000	 ;
	objectlogic[OLT_BALDY].movspeed = 0x40000	 ;
	objectlogic[OLT_TERRA].movspeed = 0x20000	 ;
	objectlogic[OLT_GHOUL].movspeed = 0x80000	 ;
	objectlogic[OLT_PHANTOM].movspeed = 0xa0000	 ;
	objectlogic[OLT_DEMON].movspeed = 0x70000	 ;
	objectlogic[OLT_WEAPON1].movspeed = 0		 ;
	objectlogic[OLT_WEAPON2].movspeed = 0		 ;
	objectlogic[OLT_WEAPON3].movspeed = 0		 ;
	objectlogic[OLT_WEAPON4].movspeed = 0		 ;
	objectlogic[OLT_WEAPON5].movspeed = 0		 ;
	objectlogic[OLT_LIZARD].movspeed = 0x60000	 ;
	objectlogic[OLT_DEATHHEAD].movspeed = 0xc0000 ;
	objectlogic[OLT_TROLL].movspeed = 0x60000	 ;

	objectlogic[OLT_PLAYER1].framespeed = 0x6000;
	objectlogic[OLT_PLAYER2].framespeed = 0x6000;
	objectlogic[OLT_HEALTH].framespeed = 0x0;
	objectlogic[OLT_WEAPON].framespeed = 0x08000;
	objectlogic[OLT_THERMO].framespeed = 0x0;
	objectlogic[OLT_INFRA].framespeed = 0x0;
	objectlogic[OLT_INVISI].framespeed = 0x0;
	objectlogic[OLT_INVINC].framespeed = 0x0;
	objectlogic[OLT_DRAGON].framespeed = 0x4000;
	objectlogic[OLT_BOUNCY].framespeed = 0x0;
	objectlogic[OLT_MARINE].framespeed = 0x6000;
	objectlogic[OLT_BALDY].framespeed = 0x4000;
	objectlogic[OLT_TERRA].framespeed = 0x6000;
	objectlogic[OLT_GHOUL].framespeed = 0x0;
	objectlogic[OLT_PHANTOM].framespeed = 0xa000;
	objectlogic[OLT_DEMON].framespeed = 0x7000;
	objectlogic[OLT_WEAPON1].framespeed = 0x08000;
	objectlogic[OLT_WEAPON2].framespeed = 0x08000;
	objectlogic[OLT_WEAPON3].framespeed = 0x08000;
	objectlogic[OLT_WEAPON4].framespeed = 0x08000;
	objectlogic[OLT_WEAPON5].framespeed = 0x08000;
	objectlogic[OLT_LIZARD].framespeed = 0x4000;
	objectlogic[OLT_DEATHHEAD].framespeed = 0x6000;
	objectlogic[OLT_TROLL].framespeed = 0x4000;

	objectlogic[OLT_PLAYER1].firey = -60;
	objectlogic[OLT_PLAYER2].firey = -60;
	objectlogic[OLT_HEALTH].firey	= 0;
	objectlogic[OLT_WEAPON].firey	= 0;
	objectlogic[OLT_THERMO].firey	= 0;
	objectlogic[OLT_INFRA].firey	= 0;
	objectlogic[OLT_INVISI].firey	= 0;
	objectlogic[OLT_INVINC].firey	= 0;
	objectlogic[OLT_DRAGON].firey = -144;
	objectlogic[OLT_BOUNCY].firey =	0;
	objectlogic[OLT_MARINE].firey = -60;
	objectlogic[OLT_BALDY].firey = -60;
	objectlogic[OLT_TERRA].firey = -60;
	objectlogic[OLT_GHOUL].firey = -64;
	objectlogic[OLT_PHANTOM].firey = -60;
	objectlogic[OLT_DEMON].firey = -90;
	objectlogic[OLT_WEAPON1].firey = 0;
	objectlogic[OLT_WEAPON2].firey = 0;
	objectlogic[OLT_WEAPON3].firey = 0;
	objectlogic[OLT_WEAPON4].firey = 0;
	objectlogic[OLT_WEAPON5].firey = 0;
	objectlogic[OLT_LIZARD].firey = -60;
	objectlogic[OLT_DEATHHEAD].firey =  -60;
	objectlogic[OLT_TROLL].firey = -60;

	return;
}