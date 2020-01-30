#include "pch.h"
#include "CppUnitTest.h"
#include <IEngine.h>
#include <Utilities/StringUtilities.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EngineTests
{
	TEST_CLASS( EngineTests ) {
public:

	TEST_METHOD( CreateEngine )
	{
		try
		{
			Engine::Init_Info ii;
			ii.sub_systems.resource_archive = Resources::IResourceArchive::create_binary_archive( "data.dat", Resources::AccessMode::read_write );
			auto e = Engine::IEngine::create( ii );
		}
		catch ( Utilities::Exception & e )
		{
			Assert::IsTrue( false, Utilities::String::utf8_2_utf16( e.what() ).c_str() );
		}
	}
	};
}
