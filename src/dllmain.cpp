#include "dllmain.h"
#include <amethyst/Log.h>
#include <amethyst/Memory.h>
#include <amethyst/runtime/AmethystContext.h>

#define FUNC_ONE_BYTE_OFFSET 0x15C
#define FUNC_TWO_BYTE_OFFSET 0x54B


ModFunction void Initialize(AmethystContext* ctx)
{
	Log::Info("Starting TesterMod...!");
	auto addrOne = SigScanSafe("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B FA 48 8B D9 48 8B 02");
	if (addrOne.has_value())
		Log::Info("Found address: 0x{:X}", addrOne.value());
	else
		Log::Error("Failed to find address!");

	
	auto addrTwo = SigScanSafe("40 53 55 56 57 41 54 41 55 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 0F B6 AC 24 ? ? ? ? 4C 8D B1");
	if (addrTwo.has_value())
		Log::Info("Found address: 0x{:X}", addrTwo.value());
	else
		Log::Error("Failed to find address!");

	uintptr_t addrOneNormal = addrOne.value();
	uintptr_t addrTwoNormal = addrTwo.value();

	uintptr_t addrOneOffset = addrOneNormal + FUNC_ONE_BYTE_OFFSET;
	uintptr_t addrTwoOffset = addrTwoNormal + FUNC_TWO_BYTE_OFFSET;

	DWORD oldProtect;
	if (!UnprotectMemory(addrOneOffset, 1, &oldProtect))
		Log::Error("Failed to unprotect memory!");

	if (!UnprotectMemory(addrTwoOffset, 1, &oldProtect))
		Log::Error("Failed to unprotect memory!");

	Log::Info("Memory position one: 0x{:X}", *(uint8_t*)addrOneOffset);
	Log::Info("Memory position two: 0x{:X}", *(uint8_t*)addrTwoOffset);

	uint8_t replaceVal = 0xFF;
	Amethyst::PatchManager* patchManager = &ctx->mPatchManager;

	patchManager->ApplyPatch(addrOneOffset, &replaceVal, 1);
	patchManager->ApplyPatch(addrTwoOffset, &replaceVal, 1);

	Log::Info("Memory position one: 0x{:X}", *(uint8_t*)addrOneOffset);
	Log::Info("Memory position two: 0x{:X}", *(uint8_t*)addrTwoOffset);


	ProtectMemory(addrOneOffset, 1, oldProtect);
	ProtectMemory(addrTwoOffset, 1, oldProtect);

	

}