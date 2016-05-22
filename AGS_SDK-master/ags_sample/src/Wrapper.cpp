//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

//-----------------------------------------------------------------------------
// File: AGSSample.cpp
//-----------------------------------------------------------------------------


#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <amd_ags.h>
#include <DirectXSH.h>
#include <D3D11.h>

using namespace DirectX;
//#include <AMD_Types.h>
//#include "AMD_AOFX.h"
//#include <cmath>

#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds



#pragma warning(push)
#pragma warning(disable : 4201)        // suppress nameless struct/union level 4 warnings
//AMD_DECLARE_BASIC_VECTOR_TYPE;
//#pragma warning(pop)
//AMD_DECLARE_CAMERA_TYPE;





	struct GpuinfoOutput
	{
		int                     deviceId;                       // The device id
		int                     revisionId;                     // The revision id
		int                     iNumCUs;                        // Number of GCN compute units. Zero if not GCN
		int                     iCoreClock;                     // core clock speed at 100% power in MHz
		int                     iMemoryClock;                   // memory clock speed at 100% power in MHz
		float                   fTFlops;                        // Teraflops of GPU. Zero if not GCN. Calculated from iCoreClock * iNumCUs * 64 Pixels/clk * 2 instructions/MAD
		int						ReturnCode;
		int						version;
		AGSContext*				agsContext = nullptr;
	};

	struct DriverExtensions
	{
		unsigned int            Supported;                       // The device id
		int                     ReturnCode;                     // The revision id

	};
	struct AGSOuput
	{
		int            Value;                       // The device id
		int            ReturnCode;                     // The revision id

	};
	/*

	struct AOFXInput
	{


		ID3D11ShaderResourceView*           m_pDepthSRV = nullptr;
		ID3D11ShaderResourceView*           m_pNormalSRV = nullptr;
		ID3D11RenderTargetView*             m_pOutputRTV = nullptr;

		AMD::AOFX_LAYER_PROCESS                  m_LayerProcess;
		AMD::AOFX_BILATERAL_BLUR_RADIUS          m_BilateralBlurRadius;
		AMD::AOFX_SAMPLE_COUNT                   m_SampleCount;
		AMD::AOFX_NORMAL_OPTION                  m_NormalOption;
		AMD::AOFX_TAP_TYPE                       m_TapType;

		float                               m_MultiResLayerScale;
		float                               m_PowIntensity;
		float                               m_RejectRadius;
		float                               m_AcceptRadius;
		float                               m_RecipFadeOutDist;
		float                               m_LinearIntensity;
		float                               m_NormalScale;
		float                               m_ViewDistanceDiscard;
		float                               m_ViewDistanceFade;
		float                               m_DepthUpsampleThreshold;

		int m_Implementation;
	};*/

	
	extern "C"	DllExport int main()
	{

		return 1;
	}


	namespace AMD

	{

#define SAFE_CALL(x)         { if (x != S_OK) assert(0); }

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef clamp
#define clamp(x,a,b) (min(max(x, a), b))
#endif

#ifndef D3DX_PI
#define D3DX_PI 3.1415926535897932384626433832795f
#endif

#ifndef INV_LN2
#define INV_LN2 1.44269504f
#endif

#ifndef SQRT_LN2
#define SQRT_LN2 0.832554611f
#endif

#define MAX_BLUR_RADIUS 16
#define NUM_BLUR_PERMUTATIONS (MAX_BLUR_RADIUS/2 + 1)

#define MAX_HBAO_STEP_SIZE 8
#define NUM_HBAO_PERMUTATIONS (MAX_HBAO_STEP_SIZE)


		//-------------------------------------------------------------------------------------------------
		// Global variables
		//-------------------------------------------------------------------------------------------------

		//AMD::AOFX_Desc           g_aoDesc;

		int PrintInfo(AGSContext* agsContext, AGSGPUInfo info)
		{
			// Enable run-time memory check for debug builds.
			// (When _DEBUG is not defined, calls to _CrtSetDbgFlag are removed during preprocessing.)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);



			int displayIndex = 0;
			int primaryDisplayIndex = 0;
			DISPLAY_DEVICEA displayDevice;
			displayDevice.cb = sizeof(displayDevice);
			while (EnumDisplayDevicesA(0, displayIndex, &displayDevice, 0))
			{
				if (displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
				{
					primaryDisplayIndex = displayIndex;
				}

				//printf( "Display Device: %d %s %s\n", displayIndex, displayDevice.DeviceString, displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE ? "(primary)" : "" );

				displayIndex++;
			}




			printf("%s, device id: 0x%04X, revision id: 0x%02X\n", info.adapterString ? info.adapterString : "unknown GPU", info.deviceId, info.revisionId);
			printf("Driver version:            %s\n", info.driverVersion);
			printf("Radeon Software:           %s\n", info.radeonSoftwareVersion);
			printf("-----------------------------------------------------------------\n");

			printf("Is %sGCN, %d CUs, core clock %d MHz, memory clock %d MHz, %.1f Tflops\n", info.version == AGSGPUInfo::ArchitectureVersion_GCN ? "" : "not ", info.iNumCUs, info.iCoreClock, info.iMemoryClock, info.fTFlops);

			int totalGPUs = 0;
			if (agsGetTotalGPUCount(agsContext, &totalGPUs) == AGS_SUCCESS)
			{
				printf("Total system GPU count = %d\n", totalGPUs);

				for (int i = 0; i < totalGPUs; i++)
				{
					long long memory = 0;
					if (agsGetGPUMemorySize(agsContext, i, &memory) == AGS_SUCCESS)
					{
						printf("GPU %d Memory: %dMBs\n", i, (int)(memory / (1024 * 1024)));
					}
					else
					{
						printf("Failed to get memory size from GPU %d\n", i);
					}
				}

				printf("-----------------------------------------------------------------\n");
			}
			else
			{
				printf("Failed to get total GPU count\n");
			}

			int numCrossfireGPUs = 0;
			if (agsGetCrossfireGPUCount(agsContext, &numCrossfireGPUs) == AGS_SUCCESS)
			{
				printf("Crossfire GPU count = %d\n", numCrossfireGPUs);
				printf("-----------------------------------------------------------------\n");
			}
			else
			{
				printf("Failed to get Crossfire GPU count\n");
			}


			printf("-----------------------------------------------------------------\n");







		//	_getch();

			return 0;
		}



	// Description
	//   Function used to initialize the AGS library.
	//   Must be called prior to any of the subsequent AGS API calls.
	//
	// Input params
	//   context - Address of a pointer to a context. This function allocates a context on the heap which is then required for all subsequent API calls.
	//   info    - Optional pointer to a AGSGPUInfo struct which will get filled in for the primary adapter.
	//
	extern "C" DllExport GpuinfoOutput GetInfo()
	{
		GpuinfoOutput Retinfo = GpuinfoOutput();

		AGSContext* agsContext = nullptr;

		AGSGPUInfo info;

		Retinfo.ReturnCode = agsInit(&agsContext, &info);
	
		if (Retinfo.ReturnCode == AGS_SUCCESS)
		{

			printf("AGS Library initialized\n\n");
			printf("-----------------------------------------------------------------\n");
			Retinfo.deviceId = info.deviceId;
			Retinfo.revisionId = info.revisionId;
			Retinfo.version = info.version;
			Retinfo.iNumCUs = info.iNumCUs;
			Retinfo.iCoreClock = info.iCoreClock;
			Retinfo.iMemoryClock = info.iMemoryClock;
			Retinfo.fTFlops = info.fTFlops;
			Retinfo.agsContext = agsContext;
		}
		else
		{
			printf("Failed to initialize AGS Library\n");
		}
		
		PrintInfo(agsContext, info);
		//_getch();		
		return Retinfo;
	}



	
	// Description
	//   Function used to clean up the AGS library.
	//
	// Input params
	//   context - Pointer to a context. This function will deallocate the context from the heap.
	//
	extern "C" DllExport AGSReturnCode DeInt(AGSContext* agsContext)
	{
		return agsDeInit(agsContext);
	}


	// Description
	//   Function used to query the number of GPUs used for Crossfire acceleration.
	//   This may be different from the total number of GPUs present in the system
	//   which you can query using agsGetTotalGPUCount which reports all GPUs,
	//   even if they are not configured for Crossfire.
	//
	// Input params
	//   context - Pointer to a context.
	//
	// Output params
	// 	 numGPUs - Number of GPUs used for Crossfire acceleration
	//
	extern "C" DllExport AGSOuput GetCrossfireGPUCount(AGSContext* context)
	{
		int totalGPUs = 0;
		AGSReturnCode ret = agsGetCrossfireGPUCount(context, &totalGPUs);
		AGSOuput out = AGSOuput();
		out.ReturnCode = ret;
		out.Value = totalGPUs;
		return out;
	}

	// Description
	//   Function used to query the number of GPUs in the system.
	//   This number may be different from agsGetCrossfireGPUCount as it reports
	//   all devices installed in the system, and not only those configured for
	//   Crossfire.
	//
	// Input params
	//   context - Pointer to a context.
	//
	// Output params
	//   numGPUs - Number of GPUs in the system.
	//
	extern "C" DllExport AGSOuput GetTotalGPUCount(AGSContext* context)
	{
		int totalGPUs = 0;
		AGSReturnCode ret = agsGetTotalGPUCount(context, &totalGPUs);
		AGSOuput out = AGSOuput();
		out.ReturnCode = ret;
		out.Value = totalGPUs;
		return out;
	}

	// Description
	//   Function used to query the memory size of a GPU. The number of GPUs should
	//   be obtained using agsGetTotalGPUCount
	//
	// Input params
	//   context - Pointer to a context.
	//   gpuIndex - The GPU index to query
	//
	// Output params
	//   sizeInBytes - Memory size on the device in bytes
	//
	extern "C" DllExport AGSOuput GetGPUMemorySize(AGSContext* context, int gpuIndex)
	{
		long long sizeout = 0;
		AGSReturnCode ret = agsGetGPUMemorySize(context, gpuIndex, &sizeout);
		AGSOuput out = AGSOuput();
		out.ReturnCode = ret;
		out.Value = (int)(sizeout / 1024) / 1024;
		return out;
	}


	// Description
	//   Function used to initialize the AMD-specific driver extensions for D3D11
	//
	// Input params
	//   context -             Pointer to a context. This is generated by agsInit()
	//   device -              The D3D11 device.
	//   extensionsSupported - Pointer to a bit mask that this function will fill in to indicate which extensions are supported.
	//
	extern "C" DllExport DriverExtensions DriverExtensions_Init(AGSContext* agsContext, ID3D11Device* device)
	{
		unsigned int   g_ExtensionsSupported = 0;
		DriverExtensions retSupport = DriverExtensions();
		int ret = agsDriverExtensions_Init(agsContext, device, &g_ExtensionsSupported);
		retSupport.ReturnCode = ret;
		retSupport.Supported = g_ExtensionsSupported;
		return retSupport;
	}


	// Description
	//   Function used to cleanup any AMD-specific driver extensions for D3D11
	//
	// Input params
	//   context -             Pointer to a context.
	//
	extern "C" DllExport AGSReturnCode DriverExtensions_DeInit(AGSContext* agsContext)
	{

		return agsDriverExtensions_DeInit(agsContext);
	}


	// Description
	//   Function used to set the primitive topology. If you are using any of the extended topology types, then this function should
	//   be called to set ALL topology types.
	//
	// Input params
	//   context -             Pointer to a context.
	//   topology -            The topology to set on the D3D11 device. This can be either an AGS-defined topology such as AGS_PRIMITIVE_TOPOLOGY_QUAD_LIST
	//                         or a standard D3D-defined topology such as D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP.
	//                         NB. the AGS-defined types will require casting to a D3D_PRIMITIVE_TOPOLOGY type.
	//
	extern "C"	DllExport AGSReturnCode DriverExtensions_IASetPrimitiveTopology(AGSContext* context, enum D3D_PRIMITIVE_TOPOLOGY topology)
	{

		return agsDriverExtensions_IASetPrimitiveTopology(context, topology);
	}

	// Description
	//   Function used indicate to the driver it can overlap the subsequent batch of back-to-back dispatches
	//
	// Input params
	//   context -             Pointer to a context.
	//
	extern "C" DllExport AGSReturnCode DriverExtensions_BeginUAVOverlap(AGSContext* context)
	{

		return agsDriverExtensions_BeginUAVOverlap(context);
	}

	// Description
	//   Function used indicate to the driver it can no longer overlap the batch of back-to-back dispatches that has been submitted
	//
	// Input params
	//   context -             Pointer to a context.
	//
	extern "C"	DllExport AGSReturnCode DriverExtensions_EndUAVOverlap(AGSContext* context)
	{

		return agsDriverExtensions_EndUAVOverlap(context);
	}

	// Description
	//   Function used to set the depth bounds test extension
	//
	// Input params
	//   context -  Pointer to a context.
	//   enabled -  Whether to enable or disable the depth bounds testing. If disabled, the next two args are ignored.
	//   minDepth - The near depth range to clip against.
	//   maxDepth - The far depth range to clip against.
	//
	extern "C"	DllExport AGSReturnCode DriverExtensions_SetDepthBounds(AGSContext* context, bool enabled, float minDepth, float maxDepth)
	{

		return agsDriverExtensions_SetDepthBounds(context, enabled, minDepth, maxDepth);
	}

	// Description
	//   Function used to submit a batch of draws via MultiDrawIndirect
	//
	// Input params
	//   context -             Pointer to a context.
	//
	extern "C" DllExport AGSReturnCode DriverExtensions_MultiDrawInstancedIndirect(AGSContext* context, unsigned int drawCount, ID3D11Buffer* pBufferForArgs, unsigned int alignedByteOffsetForArgs, unsigned int byteStrideForArgs)
	{
		return agsDriverExtensions_MultiDrawInstancedIndirect(context, drawCount, pBufferForArgs, alignedByteOffsetForArgs, byteStrideForArgs);
	}

	// Description
	//   Function used to submit a batch of draws via MultiDrawIndirect
	//
	// Input params
	//   context -             Pointer to a context.
	//
	extern "C"	DllExport AGSReturnCode DriverExtensions_MultiDrawIndexedInstancedIndirect(AGSContext* context, unsigned int drawCount, ID3D11Buffer* pBufferForArgs, unsigned int alignedByteOffsetForArgs, unsigned int byteStrideForArgs)
	{

		return agsDriverExtensions_MultiDrawIndexedInstancedIndirect(context, drawCount, pBufferForArgs, alignedByteOffsetForArgs, byteStrideForArgs);
	}

	// Description
	//   Function to register the app with the DX11 driver
	//
	// Input params
	//   context -             Pointer to a context.
	//   engineName -          Optional engine name.
	//   engineVersion -       Engine version.
	//   appName -             Optional app name.
	//   appVersion -          App version.
	//
	// Remarks
	//   Use AGS_MAKE_VERSION for specific versioning, otherwise use AGS_UNSPECIFIED_VERSION.
	//   While both engineName and appName are optional, at least one must be specified.
	//
	extern "C" DllExport AGSReturnCode DriverExtensions_RegisterApp(AGSContext* context, const char* engineName, unsigned int engineVersion, const char* appName, unsigned int appVersion)
	{

		return agsDriverExtensions_RegisterApp(context, engineName, engineVersion, appName, appVersion);
	}
	//-------------------------------------------------------------------------------------
	inline void InitResultData(_Out_writes_(6 * 6 + 1) float *shResult, _In_reads_(order*order) const float *v)
	{
		for (size_t i = 0; i < XM_SH_MAXORDER*XM_SH_MAXORDER + 1; ++i)
		{
			shResult[i] = v[i];
		}
	}
	
	
	void dump_coeffs(_In_ FILE* f, _In_ size_t order, _In_reads_(order*order) const float *v, _In_z_ const char* varname)
	{
		fprintf(f, "static float g_%s[%Iu*%Iu] = {\n    ", varname, order, order);

		for (size_t i = 0; i < order*order; ++i)
		{
			fprintf(f, "%lff, ", v[i]);
		}

		fprintf(f, "};\n");

	}
	
	//-------------------------------------------------------------------------------------
	// Projects a function represented in a cube map into spherical harmonics.
	//
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ff476300.aspx
	//-------------------------------------------------------------------------------------
	extern "C" DllExport int SHProject(ID3D11DeviceContext *context,  ID3D11Texture2D *cubeMap , size_t order,
		OUT float** resultR, OUT  float** resultG,OUT float** resultB)
	{
		int Out=0;
		
		HRESULT hr;
	
		*resultR = new float[order*order + 1];
		*resultG = new float[order*order + 1];
		*resultB = new float[order*order + 1];
		
		hr = SHProjectCubeMap(context, order, cubeMap, *resultR, *resultG, *resultB);
	
	/*	 FILE* fp = nullptr;
		 if (_wfopen_s(&fp, L"coeff.cpp", L"w") == 0)
		 {			
			 dump_coeffs(fp, order, shResultA, "shxyfuncR");
			 dump_coeffs(fp, order, shResultB, "shxyfuncG");
			 dump_coeffs(fp, order, shResultC, "shxyfuncB");
		 }		*/

		if (FAILED(hr))
		{
			printf("SHProjectCubemap failed\n");
			Out = -1;
		}
		if (SUCCEEDED(hr))
		{
			printf("SHProjectCubemap Finished\n");
			Out = 1;
		
		}
	
		return Out;
	}

	
	extern "C" DllExport void FreeArrayFloat(float* outputs)
	{
		delete[] outputs;
	}
	extern "C" DllExport void FreeArrayInt(int* outputs)
	{
		delete[] outputs;
	}


	//--------------------------------------------------------------------------------
	extern "C" DllExport int CreateRandomTexture(ID3D11Device *pD3DDevice, size_t RANDOM_TEXTURE_WIDTH, size_t NUM_DIRECTIONS,OUT ID3D11Texture2D** m_RandomTexture)
	{
		// Mersenne-Twister random numbers in [0,1).
		static const float MersenneTwisterNumbers[1024] = {
			0.463937f,0.340042f,0.223035f,0.468465f,0.322224f,0.979269f,0.031798f,0.973392f,0.778313f,0.456168f,0.258593f,0.330083f,0.387332f,0.380117f,0.179842f,0.910755f,
			0.511623f,0.092933f,0.180794f,0.620153f,0.101348f,0.556342f,0.642479f,0.442008f,0.215115f,0.475218f,0.157357f,0.568868f,0.501241f,0.629229f,0.699218f,0.707733f,
			0.556725f,0.005520f,0.708315f,0.583199f,0.236644f,0.992380f,0.981091f,0.119804f,0.510866f,0.560499f,0.961497f,0.557862f,0.539955f,0.332871f,0.417807f,0.920779f,
			0.730747f,0.076690f,0.008562f,0.660104f,0.428921f,0.511342f,0.587871f,0.906406f,0.437980f,0.620309f,0.062196f,0.119485f,0.235646f,0.795892f,0.044437f,0.617311f,
			0.891128f,0.263161f,0.245298f,0.276518f,0.786986f,0.059768f,0.424345f,0.433341f,0.052190f,0.699924f,0.139479f,0.402873f,0.741976f,0.557978f,0.127093f,0.946352f,
			0.205587f,0.092822f,0.422956f,0.715176f,0.711952f,0.926062f,0.368646f,0.286516f,0.241413f,0.831616f,0.232247f,0.478637f,0.366948f,0.432024f,0.268430f,0.619122f,
			0.391737f,0.056698f,0.067702f,0.509009f,0.920858f,0.298358f,0.701015f,0.044309f,0.936794f,0.485976f,0.271286f,0.108779f,0.325844f,0.682314f,0.955090f,0.658145f,
			0.295861f,0.562559f,0.867194f,0.810552f,0.487959f,0.869567f,0.224706f,0.962637f,0.646548f,0.003730f,0.228857f,0.263667f,0.365176f,0.958302f,0.606619f,0.901869f,
			0.757257f,0.306061f,0.633172f,0.407697f,0.443632f,0.979959f,0.922944f,0.946421f,0.594079f,0.604343f,0.864211f,0.187557f,0.877119f,0.792025f,0.954840f,0.976719f,
			0.350546f,0.834781f,0.945113f,0.155877f,0.411841f,0.552378f,0.855409f,0.741383f,0.761251f,0.896223f,0.782077f,0.266224f,0.128873f,0.645733f,0.591567f,0.247385f,
			0.260848f,0.811970f,0.653369f,0.976713f,0.221533f,0.957436f,0.294018f,0.159025f,0.820596f,0.569601f,0.934328f,0.467182f,0.763165f,0.835736f,0.240033f,0.389869f,
			0.998754f,0.783739f,0.758034f,0.614317f,0.221128f,0.502497f,0.978066f,0.247794f,0.619551f,0.658307f,0.769667f,0.768478f,0.337143f,0.370689f,0.084723f,0.510534f,
			0.594996f,0.994636f,0.181230f,0.868113f,0.312023f,0.480495f,0.177356f,0.367374f,0.741642f,0.202983f,0.229404f,0.108165f,0.098607f,0.010412f,0.727391f,0.942217f,
			0.023850f,0.110631f,0.958293f,0.208996f,0.584609f,0.491803f,0.238266f,0.591587f,0.297477f,0.681421f,0.215040f,0.587764f,0.704494f,0.978978f,0.911686f,0.692657f,
			0.462987f,0.273259f,0.802855f,0.651633f,0.736728f,0.986217f,0.402363f,0.524098f,0.740470f,0.799076f,0.918257f,0.705367f,0.477477f,0.102279f,0.809959f,0.860645f,
			0.118276f,0.009567f,0.280106f,0.948473f,0.025423f,0.458173f,0.512607f,0.082088f,0.536906f,0.472590f,0.835726f,0.078518f,0.357919f,0.797522f,0.570516f,0.162719f,
			0.815968f,0.874141f,0.915300f,0.392073f,0.366307f,0.766238f,0.462755f,0.087614f,0.402357f,0.277686f,0.294194f,0.392791f,0.504893f,0.263420f,0.509197f,0.518974f,
			0.738809f,0.965800f,0.003864f,0.976899f,0.292287f,0.837148f,0.525498f,0.743779f,0.359015f,0.060636f,0.595481f,0.483102f,0.900195f,0.423277f,0.981990f,0.154968f,
			0.085584f,0.681517f,0.814437f,0.105936f,0.972238f,0.207062f,0.994642f,0.989271f,0.646217f,0.330263f,0.432094f,0.139929f,0.908629f,0.271571f,0.539319f,0.845182f,
			0.140069f,0.001406f,0.340195f,0.582218f,0.693570f,0.293148f,0.733441f,0.375523f,0.676068f,0.130642f,0.606523f,0.441091f,0.113519f,0.844462f,0.399921f,0.551049f,
			0.482781f,0.894854f,0.188909f,0.431045f,0.043693f,0.394601f,0.544309f,0.798761f,0.040417f,0.022292f,0.681257f,0.598379f,0.069981f,0.255632f,0.174776f,0.880842f,
			0.412071f,0.397976f,0.932835f,0.979471f,0.244276f,0.488083f,0.313785f,0.858199f,0.390958f,0.426132f,0.754800f,0.360781f,0.862827f,0.526424f,0.090054f,0.673971f,
			0.715044f,0.237489f,0.210234f,0.952837f,0.448429f,0.738062f,0.077342f,0.260666f,0.590478f,0.127519f,0.628981f,0.136232f,0.860189f,0.596789f,0.524043f,0.897171f,
			0.648864f,0.116735f,0.666835f,0.536993f,0.811733f,0.854961f,0.857206f,0.945069f,0.434195f,0.602343f,0.823780f,0.109481f,0.684652f,0.195598f,0.213630f,0.283516f,
			0.387092f,0.182029f,0.834655f,0.948975f,0.373107f,0.249751f,0.162575f,0.587850f,0.192648f,0.737863f,0.777432f,0.651490f,0.562558f,0.918301f,0.094830f,0.260698f,
			0.629400f,0.751325f,0.362210f,0.649610f,0.397390f,0.670624f,0.215662f,0.925465f,0.908397f,0.486853f,0.141060f,0.236122f,0.926399f,0.416056f,0.781483f,0.538538f,
			0.119521f,0.004196f,0.847561f,0.876772f,0.945552f,0.935095f,0.422025f,0.502860f,0.932500f,0.116670f,0.700854f,0.995577f,0.334925f,0.174659f,0.982878f,0.174110f,
			0.734294f,0.769366f,0.917586f,0.382623f,0.795816f,0.051831f,0.528121f,0.691978f,0.337981f,0.675601f,0.969444f,0.354908f,0.054569f,0.254278f,0.978879f,0.611259f,
			0.890006f,0.712659f,0.219624f,0.826455f,0.351117f,0.087383f,0.862534f,0.805461f,0.499343f,0.482118f,0.036473f,0.815656f,0.016539f,0.875982f,0.308313f,0.650039f,
			0.494165f,0.615983f,0.396761f,0.921652f,0.164612f,0.472705f,0.559820f,0.675677f,0.059891f,0.295793f,0.818010f,0.769365f,0.158699f,0.648142f,0.228793f,0.627454f,
			0.138543f,0.639463f,0.200399f,0.352380f,0.470716f,0.888694f,0.311777f,0.571183f,0.979317f,0.457287f,0.115151f,0.725631f,0.620539f,0.629373f,0.850207f,0.949974f,
			0.254675f,0.142306f,0.688887f,0.307235f,0.284882f,0.847675f,0.617070f,0.207422f,0.550545f,0.541886f,0.173878f,0.474841f,0.678372f,0.289180f,0.528111f,0.306538f,
			0.869399f,0.040299f,0.417301f,0.472569f,0.857612f,0.917462f,0.842319f,0.986865f,0.604528f,0.731115f,0.607880f,0.904675f,0.397955f,0.627867f,0.533371f,0.656758f,
			0.627210f,0.223554f,0.268442f,0.254858f,0.834380f,0.131010f,0.838028f,0.613512f,0.821627f,0.859779f,0.405212f,0.909901f,0.036186f,0.643093f,0.187064f,0.945730f,
			0.319022f,0.709012f,0.852200f,0.559587f,0.865751f,0.368890f,0.840416f,0.950571f,0.315120f,0.331749f,0.509218f,0.468617f,0.119006f,0.541820f,0.983444f,0.115515f,
			0.299804f,0.840386f,0.445282f,0.900755f,0.633600f,0.304196f,0.996153f,0.844025f,0.462361f,0.314402f,0.850035f,0.773624f,0.958303f,0.765382f,0.567577f,0.722607f,
			0.001299f,0.189690f,0.364661f,0.192390f,0.836882f,0.783680f,0.026723f,0.065230f,0.588791f,0.937752f,0.993644f,0.597499f,0.851975f,0.670339f,0.360987f,0.755649f,
			0.571521f,0.231990f,0.425067f,0.116442f,0.321815f,0.629616f,0.701207f,0.716931f,0.146357f,0.360526f,0.498487f,0.846096f,0.307994f,0.323456f,0.288884f,0.477935f,
			0.236433f,0.876589f,0.667459f,0.977175f,0.179347f,0.479408f,0.633292f,0.957666f,0.343651f,0.871846f,0.452856f,0.895494f,0.327657f,0.867779f,0.596825f,0.907009f,
			0.417409f,0.530739f,0.547422f,0.141032f,0.721096f,0.587663f,0.830054f,0.460860f,0.563898f,0.673780f,0.035824f,0.755808f,0.331846f,0.653460f,0.926339f,0.724599f,
			0.978501f,0.495221f,0.098108f,0.936766f,0.139911f,0.851336f,0.889867f,0.376509f,0.661482f,0.156487f,0.671886f,0.487835f,0.046571f,0.441975f,0.014015f,0.440433f,
			0.235927f,0.163762f,0.075399f,0.254734f,0.214011f,0.554803f,0.712877f,0.795785f,0.471616f,0.105032f,0.355989f,0.834418f,0.498021f,0.018318f,0.364799f,0.918869f,
			0.909222f,0.858506f,0.928250f,0.946347f,0.755364f,0.408753f,0.137841f,0.247870f,0.300618f,0.470068f,0.248714f,0.521691f,0.009862f,0.891550f,0.908914f,0.227533f,
			0.702908f,0.596738f,0.581597f,0.099904f,0.804893f,0.947457f,0.080649f,0.375755f,0.890498f,0.689130f,0.600941f,0.382261f,0.814084f,0.258373f,0.278029f,0.907399f,
			0.625024f,0.016637f,0.502896f,0.743077f,0.247834f,0.846201f,0.647815f,0.379888f,0.517357f,0.921494f,0.904846f,0.805645f,0.671974f,0.487205f,0.678009f,0.575624f,
			0.910779f,0.947642f,0.524788f,0.231298f,0.299029f,0.068158f,0.569690f,0.121049f,0.701641f,0.311914f,0.447310f,0.014019f,0.013391f,0.257855f,0.481835f,0.808870f,
			0.628222f,0.780253f,0.202719f,0.024902f,0.774355f,0.783080f,0.330077f,0.788864f,0.346888f,0.778702f,0.261985f,0.696691f,0.212839f,0.713849f,0.871828f,0.639753f,
			0.711037f,0.651247f,0.042374f,0.236938f,0.746267f,0.235043f,0.442707f,0.195417f,0.175918f,0.987980f,0.031270f,0.975425f,0.277087f,0.752667f,0.639751f,0.507857f,
			0.873571f,0.775393f,0.390003f,0.415997f,0.287861f,0.189340f,0.837939f,0.186253f,0.355633f,0.803788f,0.029124f,0.802046f,0.248046f,0.354010f,0.420571f,0.109523f,
			0.731250f,0.700653f,0.716019f,0.651507f,0.250055f,0.884214f,0.364255f,0.244975f,0.472268f,0.080641f,0.309332f,0.250613f,0.519091f,0.066142f,0.037804f,0.865752f,
			0.767738f,0.617325f,0.537048f,0.743959f,0.401200f,0.595458f,0.869843f,0.193999f,0.670364f,0.018494f,0.743159f,0.979555f,0.382352f,0.191059f,0.992247f,0.946175f,
			0.306473f,0.793720f,0.687331f,0.556239f,0.958367f,0.390949f,0.357823f,0.110213f,0.977540f,0.831431f,0.485895f,0.148678f,0.847327f,0.733145f,0.397393f,0.376365f,
			0.398704f,0.463869f,0.976946f,0.844771f,0.075688f,0.473865f,0.470958f,0.548172f,0.350174f,0.727441f,0.123139f,0.347760f,0.839587f,0.562705f,0.036853f,0.564723f,
			0.960356f,0.220534f,0.906969f,0.677664f,0.841052f,0.111530f,0.032346f,0.027749f,0.468255f,0.229196f,0.508756f,0.199613f,0.298103f,0.677274f,0.526005f,0.828221f,
			0.413321f,0.305165f,0.223361f,0.778072f,0.198089f,0.414976f,0.007498f,0.464238f,0.785213f,0.534428f,0.060537f,0.572427f,0.693334f,0.865843f,0.034964f,0.586806f,
			0.161710f,0.203743f,0.656513f,0.604340f,0.688333f,0.257211f,0.246437f,0.338237f,0.839947f,0.268420f,0.913245f,0.759551f,0.289283f,0.347280f,0.508970f,0.361526f,
			0.554649f,0.086439f,0.024344f,0.661653f,0.988840f,0.110613f,0.129422f,0.405940f,0.781764f,0.303922f,0.521807f,0.236282f,0.277927f,0.699228f,0.733812f,0.772090f,
			0.658423f,0.056394f,0.153089f,0.536837f,0.792251f,0.165229f,0.592251f,0.228337f,0.147078f,0.116056f,0.319268f,0.293400f,0.872600f,0.842240f,0.306238f,0.228790f,
			0.745704f,0.821321f,0.778268f,0.611390f,0.969139f,0.297654f,0.367369f,0.815074f,0.985840f,0.693232f,0.411759f,0.366651f,0.345481f,0.609060f,0.778929f,0.640823f,
			0.340969f,0.328489f,0.898686f,0.952345f,0.272572f,0.758995f,0.111269f,0.613403f,0.864397f,0.607601f,0.357317f,0.227619f,0.177081f,0.773828f,0.318257f,0.298335f,
			0.679382f,0.454625f,0.976745f,0.244511f,0.880111f,0.046238f,0.451342f,0.709265f,0.784123f,0.488338f,0.228713f,0.041251f,0.077453f,0.718891f,0.454221f,0.039182f,
			0.614777f,0.538681f,0.856650f,0.888921f,0.184013f,0.487999f,0.880338f,0.726824f,0.112945f,0.835710f,0.943366f,0.340094f,0.167909f,0.241240f,0.125953f,0.460130f,
			0.789923f,0.313898f,0.640780f,0.795920f,0.198025f,0.407344f,0.673839f,0.414326f,0.185900f,0.353436f,0.786795f,0.422102f,0.133975f,0.363270f,0.393833f,0.748760f,
			0.328130f,0.115681f,0.253865f,0.526924f,0.672761f,0.517447f,0.686442f,0.532847f,0.551176f,0.667406f,0.382640f,0.408796f,0.649460f,0.613948f,0.600470f,0.485404f,
		};
		int Out = 0;

		HRESULT hr;
		int randomNumberIndex = 0;
		INT16 *data = new INT16[RANDOM_TEXTURE_WIDTH*RANDOM_TEXTURE_WIDTH * 4];
		for (UINT i = 0; i < RANDOM_TEXTURE_WIDTH*RANDOM_TEXTURE_WIDTH * 4; i += 4)
		{
			//assert(randomNumberIndex < 1024);
			if (randomNumberIndex > 1024)
			{				
				randomNumberIndex = 0;
			}
			
			float r1 = MersenneTwisterNumbers[randomNumberIndex++];
			float r2 = MersenneTwisterNumbers[randomNumberIndex++];

			// Use random rotatation angles in [0,2P/NUM_HBAO_DIRECTIONS).
			// This looks the same as sampling [0,2PI), but is faster.
			float angle = 2.0f * D3DX_PI * r1 / NUM_DIRECTIONS;
			data[i] = (INT16)((1 << 15)*cos(angle));
			data[i + 1] = (INT16)((1 << 15)*sin(angle));
			data[i + 2] = (INT16)((1 << 15)*r2);
			data[i + 3] = 0;
		}

		D3D11_TEXTURE2D_DESC desc ;
		desc.Width = RANDOM_TEXTURE_WIDTH;
		desc.Height = RANDOM_TEXTURE_WIDTH;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA srd;
		srd.pSysMem = data;
		srd.SysMemPitch = RANDOM_TEXTURE_WIDTH * 4 * sizeof(INT16);
		srd.SysMemSlicePitch = 0;

		SAFE_RELEASE(*m_RandomTexture);
		hr = pD3DDevice->CreateTexture2D(&desc, &srd, m_RandomTexture);	

		delete[] data;	

		if (FAILED(hr))
		{
			printf("Mersenne Twister Numbers Texture Build failed\n");
			Out = -1;
		}
		if (SUCCEEDED(hr))
		{
			printf("Mersenne Twister Numbers Textrue Build Finished\n");
			Out = 1;

		}
		return Out;
	}

	
	// Description
	/**
	Initialize internal data inside AOFX_Desc
	Calling this function requires setting up m_pDevice member
	*/
	/*extern "C" DllExport AOFX_RETURN_CODE InitializeAOFX(ID3D11Device* m_pDevice, ID3D11DeviceContext*   m_pDeviceContext)
	{
		
		g_aoDesc.m_pDevice = m_pDevice;
		g_aoDesc.m_pDeviceContext = m_pDeviceContext;
		return AOFX_Initialize(g_aoDesc);
	}


	// Description
	/**
	Release all internal data used by AOFX_OpaqueDesc
	*/
	/*extern "C" DllExport AOFX_RETURN_CODE RenderAOFX(AOFXInput Invalue, uint m_MultiResLayerCount)
	{
		g_aoDesc.m_pDepthSRV = Invalue.m_pDepthSRV;
		g_aoDesc.m_pNormalSRV = Invalue.m_pNormalSRV;
		g_aoDesc.m_pOutputRTV = Invalue.m_pOutputRTV;	
		g_aoDesc.m_OutputChannelsFlag = 8 | 4 | 2 | 1;
		g_aoDesc.m_Implementation = Invalue.m_Implementation;

		for (AMD::uint i = 0; i < m_MultiResLayerCount; i++)
		{
			g_aoDesc.m_LayerProcess[i] = Invalue.m_LayerProcess;
			g_aoDesc.m_MultiResLayerScale[i] = 1.0f / powf(Invalue.m_MultiResLayerScale, (float)i);
			g_aoDesc.m_PowIntensity[i] = Invalue.m_PowIntensity;
			g_aoDesc.m_BilateralBlurRadius[i] = Invalue.m_BilateralBlurRadius;
			g_aoDesc.m_ViewDistanceDiscard[i] = Invalue.m_ViewDistanceDiscard;
			g_aoDesc.m_ViewDistanceFade[i] = Invalue.m_ViewDistanceFade;
			g_aoDesc.m_NormalScale[i] = Invalue.m_NormalScale;
			g_aoDesc.m_LinearIntensity[i] = Invalue.m_LinearIntensity;
			g_aoDesc.m_AcceptRadius[i] = Invalue.m_AcceptRadius;
			g_aoDesc.m_RejectRadius[i] = Invalue.m_RejectRadius;
			g_aoDesc.m_RecipFadeOutDist[i] = Invalue.m_RecipFadeOutDist;
			g_aoDesc.m_DepthUpsampleThreshold[i] = Invalue.m_DepthUpsampleThreshold;

			g_aoDesc.m_SampleCount[i] = Invalue.m_SampleCount;
			g_aoDesc.m_NormalOption[i] = Invalue.m_NormalOption;
			g_aoDesc.m_TapType[i] = Invalue.m_TapType;
		}

		return AOFX_Render(g_aoDesc);
	}
	

	// Description
	/**
	Resize internal texture resources inside AOFX_OpaqueDesc
	Calling this function requires setting up:
	* m_pDevice
	It will also take into account the state of the following members:
	* Active layers process (as defined by m_LayerProcess[])
	* m_MultiResLayerScale
	* m_InputSize
	* m_NormalOption
	*/
	/*extern "C" DllExport AOFX_RETURN_CODE ReSizeAOFX( int g_uWidth,  int g_uHeight)
	{
	
		g_aoDesc.m_LayerProcess[0] = AOFX_LAYER_PROCESS_DEINTERLEAVE_NONE;
		g_aoDesc.m_LayerProcess[1] = AOFX_LAYER_PROCESS_NONE;
		g_aoDesc.m_LayerProcess[2] = AOFX_LAYER_PROCESS_NONE;

		g_aoDesc.m_InputSize.x = g_uWidth;
		g_aoDesc.m_InputSize.y = g_uHeight;
		g_aoDesc.m_NormalOption[0];
		g_aoDesc.m_MultiResLayerScale[3];	

		return AOFX_Resize(g_aoDesc);
	}
	

	// Description
	/**
	Release all internal data used by AOFX_OpaqueDesc
	*/
	/*extern "C" DllExport AOFX_RETURN_CODE ReleaseAOFX()
	{
		return AOFX_Release(g_aoDesc);
	}*/

	
};