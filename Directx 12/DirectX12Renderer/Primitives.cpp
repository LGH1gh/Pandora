//#include "Primitives.h"
//
//const UINT VERTEX_BUFFER_SIZE = 16 * 1024;
//
//Primitives::Primitives()
//{
//	setup[0].bufferPos = 0;
//	setup[1].bufferPos = 0;
//	setup[2].bufferPos = 0;
//}
//
//Primitives::~Primitives()
//{
//}
//
//void Primitives::CreateResource(Device device, BlendState opaque, BlendState translucent)
//{
//	const AttributeDesc format0[] =
//	{
//		{ 0, ATTRIBUTE_FORMAT_FLOAT2_32, "Vertex" },
//		{ 0, ATTRIBUTE_FORMAT_FLOAT2_32, "TexCoord"},
//	};
//	const AttributeDesc format1[] =
//	{
//		{ 0, ATTRIBUTE_FORMAT_FLOAT2_32, "Vertex" },
//		{ 0, ATTRIBUTE_FORMAT_FLOAT2_32, "TexCoord"},
//	};
//
//	//setup[0].vertexBuffer = CreateBuffer(device, BufferParams(sizeof(float) * 2, VERTEX_BUFFER_SIZE, HEAP_TYPE_UPLOAD, RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
//
//}
//
//void Primitives::DrawVertices(Context context, DrawSetup& setup, const void* vertices, const UINT vertexCount, const UINT vertexStride, PrimitiveType primitiveType, BlendMode blendMode, const float* color, const ResourceTable resources, const SamplerTable samplers)
//{
//	
//}