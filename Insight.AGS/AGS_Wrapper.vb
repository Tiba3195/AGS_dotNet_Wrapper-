Imports System.Runtime.InteropServices
Imports System.Text
Imports SharpDX
Imports SharpDX.Direct3D11

Public Class AGS_Wrapper

    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function main() As Integer
    End Function
    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function DeInt(<MarshalAs(UnmanagedType.SysInt)> Context As IntPtr) As AGSReturnCode
    End Function
    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function GetInfo() As <MarshalAs(UnmanagedType.Struct)> GpuinfoOutput
    End Function
    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function DriverExtensions_Init(<MarshalAs(UnmanagedType.SysInt)> Context As IntPtr, <MarshalAs(UnmanagedType.SysInt)> Device As IntPtr) As <MarshalAs(UnmanagedType.Struct)> DriverExtensions
    End Function
    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function GetCrossfireGPUCount(<MarshalAs(UnmanagedType.SysInt)> Context As IntPtr) As AGSOuput
    End Function

    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function GetTotalGPUCount(<MarshalAs(UnmanagedType.SysInt)> Context As IntPtr) As AGSOuput
    End Function

    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function GetGPUMemorySize(<MarshalAs(UnmanagedType.SysInt)> Context As IntPtr, GPUIndex As Integer) As AGSOuput
    End Function

    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function DriverExtensions_DeInit(<MarshalAs(UnmanagedType.SysInt)> Context As IntPtr) As AGSReturnCode
    End Function

    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function InitializeAOFX(<MarshalAs(UnmanagedType.SysInt)> Device As IntPtr, <MarshalAs(UnmanagedType.SysInt)> DeviceContext As IntPtr) As AOFX_RETURN_CODE
    End Function
    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function ReleaseAOFX() As AOFX_RETURN_CODE
    End Function
    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function ReSizeAOFX(w As Integer, h As Integer) As AOFX_RETURN_CODE
    End Function

    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function RenderAOFX(<MarshalAs(UnmanagedType.Struct)> ByRef Input As AOFXInput, layercount As Integer) As AOFX_RETURN_CODE
    End Function


    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function SHProject(<MarshalAs(UnmanagedType.SysInt)> Context As IntPtr, <MarshalAs(UnmanagedType.SysInt)> CubeMap As IntPtr, order As Integer,
       <Out> ByRef resultR As IntPtr, <Out> ByRef resultG As IntPtr, <Out> ByRef resultB As IntPtr) As Integer
    End Function

    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Public Shared Sub FreeArrayFloat(outputs As IntPtr)
    End Sub
    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Public Shared Sub FreeArrayInt(outputs As IntPtr)
    End Sub
    <DllImport("AGS_Wrapper.dll", SetLastError:=True, CallingConvention:=CallingConvention.StdCall)>
    Private Shared Function CreateRandomTexture(<MarshalAs(UnmanagedType.SysInt)> ID3D11Device As IntPtr, RANDOM_TEXTURE_WIDTH As Integer, NUM_DIRECTIONS As Integer,
       <Out> ByRef ID3D11Texture2D As IntPtr) As Integer
    End Function

    Private AOFX_Result As AOFX_RETURN_CODE
    Private GPUInfo As GpuinfoOutput
    Private AGSContext As IntPtr
    Private SupportedExtensions As AGSDriverExtension

    Private AGS_Setup As Boolean
    Private Has_Driver_Extensions As Boolean

    Public Function InitAGS(ByRef D3D11Device As IntPtr) As GpuinfoOutput
        Dim DriverExtensionsTest As DriverExtensions


        GPUInfo = GetInfo()

        AGSContext = GPUInfo.ContextPointer

        If GPUInfo.ReturnCode = AGSReturnCode.AGS_SUCCESS Then
            AGS_Setup = True
            DriverExtensionsTest = DriverExtensions_Init(AGSContext, D3D11Device)
        Else
            Throw New Exception("Failed to load AGS " & GPUInfo.ReturnCode.ToString)
        End If
        If DriverExtensionsTest.ReturnCode = AGSReturnCode.AGS_SUCCESS Then
            Has_Driver_Extensions = True
            SupportedExtensions = DriverExtensionsTest.Supported
        Else
            Throw New Exception("Failed to load Driver Extensions " & DriverExtensionsTest.ReturnCode.ToString)
        End If

        Return GPUInfo
    End Function

    Public Function SetAOFXDefults() As AOFXInput
        Dim blah As AOFXInput
        blah.m_LayerProcess = AOFX_LAYER_PROCESS.AOFX_LAYER_PROCESS_DEINTERLEAVE_NONE
        blah.m_MultiResLayerScale = 2.0F
        blah.m_PowIntensity = 1.0F
        blah.m_BilateralBlurRadius = AOFX_BILATERAL_BLUR_RADIUS.AOFX_BILATERAL_BLUR_RADIUS_16
        blah.m_ViewDistanceDiscard = 100.0F
        blah.m_ViewDistanceFade = 99.0F
        blah.m_NormalScale = 0.1F
        blah.m_LinearIntensity = 0.6F
        blah.m_AcceptRadius = 0.003F
        blah.m_RejectRadius = 0.8F
        blah.m_RecipFadeOutDist = 6.0F
        blah.m_DepthUpsampleThreshold = 0.05F
        blah.m_SampleCount = AOFX_SAMPLE_COUNT.AOFX_SAMPLE_COUNT_LOW
        blah.m_NormalOption = AOFX_NORMAL_OPTION.AOFX_NORMAL_OPTION_NONE
        blah.m_TapType = AOFX_TAP_TYPE.AOFX_TAP_TYPE_FIXED
        blah.m_Implementation = AOFX_IMPLEMENTATION_MASK.AOFX_IMPLEMENTATION_MASK_BLUR_CS Or AOFX_IMPLEMENTATION_MASK.AOFX_IMPLEMENTATION_MASK_KERNEL_CS Or AOFX_IMPLEMENTATION_MASK.AOFX_IMPLEMENTATION_MASK_UTILITY_CS
    End Function

    Public Sub SetupAOFX(device As IntPtr, devicecontext As IntPtr)
        AOFX_Result = InitializeAOFX(device, devicecontext)

        If AOFX_Result <> AOFX_RETURN_CODE.AOFX_RETURN_CODE_SUCCESS Then
            Throw New Exception("Failed to load AOFX " & AOFX_Result.ToString)
        End If
    End Sub
    Public Sub CloseAOFX()
        AOFX_Result = ReleaseAOFX()

        If AOFX_Result <> AOFX_RETURN_CODE.AOFX_RETURN_CODE_SUCCESS Then
            Throw New Exception("Failed to Close AOFX " & AOFX_Result.ToString)
        End If
    End Sub

    Public Sub SetAOFXOptions(w As Integer, h As Integer)
        AOFX_Result = ReSizeAOFX(w, h)

        If AOFX_Result <> AOFX_RETURN_CODE.AOFX_RETURN_CODE_SUCCESS Then
            Throw New Exception("Failed to load Change AOFX Settings " & AOFX_Result.ToString)
        End If
    End Sub
    Public Function BuildRandomTexture(ID3D11Device As IntPtr, RANDOM_TEXTURE_WIDTH As Integer, NUM_DIRECTIONS As Integer) As Texture2D
        Try
            Dim Temp As Texture2D
            Dim SHR As IntPtr
            Dim HRESULT As Integer = CreateRandomTexture(ID3D11Device, RANDOM_TEXTURE_WIDTH, NUM_DIRECTIONS, SHR)
            Temp = New Texture2D(SHR)
            If HRESULT = 1 Then
                Return Temp
            Else
                Return Nothing
            End If

        Catch ex As Exception
            Return Nothing
        End Try

    End Function

    Public Function SHProjectCubeMap(Context As IntPtr, CubeMap As IntPtr, Order As Integer) As Color3()
        Dim SHR As IntPtr
        Dim SHG As IntPtr
        Dim SHB As IntPtr
        Try

            Dim BuiltSH As Color3() = New Color3(Order * Order - 1) {}
            Dim HRESULT As Integer = SHProject(Context, CubeMap, Order, SHR, SHG, SHB)
            Dim _shr As Single() = New Single(Order * Order - 1) {}
            Dim _shg As Single() = New Single(Order * Order - 1) {}
            Dim _shb As Single() = New Single(Order * Order - 1) {}
            Marshal.Copy(SHR, _shr, 0, Order * Order)
            Marshal.Copy(SHG, _shg, 0, Order * Order)
            Marshal.Copy(SHB, _shb, 0, Order * Order)
            If HRESULT = 1 Then
                For i As Integer = 0 To Order * Order - 1
                    Dim SHVector As Color3 = New Color3(_shr(i), _shg(i), _shb(i))
                    BuiltSH(i) = SHVector
                Next
                FreeArrayFloat(SHR)
                FreeArrayFloat(SHG)
                FreeArrayFloat(SHB)
                Return BuiltSH
            Else
                Return New Color3(Order * Order) {}
            End If
        Catch ex As Exception
            If SHR = Nothing Then
                FreeArrayFloat(SHR)
            End If
            If SHG = Nothing Then
                FreeArrayFloat(SHG)
            End If
            If SHB = Nothing Then
                FreeArrayFloat(SHB)
            End If
            Return New Color3(Order * Order) {}
        End Try

    End Function

    Public Sub RenderAOFX(Depth As IntPtr, Normals As IntPtr, OutputRT As IntPtr, LayerCOunt As Integer)
        Dim AOFXSettings As AOFXInput = SetAOFXDefults()
        AOFXSettings.m_pDepthSRV = Depth
        AOFXSettings.m_pNormalSRV = Normals
        AOFXSettings.m_pOutputRTV = OutputRT

        AOFX_Result = RenderAOFX(AOFXSettings, LayerCOunt)

        If AOFX_Result <> AOFX_RETURN_CODE.AOFX_RETURN_CODE_SUCCESS Then
            Throw New Exception("Failed to Render AOFX " & AOFX_Result.ToString)
        End If
    End Sub

    Public Function GetGPUCount() As Integer
        Dim out0 As AGSOuput = GetTotalGPUCount(AGSContext)
        If out0.ReturnCode = AGSReturnCode.AGS_SUCCESS Then
            Return out0.Value
        Else
            Return -1
        End If
    End Function
    Public Function GetCrossFireGPUCount() As Integer
        Dim out0 As AGSOuput = GetCrossfireGPUCount(AGSContext)
        If out0.ReturnCode = AGSReturnCode.AGS_SUCCESS Then
            Return out0.Value
        Else
            Return -1
        End If
    End Function
    Public Function GetMemorySize(GPUIndex As Integer) As Integer
        Dim out0 As AGSOuput = GetGPUMemorySize(AGSContext, GPUIndex)
        If out0.ReturnCode = AGSReturnCode.AGS_SUCCESS Then
            Return out0.Value
        Else
            Return -1
        End If
    End Function
    Public Sub CloseAGS()
        Dim outtester As AGSReturnCode

        outtester = DriverExtensions_DeInit(AGSContext)

        If outtester <> AGSReturnCode.AGS_SUCCESS Then
            Throw New Exception("Failed to Clean Up Driver Extensions " & outtester.ToString)
        End If

        outtester = DeInt(AGSContext)

        If outtester <> AGSReturnCode.AGS_SUCCESS Then
            Throw New Exception("Failed to Clean Up AGS " & outtester.ToString)
        End If
    End Sub


End Class
