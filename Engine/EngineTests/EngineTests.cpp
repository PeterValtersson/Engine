#include "pch.h"
#include "CppUnitTest.h"
#include <ECSEngine.h>
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
			ECSEngine::Init_Info ii;
			ii.sub_systems.resource_archive = ResourceHandler::IResourceArchive::create_binary_archive( "data.dat", ResourceHandler::AccessMode::read_write );
			auto e = ECSEngine::Engine::create( ii );

		}
		catch ( Utilities::Exception & e )
		{
			Assert::IsTrue( false, Utilities::String::utf8_2_utf16( e.what() ).c_str() );
		}
	}

	TEST_METHOD( ExitTest )
	{
		try
		{
			ECSEngine::Init_Info ii;
			ii.sub_systems.resource_archive = ResourceHandler::IResourceArchive::create_binary_archive( "data.dat", ResourceHandler::AccessMode::read_write );
			auto e = ECSEngine::Engine::create( ii );
			e->start();
		}
		catch ( Utilities::Exception & e )
		{
			Assert::IsTrue( false, Utilities::String::utf8_2_utf16( e.what() ).c_str() );
		}
	}

	TEST_METHOD( TriangleTest )
	{
		static const char* vs = R"(

		struct VertexInputType
		{
			float3 position : POSITION;
			float3 color : COLOR;
		};

		struct PixelInputType
		{
			float4 position : SV_POSITION;
			float4 color : COLOR;
		};	

		PixelInputType ColorVertexShader(VertexInputType input)
		{
			PixelInputType output;
    

			// Change the position vector to be 4 units for proper matrix calculations.
			output.position.w = 1.0f;

			output.position.xyz = input.position;

			// Store the input color for the pixel shader to use.
			output.color.rgb = input.color;
			output.color.a = 1.0f;
			return output;
		}


		)";
		static const char* ps = R"(

		struct PixelInputType
		{
			float4 position : SV_POSITION;
			float4 color : COLOR;
		};


		////////////////////////////////////////////////////////////////////////////////
		// Pixel Shader
		////////////////////////////////////////////////////////////////////////////////
		float4 ColorPixelShader(PixelInputType input) : SV_TARGET
		{
			return input.color;
		}


		)";

		try
		{
			ECSEngine::Init_Info ii;
			ii.sub_systems.resource_archive = ResourceHandler::IResourceArchive::create_binary_archive( "data.dat", ResourceHandler::AccessMode::read_write );
			auto e = ECSEngine::Engine::create( ii );
			auto r = e->get_sub_systems().renderer;

			struct Vertex{
				float x, y, z;
				float r, g, b;
			};
			Vertex triangleData[] = {
				{-0.5f, -0.5f, 0, 1.0f, 0.0f, 0.0f},
				{0, 0.5f, 0, 1.0f, 0.0f, 0.0f},
				{0.5f, -0.5f, 0, 1.0f, 0.0f, 0.0f}
			};
			r->UsePipelineHandler( [&]( Renderer::PipelineHandler_Interface& ph )
			{
				ph.CreateBuffer( "Triangle", Renderer::Pipeline::Buffer::VertexBuffer( triangleData, sizeof( Vertex ), 3 ) );
				ph.CreateShader( "VertexShader", Renderer::Pipeline::ShaderType::VERTEX, vs, strlen( vs ), "ColorVertexShader", "vs_5_0" );
				ph.CreateShader( "PixelShader", Renderer::Pipeline::ShaderType::PIXEL, ps, strlen( ps ), "ColorPixelShader", "ps_5_0" );
				Renderer::Pipeline::RasterizerState rs;
				rs.cullMode = Renderer::Pipeline::CullMode::CULL_BACK;
				rs.fillMode = Renderer::Pipeline::FillMode::FILL_SOLID;
				rs.windingOrder = Renderer::Pipeline::WindingOrder::CLOCKWISE;
				ph.CreateRasterizerState( "Rasterizer", rs );
			} );

			Renderer::RenderJob job;
			job.pipeline.Edit( []( Renderer::Pipeline::Pipeline_Mutable pl )
			{
				pl.IAStage.vertexBuffer = "Triangle";
				pl.IAStage.topology = Renderer::Pipeline::PrimitiveTopology::TRIANGLE_LIST;
				pl.VSStage.shader = "VertexShader";
				pl.RStage.viewport = Renderer::Default_Viewport;
				pl.RStage.rasterizerState = "Rasterizer";
				pl.PSStage.shader = "PixelShader";
				pl.OMStage.clearTargets = true;
				pl.OMStage.renderTargetCount = 1;
				pl.OMStage.renderTargets[0] = Renderer::Default_RenderTarget;
			} );
			job.vertexCount = 3;

			r->AddRenderJob( "HelloTriangle", job, Renderer::RenderGroup::RENDER_PASS_0 );


			e->start();
		}
		catch ( Utilities::Exception & e )
		{
			Assert::IsTrue( false, Utilities::String::utf8_2_utf16( e.what() ).c_str() );
		}
	}
	};
}
