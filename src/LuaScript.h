#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <string>
#include <vector>
#include <iostream>
#include <lua.hpp>
#include <stdarg.h>

/**
 * Manages a single LuaScript
 **/
class LuaScript {
public:

	enum Status{
		Uninitialized,
		Running,
		Paused,
		Dead
	};

	static LuaScript* create(const std::string& filename);

    ~LuaScript();
    void printError(const std::string& variableName, const std::string& reason);
    std::vector<int> LuaScript::getIntVector(const std::string& name);
    std::vector<std::string> LuaScript::getTableKeys(const std::string& name);
    
	void registercfunc(lua_CFunction f, std::string luaFuncName);

	void lua_voidfunc(char* argdesc, ...);
	double lua_doublefunc(char* argdesc, ...);
	int lua_intfunc(char* argdesc, ...);
	std::string lua_stringfunc(char* argdesc, ...);
	bool lua_boolfunc(char* argdesc, ...);

	inline lua_State* getLuaState()
	{
		return L;
	}

	/**
	 * Clean the stack
	 **/
    inline void clean() {
      int n = lua_gettop(L);
      lua_pop(L, n);
    }

	/**
	 * Get a variable out of the Lua Script
	 *
	 * @param variableName		the name of the variable to be accessed
	 *
	 * @return T	The value of the variable
	 **/
    template<typename T>
    T get(const std::string& variableName) {
      if(!L) {
        printError(variableName, "Script is not loaded");
        return lua_getdefault<T>();
      }
      
      T result;
      if(lua_gettostack(variableName)) { // variable succesfully on top of stack
        result = lua_get<T>(variableName);  
      } else {
        result = lua_getdefault<T>();
      }

     
      clean();
      return result;
    }

	/**
	 * put the variable on top of the stack
	 **/
    bool lua_gettostack(const std::string& variableName) {
      level = 0;
      std::string var = "";
        for(unsigned int i = 0; i < variableName.size(); i++) {
          if(variableName.at(i) == '.') {
            if(level == 0) {
              lua_getglobal(L, var.c_str());
            } else {
              lua_getfield(L, -1, var.c_str());
            }
            
            if(lua_isnil(L, -1)) {
              printError(variableName, var + " is not defined");
              return false;
            } else {
              var = "";
              level++;
            }
          } else {
            var += variableName.at(i);
          }
        }
        if(level == 0) {
          lua_getglobal(L, var.c_str());
        } else {
          lua_getfield(L, -1, var.c_str());
        }
        if(lua_isnil(L, -1)) {
            printError(variableName, var + " is not defined");
            return false;
        }

        return true;       
    }
		
    /**
	 * Generic get
	 **/
    template<typename T>
    T lua_get(const std::string& variableName) {
      return 0;
    }

	/**
	 * Failure case get
	 **/
    template<typename T>
    T lua_getdefault() {
      return 0;
    }

	Status getStatus()
	{
		return status;
	}

	void setStatus(Status status)
	{
		this->status = status;
	}

	std::string getFilename()
	{
		return filename.substr(12, filename.length() - 16);
	}
   
private:
	lua_State* L;
    std::string filename;
    int level;
	Status status;

	LuaScript();
	bool init(const std::string& filename);
};

 // Specializations

template <> 
inline bool LuaScript::lua_get<bool>(const std::string& variableName) {
	return (bool)lua_toboolean(L, -1); 
}

template <> 
inline float LuaScript::lua_get<float>(const std::string& variableName) {
    if(!lua_isnumber(L, -1)) {
      printError(variableName, "Not a number");
    }
    return (float)lua_tonumber(L, -1);
}

template <>
inline int LuaScript::lua_get<int>(const std::string& variableName) {
    if(!lua_isnumber(L, -1)) {
      printError(variableName, "Not a number");
    }
    return (int)lua_tonumber(L, -1);
}

template <>
inline std::string LuaScript::lua_get<std::string>(const std::string& variableName) {
    std::string s = "null";
    if(lua_isstring(L, -1)) {
      s = std::string(lua_tostring(L, -1));
    } else {
      printError(variableName, "Not a string");
    }
    return s;
}

template<>
inline std::string LuaScript::lua_getdefault<std::string>() {
  return "null";
}

#endif