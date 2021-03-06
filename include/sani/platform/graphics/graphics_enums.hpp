#pragma once

#include "sani/types.hpp"
#include <functional>

#include "sani/platform/platform_config.hpp"

#if SANI_TARGET_PLATFORM == SANI_PLATFORM_WINDOWS && SANI_DESKTOP
#include "GL/glew.h"
#endif

/*
	TODO: when implementing DirectX begins, 
		  create two headers that contain 
		  mappings for OpenGL and DirectX.
	      
		  The ones defined here are for OpenGL.

		  All GL functionalities are not supported
		  yet and many of the will never be implemented.
*/

namespace sani {

	namespace graphics {

		enum class BufferUsage : uint32 {
			// Buffers data will be modified once and used many times.
			Static	= GL_STATIC_DRAW,

			// Buffers data will be modified repeatedly and used many times.
			Dynamic = GL_DYNAMIC_DRAW
		};

        enum class BufferType : uint32 {
			ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
			ArrayBuffer		   = GL_ARRAY_BUFFER
		};

		enum class PrimitiveType {
			Byte    = GL_BYTE,
			UByte   = GL_UNSIGNED_BYTE,
			Short   = GL_SHORT,
			UShort  = GL_UNSIGNED_SHORT,
			Int     = GL_INT,
			UInt    = GL_UNSIGNED_INT,
			Float   = GL_FLOAT
		};

		enum class ShaderType {
			Vertex   = GL_VERTEX_SHADER,
			Fragment = GL_FRAGMENT_SHADER
		};

		enum class UniformType {
			// Data type expected: float 4 * 4.
			Mat4F		= GL_FLOAT_MAT4,

			// Data type excepted: float 3 * 3.
			Mat3F		= GL_FLOAT_MAT3,

			Float32		= GL_FLOAT
		};

		enum class RenderMode {
			Points					= GL_LINES,
			LineStrip				= GL_LINE_STRIP,
			LineLoop				= GL_LINE_LOOP,
			Lines					= GL_LINES,
			LineStripAdjacency		= GL_LINE_STRIP_ADJACENCY,
			LinesAdjacency			= GL_LINES_ADJACENCY,
			TriangleStrip			= GL_TRIANGLE_STRIP,
			TriangleFan				= GL_TRIANGLE_FAN,
			Triangles				= GL_TRIANGLES,
			TriangleStripAdjacency	= GL_TRIANGLE_STRIP_ADJACENCY,
			TrianglesAdjancency		= GL_TRIANGLES_ADJACENCY,
			Patches					= GL_PATCHES
		};

		enum class SurfaceFormat {
			ColorRGBA = GL_RGBA
		};

		enum class TextureTarget {
			Texture2D = GL_TEXTURE_2D,
			Texture3D = GL_TEXTURE_3D
		};

		enum class TextureParameterName {
			TextureMagFilter = GL_TEXTURE_MAG_FILTER,
			TextureMinFilter = GL_TEXTURE_MIN_FILTER,
			TextureWrapS	 = GL_TEXTURE_WRAP_S,
			TextureWrapT	 = GL_TEXTURE_WRAP_T,
			MaxLevel		 = GL_TEXTURE_MAX_LEVEL,
			BaseLevel		 = GL_TEXTURE_BASE_LEVEL
		};

		enum class TextureWrapMode {
			Repeat		   = GL_REPEAT,
			MirroredRepeat = GL_MIRRORED_REPEAT,
			ClampToEdge    = GL_CLAMP_TO_EDGE,
			ClampToBorder  = GL_CLAMP_TO_BORDER
		};

		enum class TextureMagFilter {
			Nearest = GL_NEAREST,
			Linear  = GL_LINEAR
		};

		enum class TextureMinFilter {
			Nearest				 = GL_NEAREST,
			Linear				 = GL_LINEAR,
			NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
			LinearMipmapNearest  = GL_LINEAR_MIPMAP_NEAREST,
			NearestMipmapLinear  = GL_NEAREST_MIPMAP_LINEAR,
			LinearMipmapLinear   = GL_LINEAR_MIPMAP_LINEAR
		};
	}
}