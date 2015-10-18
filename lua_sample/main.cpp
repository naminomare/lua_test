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

			// stack�ɐς�
			lua_getglobal(L, "func");
			for (int i = 0; i < input_num; ++i)
			{
				lua_pushnumber(L, input[i]);
			}

			// function���s�O��stack��\��
			Util::printStack(L);

			// ���s
			lua_call(L, input_num, return_num);

			// function���s���stack��\��
			Util::printStack(L);

			// stack����l�̎擾
			for (int i = 0; i < return_num; ++i)
			{
				output[i] = lua_tonumber(L, -return_num + i);
			}

			// �Ԃ�l�͂����K�v�Ȃ��̂ŁAstack���������肳����B
			lua_pop(L, return_num);

			// pop���stack�̕\��
			Util::printStack(L);

			// �\��
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
			// thread���s
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
			lua_getglobal(L, "tmp"); // tmp���X�^�b�N�ɐς�
			lua_getfield(L, -1, "GetName"); // -1�̃X�^�b�N�ʒu�ɂ���e�[�u������擾
			lua_getglobal(L, "tmp"); // this���Ƃ�̂ŁAtmp���X�^�b�N�ɐς�

			// tmp:GetName(tmp)�����s���A�X�^�b�N�����ꂢ�ɂ�����ɖ߂�l���X�^�b�N�ɐς�
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
	// �X�^�b�N������֐�
	// http://marupeke296.com/LUA_No2_Begin.html
	// ����q��
	void printStack(lua_State *L) {
		// �X�^�b�N�����擾
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
