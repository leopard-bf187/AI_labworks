#pragma once

#include "declaration.h"

namespace krystallic
{
	namespace SIMDMath
	{
#if defined(SIMD_AVX) || defined(SIMD_AVX2)

		struct alignas(16) R256x1F 
		{ 
			union
			{
				struct
				{
					__m256 v1;
				};
				__m256 v[1];
			};
		};
		

		struct alignas(16) R256x2F 
		{ 
			union
			{
				struct
				{
					__m256 v1, v2;
				};
				__m256 v[2];
			};
		};
		

		struct alignas(16) R256x1I 
		{ 
			union
			{
				struct
				{
					__m256i v1;
				};
				__m256i v[1];
			};
		};
		

		struct alignas(16) R256x2I 
		{ 
			union
			{
				struct
				{
					__m256i v1, v2;
				};
				__m256i v[2];
			};
		};
		

		struct alignas(16) R256x1D 
		{ 
			union 
			{ 
				struct 
				{ 
					__m256d v1; 
				}; 
				__m256d v[1]; 
			};
		};
		

		struct alignas(16) R256x2D 
		{ 
			union 
			{ 
				struct 
				{ 
					__m256d v1, v2; 
				};
				__m256d v[2]; 
			};
		};

#endif
	}
}