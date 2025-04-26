#include "LuaScriptManager.h"
#include <filesystem>
#include <UserInterface/Console.h>


TMap<FString, sol::table> FLuaScriptManager::ScriptCacheMap;

FLuaScriptManager::FLuaScriptManager()
{
    LuaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);
    SetLuaDefaultTypes();
}

void FLuaScriptManager::SetLuaDefaultTypes()
{
    LuaState.new_usertype<FVector>("Vector",
        sol::constructors<FVector(), FVector(float, float, float)>(),
        "x", &FVector::X,
        "y", &FVector::Y,
        "z", &FVector::Z,
        sol::meta_function::addition, [](const FVector& a, const FVector& b) { return FVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); },
        sol::meta_function::multiplication, [](const FVector& v, float f) { return v * f; }
    );
}

FLuaScriptManager& FLuaScriptManager::Get()
{
    static FLuaScriptManager Instance;
    return Instance;
}

sol::state& FLuaScriptManager::GetLua()
{
    return LuaState;
}

sol::table FLuaScriptManager::CreateLuaTable(const FString& ScriptName)
{   
    if (!std::filesystem::exists(*ScriptName))
    {
        UE_LOG(LogLevel::Error, TEXT("InValid Lua File name."));
        //assert(false && "Lua File not found");
        return sol::table();
    }
    
    if (!ScriptCacheMap.Contains(ScriptName))
    {
        sol::table LuaTable = LuaState.script_file(*ScriptName);
        ScriptCacheMap.Add(ScriptName, LuaTable);
    }
    
    sol::table& ScriptClass = ScriptCacheMap[ScriptName];

    sol::table NewEnv = LuaState.create_table();
    for (auto& pair : ScriptClass)
    {
        NewEnv[pair.first] = pair.second;
    }

    return NewEnv;
}
