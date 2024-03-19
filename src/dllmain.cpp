#include "dllmain.h"
#include <amethyst/Log.h>
#include <amethyst/Memory.h>
#include <amethyst/runtime/AmethystContext.h>
#include <vector>
#include <string>


using FoodSaturationFromString_t = float(*)(std::string *str);

void onJoinGame(void*)
{
	uintptr_t FoodSaturationFromStringAddr = SigScan("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC ? 48 8B D9 45 33 E4 BA");

	if (FoodSaturationFromStringAddr)
		Log::Info("Found FoodSaturationFromString at: 0x{:X}", FoodSaturationFromStringAddr);
	else
		Assert("Failed to find FoodSaturationFromString!");

	std::vector<std::pair<std::string, float>> values = {
		{"poor", 0.0f},
		{"low", 0.0f},
		{"normal", 0.0f},
		{"good", 0.0f},
		{"max", 0.0f},
		{"supernatural", 0.0f}
	};

	FoodSaturationFromString_t FoodSaturationFromString = (FoodSaturationFromString_t)FoodSaturationFromStringAddr;

	for (auto& value : values)
	{
		value.second = FoodSaturationFromString(&value.first);
		Log::Info("FoodSaturationFromString(\"{}\") = {}", value.first, value.second);
	}

}

ModFunction void Initialize(AmethystContext* ctx)
{
	Log::Info("Starting TesterMod...!");
	ctx->mEventManager.onStartJoinGame.AddListener(onJoinGame);
}