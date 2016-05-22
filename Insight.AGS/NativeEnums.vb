
<System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)>
Public Structure AOFXInput

    Public m_pDepthSRV As IntPtr
    Public m_pNormalSRV As IntPtr
    Public m_pOutputRTV As IntPtr

    Public m_LayerProcess As AOFX_LAYER_PROCESS
    Public m_BilateralBlurRadius As AOFX_BILATERAL_BLUR_RADIUS
    Public m_SampleCount As AOFX_SAMPLE_COUNT
    Public m_NormalOption As AOFX_NORMAL_OPTION
    Public m_TapType As AOFX_TAP_TYPE

    Public m_MultiResLayerScale As Single
    Public m_PowIntensity As Single
    Public m_RejectRadius As Single
    Public m_AcceptRadius As Single
    Public m_RecipFadeOutDist As Single
    Public m_LinearIntensity As Single
    Public m_NormalScale As Single
    Public m_ViewDistanceDiscard As Single
    Public m_ViewDistanceFade As Single
    Public m_DepthUpsampleThreshold As Single

    Public m_Implementation As AOFX_IMPLEMENTATION_MASK
End Structure

Public Enum AOFX_SAMPLE_COUNT
    AOFX_SAMPLE_COUNT_LOW = 0
    AOFX_SAMPLE_COUNT_MEDIUM = 1
    AOFX_SAMPLE_COUNT_HIGH = 2
    AOFX_SAMPLE_COUNT_ULTRA = 3

    AOFX_SAMPLE_COUNT_COUNT = 4
End Enum
Public Enum AOFX_BILATERAL_BLUR_RADIUS
    AOFX_BILATERAL_BLUR_RADIUS_2 = 0
    AOFX_BILATERAL_BLUR_RADIUS_4 = 1
    AOFX_BILATERAL_BLUR_RADIUS_8 = 2
    AOFX_BILATERAL_BLUR_RADIUS_16 = 3

    AOFX_BILATERAL_BLUR_RADIUS_NONE = -1
    AOFX_BILATERAL_BLUR_RADIUS_COUNT = 4
End Enum

Public Enum AOFX_NORMAL_OPTION
    AOFX_NORMAL_OPTION_NONE = 0
    AOFX_NORMAL_OPTION_READ_FROM_SRV = 1

    AOFX_NORMAL_OPTION_COUNT = 2
End Enum

Public Enum AOFX_TAP_TYPE
    AOFX_TAP_TYPE_FIXED = 0
    AOFX_TAP_TYPE_RANDOM_CB = 1
    AOFX_TAP_TYPE_RANDOM_SRV = 2

    AOFX_TAP_TYPE_COUNT = 3
End Enum

Public Enum AOFX_IMPLEMENTATION_MASK
    AOFX_IMPLEMENTATION_MASK_KERNEL_CS = 1
    AOFX_IMPLEMENTATION_MASK_KERNEL_PS = 2

    AOFX_IMPLEMENTATION_MASK_UTILITY_CS = 4
    AOFX_IMPLEMENTATION_MASK_UTILITY_PS = 8

    AOFX_IMPLEMENTATION_MASK_BLUR_CS = 16
    AOFX_IMPLEMENTATION_MASK_BLUR_PS = 32

    AOFX_IMPLEMENTATION_COUNT = 2
End Enum

Public Enum AOFX_OUTPUT_CHANNEL
    AOFX_OUTPUT_CHANNEL_R = 1
    AOFX_OUTPUT_CHANNEL_G = 2
    AOFX_OUTPUT_CHANNEL_B = 4
    AOFX_OUTPUT_CHANNEL_A = 8

    AOFX_OUTPUT_CHANNEL_COUNT = 16
End Enum

Public Enum AOFX_LAYER_PROCESS As Long
    AOFX_LAYER_PROCESS_DEINTERLEAVE_NONE = 0
    AOFX_LAYER_PROCESS_DEINTERLEAVE_2 = 1
    AOFX_LAYER_PROCESS_DEINTERLEAVE_4 = 2
    AOFX_LAYER_PROCESS_DEINTERLEAVE_8 = 3

    AOFX_LAYER_PROCESS_NONE = &HFFFFFFFFUI
    AOFX_LAYER_PROCESS_COUNT = 4
End Enum


<System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)>
Public Structure GpuinfoOutput
    '''int
    Public deviceId As Integer
    '''int
    Public revisionId As Integer
    '''int
    Public iNumCUs As Integer
    '''int
    Public iCoreClock As Integer
    '''int
    Public iMemoryClock As Integer
    '''float
    Public fTFlops As Single

    Public ReturnCode As AGSReturnCode
    '''ArchitectureVersion
    Public version As ArchitectureVersion
    Public ContextPointer As IntPtr
End Structure
Public Enum ArchitectureVersion As Integer
    ArchitectureVersion_Unknown = 0
    ArchitectureVersion_PreGCN = 1
    ArchitectureVersion_GCN = 2
End Enum
Public Enum AGSReturnCode As Integer
    AGS_SUCCESS = 0
    AGS_INVALID_ARGS = 1
    AGS_OUT_OF_MEMORY = 2
    AGS_ERROR_MISSING_DLL = 3
    AGS_ERROR_LEGACY_DRIVER = 4 ' returned if driver doesn't support ADL2 (from before AMD Catalyst driver 12.20)
    AGS_EXTENSION_NOT_SUPPORTED = 5
    AGS_ADL_FAILURE = 6
End Enum
Public Enum AGSDriverExtension As Integer

    '''AGS_EXTENSION_QUADLIST -> 1<<0
    AGS_EXTENSION_QUADLIST = (1) << (0)

    '''AGS_EXTENSION_SCREENRECTLIST -> 1<<1
    AGS_EXTENSION_SCREENRECTLIST = (1) << (1)

    '''AGS_EXTENSION_UAV_OVERLAP -> 1<<2
    AGS_EXTENSION_UAV_OVERLAP = (1) << (2)

    '''AGS_EXTENSION_DEPTH_BOUNDS_TEST -> 1<<3
    AGS_EXTENSION_DEPTH_BOUNDS_TEST = (1) << (3)

    '''AGS_EXTENSION_MULTIDRAWINDIRECT -> 1<<4
    AGS_EXTENSION_MULTIDRAWINDIRECT = (1) << (4)

    '''AGS_EXTENSION_CROSSFIRE_API -> 1<<5
    AGS_EXTENSION_CROSSFIRE_API = (1) << (5)

    '''AGS_EXTENSION_APP_REGISTRATION -> 1<<6
    AGS_EXTENSION_APP_REGISTRATION = (1) << (6)
End Enum
<System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)>
Public Structure DriverExtensions
    '''unsigned int
    Public Supported As AGSDriverExtension

    '''int
    Public ReturnCode As AGSReturnCode
End Structure
<System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)>
Public Structure AGSRect
    '''int
    Public iXOffset As Integer

    '''int
    Public iYOffset As Integer

    '''int
    Public iWidth As Integer

    '''int
    Public iHeight As Integer
End Structure
<System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)>
Public Structure AGSDisplayInfo

    '''int
    Public iGridXCoord As Integer

    '''int
    Public iGridYCoord As Integer

    '''AGSRect
    Public displayRect As AGSRect

    '''AGSRect
    Public displayRectVisible As AGSRect

    '''int
    Public iPreferredDisplay As Integer
End Structure
<System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)>
Public Structure AGSOuput

    '''int
    Public Value As Integer

    '''int
    Public ReturnCode As Integer
End Structure

'
' Summary:
'     Values that indicate how the pipeline interprets vertex data that is bound to
'     the input-assembler stage. These primitive topology values determine how the
'     vertex data is rendered on screen.
'
' Remarks:
'     Use the SharpDX.Direct3D11.InputAssemblerStage.SetPrimitiveTopology method and
'     a value from SharpDX.Direct3D.PrimitiveTopology to bind a primitive topology
'     to the input-assembler stage. Use the SharpDX.Direct3D11.InputAssemblerStage.GetPrimitiveTopology
'     method to retrieve the primitive topology for the input-assembler stage.The following
'     diagram shows the various primitive types for a geometry shader object.
Public Enum AGSPrimitiveTopology As Integer
    '
    ' Summary:
    '     No documentation.
    Undefined = 0
    '
    ' Summary:
    '     No documentation.
    PointList = 1
    '
    ' Summary:
    '     No documentation.
    LineList = 2
    '
    ' Summary:
    '     No documentation.
    LineStrip = 3
    '
    ' Summary:
    '     No documentation.
    TriangleList = 4
    '
    ' Summary:
    '     No documentation.
    TriangleStrip = 5
    '''AGS_PRIMITIVE_TOPOLOGY_QUADLIST -> 7
    QUADLIST = 7

    '''AGS_PRIMITIVE_TOPOLOGY_SCREENRECTLIST -> 9
    SCREENRECTLIST = 9
    '
    ' Summary:
    '     No documentation.
    LineListWithAdjacency = 10
    '
    ' Summary:
    '     No documentation.
    LineStripWithAdjacency = 11
    '
    ' Summary:
    '     No documentation.
    TriangleListWithAdjacency = 12
    '
    ' Summary:
    '     No documentation.
    TriangleStripWithAdjacency = 13
    '
    ' Summary:
    '     No documentation.
    PatchListWith1ControlPoints = 33
    '
    ' Summary:
    '     No documentation.
    PatchListWith2ControlPoints = 34
    '
    ' Summary:
    '     No documentation.
    PatchListWith3ControlPoints = 35
    '
    ' Summary:
    '     No documentation.
    PatchListWith4ControlPoints = 36
    '
    ' Summary:
    '     No documentation.
    PatchListWith5ControlPoints = 37
    '
    ' Summary:
    '     No documentation.
    PatchListWith6ControlPoints = 38
    '
    ' Summary:
    '     No documentation.
    PatchListWith7ControlPoints = 39
    '
    ' Summary:
    '     No documentation.
    PatchListWith8ControlPoints = 40
    '
    ' Summary:
    '     No documentation.
    PatchListWith9ControlPoints = 41
    '
    ' Summary:
    '     No documentation.
    PatchListWith10ControlPoints = 42
    '
    ' Summary:
    '     No documentation.
    PatchListWith11ControlPoints = 43
    '
    ' Summary:
    '     No documentation.
    PatchListWith12ControlPoints = 44
    '
    ' Summary:
    '     No documentation.
    PatchListWith13ControlPoints = 45
    '
    ' Summary:
    '     No documentation.
    PatchListWith14ControlPoints = 46
    '
    ' Summary:
    '     No documentation.
    PatchListWith15ControlPoints = 47
    '
    ' Summary:
    '     No documentation.
    PatchListWith16ControlPoints = 48
    '
    ' Summary:
    '     No documentation.
    PatchListWith17ControlPoints = 49
    '
    ' Summary:
    '     No documentation.
    PatchListWith18ControlPoints = 50
    '
    ' Summary:
    '     No documentation.
    PatchListWith19ControlPoints = 51
    '
    ' Summary:
    '     No documentation.
    PatchListWith20ControlPoints = 52
    '
    ' Summary:
    '     No documentation.
    PatchListWith21ControlPoints = 53
    '
    ' Summary:
    '     No documentation.
    PatchListWith22ControlPoints = 54
    '
    ' Summary:
    '     No documentation.
    PatchListWith23ControlPoints = 55
    '
    ' Summary:
    '     No documentation.
    PatchListWith24ControlPoints = 56
    '
    ' Summary:
    '     No documentation.
    PatchListWith25ControlPoints = 57
    '
    ' Summary:
    '     No documentation.
    PatchListWith26ControlPoints = 58
    '
    ' Summary:
    '     No documentation.
    PatchListWith27ControlPoints = 59
    '
    ' Summary:
    '     No documentation.
    PatchListWith28ControlPoints = 60
    '
    ' Summary:
    '     No documentation.
    PatchListWith29ControlPoints = 61
    '
    ' Summary:
    '     No documentation.
    PatchListWith30ControlPoints = 62
    '
    ' Summary:
    '     No documentation.
    PatchListWith31ControlPoints = 63
    '
    ' Summary:
    '     No documentation.
    PatchListWith32ControlPoints = 64
End Enum
Public Enum AOFX_RETURN_CODE
    AOFX_RETURN_CODE_SUCCESS
    AOFX_RETURN_CODE_FAIL
    AOFX_RETURN_CODE_INVALID_DEVICE
    AOFX_RETURN_CODE_INVALID_DEVICE_CONTEXT
    AOFX_RETURN_CODE_INVALID_ARGUMENT
    AOFX_RETURN_CODE_INVALID_POINTER
    AOFX_RETURN_CODE_D3D11_CALL_FAILED

    AOFX_RETURN_CODE_COUNT
End Enum