#include "lua.hpp"
#include <stdio.h>

//#define CPP_SIMPLE_SAMPLE
//#define COROUTINE_SAMPLE
//#define TABLE_SAMPLE
#define USERDATA_SAMPLE

namespace Util
{
	void printStack(lua_State*);
}

int main()
{
#ifdef CPP_SIMPLE_SAMPLE
	{
		const char* lua_file_name = "lua_sample_1.lua";

		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		luaL_dofile(L, lua_file_name);
		{
			const int input_num = 2;
			const int return_num = 4;
			lua_Number input[input_num] = { 100.0, 80.0 };
			lua_Number output[return_num] = { 0, 0, 0, 0 };

			// stackに積む
			lua_getglobal(L, "func");
			for (int i = 0; i < input_num; ++i)
			{
				lua_pushnumber(L, input[i]);
			}

			// function実行前のstackを表示
			Util::printStack(L);

			// 実行
			lua_call(L, input_num, return_num);

			// function実行後のstackを表示
			Util::printStack(L);

			// stackから値の取得
			for (int i = 0; i < return_num; ++i)
			{
				output[i] = lua_tonumber(L, -return_num + i);
			}

			// 返り値はもう必要ないので、stackをすっきりさせる。
			lua_pop(L, return_num);

			// pop後のstackの表示
			Util::printStack(L);

			// 表示
			for (int i = 0; i < return_num; ++i)
			{
				printf("%f ",output[i]);
			}
			printf("\n");
		}
		lua_close(L);
	}
#endif

#ifdef COROUTINE_SAMPLE
	{
	const char* lua_file_name = "lua_sample_2.lua";

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, lua_file_name);
	{
		const int call_num = 5;
		const int return_num = 1;
		lua_Number output[return_num] = { 0 };

		lua_State* co = lua_newthread(L);
		lua_getglobal(co, "func_co");

		for (int i = 0; i < call_num; ++i)
		{
			// thread実行
			lua_resume(co, L, 0);

			for (int j = 0; j < return_num; ++j)
			{
				output[j] = lua_tonumber(co, -return_num+j);
			}

			for (int j = 0; j < return_num; ++j)
			{
				printf("%f ", output[j]);
			}
			printf("\n");
		}

	}
	lua_close(L);
}
#endif

#ifdef TABLE_SAMPLE
	{
		const char* lua_file_name = "lua_sample_3.lua";

		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		luaL_dofile(L, lua_file_name);
		{
			lua_getglobal(L, "tmp"); // tmpをスタックに積む
			lua_getfield(L, -1, "GetName"); // -1のスタック位置にあるテーブルから取得
			lua_getglobal(L, "tmp"); // thisをとるので、tmpをスタックに積む

			// tmp:GetName(tmp)を実行し、スタックをきれいにした後に戻り値をスタックに積む
			lua_call(L, 1, 1);

			printf("%s\n",lua_tostring(L, -1));
		}
		lua_close(L);
	}
#endif

#ifdef USERDATA_SAMPLE
	{
		const char* lua_file_name = "lua_sample_4.lua";

		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		luaL_dofile(L, lua_file_name);
		{

		}
		lua_close(L);
	}
#endif
}


namespace Util
{
	// スタックを見る関数
	// http://marupeke296.com/LUA_No2_Begin.html
	// から拝借
	void printStack(lua_State *L) {
		// スタック数を取得
		const int num = lua_gettop(L);
		if (num == 0) {
			printf("No stack.\n");
			return;
		}
		for (int i = num; i >= 1; i--) {
			printf("%03d(%04d): ", i, -num + i - 1);
			int type = lua_type(L, i);
			switch (type) {
			case LUA_TNIL:
				printf("NIL\n");
				break;
			case LUA_TBOOLEAN:
				printf("BOOLEAN %s\n", lua_toboolean(L, i) ? "true" : "false");
				break;
			case LUA_TLIGHTUSERDATA:
				printf("LIGHTUSERDATA\n");
				break;
			case LUA_TNUMBER:
				printf("NUMBER %f\n", lua_tonumber(L, i));
				break;
			case LUA_TSTRING:
				printf("STRING %s\n", lua_tostring(L, i));
				break;
			case LUA_TTABLE:
				printf("TABLE\n");
				break;
			case LUA_TFUNCTION:
				printf("FUNCTION\n");
				break;
			case LUA_TUSERDATA:
				printf("USERDATA\n");
				break;
			case LUA_TTHREAD:
				printf("THREAD\n");
				break;
			}
		}
		printf("-----------------------------\n\n");
	}
}
