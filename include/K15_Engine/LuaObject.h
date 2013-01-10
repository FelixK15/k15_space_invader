#ifndef __LUA_OBJECT__
#define __LUA_OBJECT__

#include "StandardIncludes.h"
#include "LuaManager.h"

struct lua_State;

namespace K15_Engine
{
	class LuaObject
	{
	public:
		LuaObject(){LuaManager::subscribeLuaObject(this);m_dead = false;}

		virtual ~LuaObject(){}

		bool isDead(){return m_dead;}

		int setDead(lua_State *state){m_dead = true; return 0;}

		int getReference(){return m_reference;}

		void setReference(int reference){m_reference = reference;}

		int countReferences();

	private:
		bool m_dead;
		int m_reference;
	};
}

#endif //__LUA_OBJECT__