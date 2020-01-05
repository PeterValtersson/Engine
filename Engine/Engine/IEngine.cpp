#include <IEngine.h>
#include "Engine.h"

std::shared_ptr<Engine::IEngine> Engine::IEngine::create( Init_Info init_info )
{
	return std::make_shared<Engine>( init_info );
}
