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
			auto e = Engine::IEngine::create( {} );
		}
		catch ( Utilities::Exception & e )
		{
			Assert::IsTrue( false, Utilities::String::utf8_2_utf16( e.what() ).c_str() );
		}
	}
	};
}
