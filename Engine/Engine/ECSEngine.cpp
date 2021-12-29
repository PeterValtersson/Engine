#include <ECSEngine.h>
#include "EngineImpl.h"

std::unique_ptr<ECSEngine::Engine> ECSEngine::Engine::create( const Init_Info& init_info )
{
	return std::make_unique<EngineImpl>( init_info );
}
