#include <ECSEngine.h>
#include "Engine.h"

std::unique_ptr<ECSEngine::ECSEngine> ECSEngine::ECSEngine::create( const Init_Info& init_info )
{
	return std::make_unique<Engine>( init_info );
}
