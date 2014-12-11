#include "GameRuntimeApi.h"

using namespace G2;

GameRuntime::GameRuntime(std::string const& gameRuntimeLib) :
	loadScene(nullptr),
	postLoadScene(nullptr)
{

}

GameRuntime::~GameRuntime()
{

}
