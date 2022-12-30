#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include "LuaState.hpp"
#include "Tools.hpp"


namespace Lua
{
	void decrement_top(uintptr_t rl)
	{
		*(uintptr_t*)(rl + Offsets::lua_state_top) -= 16;
	}


	void increment_top(uintptr_t rl)
	{
		*(uintptr_t*)(rl + Offsets::lua_state_top) += 16;
	}


	void setlevel(uintptr_t rl, int8_t identity)
	{
		*(int8_t*)(*(uintptr_t*)(rl + 72) + 24) = identity;
	}


    int r_luam_newgco(uintptr_t rl, size_t nsize, uint8_t memcat)
    {
        auto global = Deobfuscation::global(rl);
        auto frealloc = *reinterpret_cast<uintptr_t(__cdecl**)(uintptr_t, uintptr_t, uintptr_t, size_t)>(global + 12);
        auto block = frealloc(*reinterpret_cast<uintptr_t*>(global + 16), 0, 0, nsize);
        *reinterpret_cast<uintptr_t*>(global + 44) += nsize; //44 totalbytes
        *reinterpret_cast<uintptr_t*>(global + 4 * memcat + 320) += nsize; //320 memcatbytes
        return block;
    }
}


namespace IDA
{
    int r_lua_newthread(uintptr_t rl)
    {
        int v13;
        auto v55 = *reinterpret_cast<BYTE*>(rl + 4);
        auto v10 = Lua::r_luam_newgco(rl, 76, v55);
        v13 = *reinterpret_cast<BYTE*>(((rl + 16) ^ *reinterpret_cast<DWORD*>(rl + 16)) + 0x14) & 3;
        *reinterpret_cast<BYTE*>(v10 + 2) = 8;
        *reinterpret_cast<BYTE*>(v10 + 1) = v13;
        v13 = *reinterpret_cast<BYTE*>(rl + 4);
        *reinterpret_cast<BYTE*>(v10) = v13;
        auto v14 = *reinterpret_cast<DWORD*>(rl + 16);
        *reinterpret_cast<DWORD*>(v10 + 60) = 0;
        *reinterpret_cast<DWORD*>(v10 + 16) = (rl + 16) ^ v14 ^ (v10 + 16);
        *reinterpret_cast<DWORD*>(v10 + 28) = 0;
        *reinterpret_cast<DWORD*>(v10 + 40) = v10 + 40;
        *reinterpret_cast<DWORD*>(v10 + 3) = 0;
        *reinterpret_cast<DWORD*>(v10 + 64) = 0;
        *reinterpret_cast<DWORD*>(v10 + 44) = 0;
        *reinterpret_cast<DWORD*>(v10 + 48) = 0;
        *reinterpret_cast<DWORD*>(v10 + 12) = 0;
        *reinterpret_cast<DWORD*>(v10 + 36) = 0;
        *reinterpret_cast<DWORD*>(v10 + 68) = 0;
        *reinterpret_cast<DWORD*>(v10 + 52) = 0;
        *reinterpret_cast<DWORD*>(v10 + 72) = 0;
        *reinterpret_cast<BYTE*>(v10 + 4) = *reinterpret_cast<BYTE*>(rl + 4);
        auto v15 = (rl + 16) ^ *reinterpret_cast<DWORD*>(rl + 16);
        auto  v54 = v15;
        auto v16 = Lua::r_luam_newgco(rl, 195, v55);
        auto  v17 = v16;
        auto v18 = v54;
        auto v19 = v55;
        *reinterpret_cast<DWORD*>(v10 + 32) = v17 + 168;
        v19 = *reinterpret_cast<BYTE*>(v10);
        *reinterpret_cast<DWORD*>(v10 + 36) = v17;
        *reinterpret_cast<DWORD*>(v10 + 12) = v17;
        v55 = v19;
        *reinterpret_cast<DWORD*>(v10 + 44) = 8;
        v54 = (rl + 16) ^ *reinterpret_cast<DWORD*>(rl + 16);
        auto v20 = Lua::r_luam_newgco(rl, 720, v55);
        auto v23 = v20;
        *reinterpret_cast<DWORD*>(v10 + 28) = v20;
        auto v24 = 45;
        *reinterpret_cast<DWORD*>(v10 + 40) = (v10 + 40) ^ 0x2D;
        auto v25 = (DWORD*)(v20 + 12);
        do
        {
            *v25 = 0;
            v25 += 4;
            --v24;
        } while (v24);
        *reinterpret_cast<DWORD*>(v10 + 20) = v23;
        *reinterpret_cast<DWORD*>(v10 + 24) = v23 + 16 * (((v10 + 40) ^ *reinterpret_cast<DWORD*>(v10 + 40)) - 5);
        *reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(v10 + 12) + 4) = v23;
        *reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(v10 + 20) + 12) = 0;
        *reinterpret_cast<DWORD*>(v10 + 20) += 16;
        *reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(v10 + 12) + 12) = *reinterpret_cast<DWORD*>(v10 + 20);
        auto v26 = *reinterpret_cast<DWORD*>(v10 + 12);
        *reinterpret_cast<DWORD*>(v10 + 8) = *reinterpret_cast<DWORD*>(v26 + 12);
        *reinterpret_cast<DWORD*>(v26) = *reinterpret_cast<DWORD*>(v10 + 20) + 320;
        *reinterpret_cast<DWORD*>(v10 + 64) = *reinterpret_cast<DWORD*>(rl + 64);
        *reinterpret_cast<BYTE*>(v10 + 6) = *reinterpret_cast<BYTE*>(rl + 6);
        auto v27 = *reinterpret_cast<DWORD*>(rl + 20);
        *reinterpret_cast<DWORD*>(v27) = v10;
        *reinterpret_cast<DWORD*>(v27 + 12) = 8;
        *reinterpret_cast<DWORD*>(rl + 20) += 16;
        auto v28 = *(void(__cdecl**)(int, int))(((rl + 16) ^ *reinterpret_cast<DWORD*>(rl + 16)) + 0x85C);
        if (v28)
            v28(rl, v10);
        return v10;
    }
}