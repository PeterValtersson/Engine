#include <IEngine.h>
#include "Engine.h"

std::unique_ptr<Engine::IEngine> Engine::IEngine::create( Init_Info init_info )
{
	return std::make_unique<Engine>( init_info );
}
