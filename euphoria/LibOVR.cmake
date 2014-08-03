set(libovr_include
	../external/LibOVR/Include/OVR.h
	../external/LibOVR/Include/OVR_Kernel.h
	../external/LibOVR/Include/OVR_Version.h
)

set(libovr_src
	../external/LibOVR/Src/OVR_CAPI.cpp
	../external/LibOVR/Src/OVR_CAPI.h
	../external/LibOVR/Src/OVR_CAPI_D3D.h
	../external/LibOVR/Src/OVR_CAPI_GL.h
	../external/LibOVR/Src/OVR_JSON.cpp
	../external/LibOVR/Src/OVR_JSON.h
	../external/LibOVR/Src/OVR_Profile.cpp
	../external/LibOVR/Src/OVR_Profile.h
	../external/LibOVR/Src/OVR_SerialFormat.cpp
	../external/LibOVR/Src/OVR_SerialFormat.h
	../external/LibOVR/Src/OVR_Stereo.cpp
	../external/LibOVR/Src/OVR_Stereo.h
)

set(libovr_capi
	../external/LibOVR/Src/CAPI/CAPI_DistortionRenderer.cpp
	../external/LibOVR/Src/CAPI/CAPI_DistortionRenderer.h
	../external/LibOVR/Src/CAPI/CAPI_FrameTimeManager.cpp
	../external/LibOVR/Src/CAPI/CAPI_FrameTimeManager.h
	../external/LibOVR/Src/CAPI/CAPI_HMDRenderState.cpp
	../external/LibOVR/Src/CAPI/CAPI_HMDRenderState.h
	../external/LibOVR/Src/CAPI/CAPI_HMDState.cpp
	../external/LibOVR/Src/CAPI/CAPI_HMDState.h
	../external/LibOVR/Src/CAPI/CAPI_HSWDisplay.cpp
	../external/LibOVR/Src/CAPI/CAPI_HSWDisplay.h
)

set(libovr_d3d
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D10_DistortionRenderer.cpp
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D10_DistortionRenderer.h
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D10_HSWDisplay.cpp
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D10_HSWDisplay.h
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D11_DistortionRenderer.cpp
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D11_DistortionRenderer.h
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D11_HSWDisplay.cpp
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D11_HSWDisplay.h
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D1X_DistortionRenderer.cpp
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D1X_DistortionRenderer.h
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D1X_HSWDisplay.cpp
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D1X_HSWDisplay.h
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D1X_Util.cpp
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D1X_Util.h
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D9_DistortionRenderer.cpp
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D9_DistortionRenderer.h
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D9_HSWDisplay.cpp
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D9_HSWDisplay.h
	../external/LibOVR/Src/CAPI/D3D1X/CAPI_D3D9_Util.cpp
)

set(libovr_gl
	../external/LibOVR/Src/CAPI/GL/CAPI_GL_DistortionRenderer.cpp
	../external/LibOVR/Src/CAPI/GL/CAPI_GL_DistortionRenderer.h
	../external/LibOVR/Src/CAPI/GL/CAPI_GL_DistortionShaders.h
	../external/LibOVR/Src/CAPI/GL/CAPI_GL_HSWDisplay.cpp
	../external/LibOVR/Src/CAPI/GL/CAPI_GL_HSWDisplay.h
	../external/LibOVR/Src/CAPI/GL/CAPI_GL_Util.cpp
	../external/LibOVR/Src/CAPI/GL/CAPI_GL_Util.h
)

set(libovr_shaders
	../external/LibOVR/Src/CAPI/Shaders/bin2header.exe
	../external/LibOVR/Src/CAPI/Shaders/Distortion_ps.h
	../external/LibOVR/Src/CAPI/Shaders/Distortion_ps.psh
	../external/LibOVR/Src/CAPI/Shaders/Distortion_ps_refl.h
	../external/LibOVR/Src/CAPI/Shaders/Distortion_vs.h
	../external/LibOVR/Src/CAPI/Shaders/Distortion_vs.vsh
	../external/LibOVR/Src/CAPI/Shaders/Distortion_vs_refl.h
	../external/LibOVR/Src/CAPI/Shaders/DistortionChroma_ps.h
	../external/LibOVR/Src/CAPI/Shaders/DistortionChroma_ps.psh
	../external/LibOVR/Src/CAPI/Shaders/DistortionChroma_ps_refl.h
	../external/LibOVR/Src/CAPI/Shaders/DistortionChroma_vs.h
	../external/LibOVR/Src/CAPI/Shaders/DistortionChroma_vs.vsh
	../external/LibOVR/Src/CAPI/Shaders/DistortionChroma_vs_refl.h
	../external/LibOVR/Src/CAPI/Shaders/DistortionTimewarp_vs.h
	../external/LibOVR/Src/CAPI/Shaders/DistortionTimewarp_vs.vsh
	../external/LibOVR/Src/CAPI/Shaders/DistortionTimewarp_vs_refl.h
	../external/LibOVR/Src/CAPI/Shaders/DistortionTimewarpChroma_vs.h
	../external/LibOVR/Src/CAPI/Shaders/DistortionTimewarpChroma_vs.vsh
	../external/LibOVR/Src/CAPI/Shaders/DistortionTimewarpChroma_vs_refl.h
	../external/LibOVR/Src/CAPI/Shaders/genPixelShaderHeader.bat
	../external/LibOVR/Src/CAPI/Shaders/genVertexShaderHeader.bat
	../external/LibOVR/Src/CAPI/Shaders/ShaderReflector.exe
	../external/LibOVR/Src/CAPI/Shaders/SimpleQuad_ps.h
	../external/LibOVR/Src/CAPI/Shaders/SimpleQuad_ps.psh
	../external/LibOVR/Src/CAPI/Shaders/SimpleQuad_ps_refl.h
	../external/LibOVR/Src/CAPI/Shaders/SimpleQuad_vs.h
	../external/LibOVR/Src/CAPI/Shaders/SimpleQuad_vs.vsh
	../external/LibOVR/Src/CAPI/Shaders/SimpleQuad_vs_refl.h
	../external/LibOVR/Src/CAPI/Shaders/SimpleTexturedQuad_ps.h
	../external/LibOVR/Src/CAPI/Shaders/SimpleTexturedQuad_ps.psh
	../external/LibOVR/Src/CAPI/Shaders/SimpleTexturedQuad_ps_refl.h
	../external/LibOVR/Src/CAPI/Shaders/SimpleTexturedQuad_vs.h
	../external/LibOVR/Src/CAPI/Shaders/SimpleTexturedQuad_vs.vsh
	../external/LibOVR/Src/CAPI/Shaders/SimpleTexturedQuad_vs_refl.h
)

set(libovr_textures
	../external/LibOVR/Src/CAPI/Textures/healthAndSafety.tga.h
)

set(libovr_displays
	../external/LibOVR/Src/Displays/OVR_Display.cpp
	../external/LibOVR/Src/Displays/OVR_Display.h
	../external/LibOVR/Src/Displays/OVR_Win32_Display.cpp
	../external/LibOVR/Src/Displays/OVR_Win32_Display.h
	../external/LibOVR/Src/Displays/OVR_Win32_Dxgi_Display.h
	../external/LibOVR/Src/Displays/OVR_Win32_FocusReader.cpp
	../external/LibOVR/Src/Displays/OVR_Win32_FocusReader.h
	../external/LibOVR/Src/Displays/OVR_Win32_RenderShim.cpp
	../external/LibOVR/Src/Displays/OVR_Win32_ShimFunctions.cpp
	../external/LibOVR/Src/Displays/OVR_Win32_ShimFunctions.h
)

set(libovr_kernel
	../external/LibOVR/Src/Kernel/OVR_Alg.cpp
	../external/LibOVR/Src/Kernel/OVR_Alg.h
	../external/LibOVR/Src/Kernel/OVR_Allocator.cpp
	../external/LibOVR/Src/Kernel/OVR_Allocator.h
	../external/LibOVR/Src/Kernel/OVR_Array.h
	../external/LibOVR/Src/Kernel/OVR_Atomic.cpp
	../external/LibOVR/Src/Kernel/OVR_Atomic.h
	../external/LibOVR/Src/Kernel/OVR_Color.h
	../external/LibOVR/Src/Kernel/OVR_Compiler.h
	../external/LibOVR/Src/Kernel/OVR_ContainerAllocator.h
	../external/LibOVR/Src/Kernel/OVR_CRC32.cpp
	../external/LibOVR/Src/Kernel/OVR_CRC32.h
	../external/LibOVR/Src/Kernel/OVR_Delegates.h
	../external/LibOVR/Src/Kernel/OVR_Deque.h
	../external/LibOVR/Src/Kernel/OVR_File.cpp
	../external/LibOVR/Src/Kernel/OVR_File.h
	../external/LibOVR/Src/Kernel/OVR_FileFILE.cpp
	../external/LibOVR/Src/Kernel/OVR_Hash.h
	../external/LibOVR/Src/Kernel/OVR_KeyCodes.h
	../external/LibOVR/Src/Kernel/OVR_List.h
	../external/LibOVR/Src/Kernel/OVR_Lockless.cpp
	../external/LibOVR/Src/Kernel/OVR_Lockless.h
	../external/LibOVR/Src/Kernel/OVR_Log.cpp
	../external/LibOVR/Src/Kernel/OVR_Log.h
	../external/LibOVR/Src/Kernel/OVR_Math.cpp
	../external/LibOVR/Src/Kernel/OVR_Math.h
	../external/LibOVR/Src/Kernel/OVR_Nullptr.h
	../external/LibOVR/Src/Kernel/OVR_Observer.h
	../external/LibOVR/Src/Kernel/OVR_RefCount.cpp
	../external/LibOVR/Src/Kernel/OVR_RefCount.h
	../external/LibOVR/Src/Kernel/OVR_SharedMemory.cpp
	../external/LibOVR/Src/Kernel/OVR_SharedMemory.h
	../external/LibOVR/Src/Kernel/OVR_Std.cpp
	../external/LibOVR/Src/Kernel/OVR_Std.h
	../external/LibOVR/Src/Kernel/OVR_String.cpp
	../external/LibOVR/Src/Kernel/OVR_String.h
	../external/LibOVR/Src/Kernel/OVR_String_FormatUtil.cpp
	../external/LibOVR/Src/Kernel/OVR_String_PathUtil.cpp
	../external/LibOVR/Src/Kernel/OVR_StringHash.h
	../external/LibOVR/Src/Kernel/OVR_SysFile.cpp
	../external/LibOVR/Src/Kernel/OVR_SysFile.h
	../external/LibOVR/Src/Kernel/OVR_System.cpp
	../external/LibOVR/Src/Kernel/OVR_System.h
	../external/LibOVR/Src/Kernel/OVR_ThreadCommandQueue.cpp
	../external/LibOVR/Src/Kernel/OVR_ThreadCommandQueue.h
	../external/LibOVR/Src/Kernel/OVR_Threads.h
	../external/LibOVR/Src/Kernel/OVR_ThreadsWinAPI.cpp
	../external/LibOVR/Src/Kernel/OVR_Timer.cpp
	../external/LibOVR/Src/Kernel/OVR_Timer.h
	../external/LibOVR/Src/Kernel/OVR_Types.h
	../external/LibOVR/Src/Kernel/OVR_UTF8Util.cpp
	../external/LibOVR/Src/Kernel/OVR_UTF8Util.h
)

set(libovr_net
	../external/LibOVR/Src/Net/OVR_BitStream.cpp
	../external/LibOVR/Src/Net/OVR_BitStream.h
	../external/LibOVR/Src/Net/OVR_MessageIDTypes.h
	../external/LibOVR/Src/Net/OVR_NetworkPlugin.cpp
	../external/LibOVR/Src/Net/OVR_NetworkPlugin.h
	../external/LibOVR/Src/Net/OVR_NetworkTypes.h
	../external/LibOVR/Src/Net/OVR_PacketizedTCPSocket.cpp
	../external/LibOVR/Src/Net/OVR_PacketizedTCPSocket.h
	../external/LibOVR/Src/Net/OVR_RPC1.cpp
	../external/LibOVR/Src/Net/OVR_RPC1.h
	../external/LibOVR/Src/Net/OVR_Session.cpp
	../external/LibOVR/Src/Net/OVR_Session.h
	../external/LibOVR/Src/Net/OVR_Socket.cpp
	../external/LibOVR/Src/Net/OVR_Socket.h
	../external/LibOVR/Src/Net/OVR_Win32_Socket.cpp
	../external/LibOVR/Src/Net/OVR_Win32_Socket.h
)

set(libovr_sensors
	../external/LibOVR/Src/Sensors/OVR_DeviceConstants.h
)

set(libovr_service
	../external/LibOVR/Src/Service/Service_NetClient.cpp
	../external/LibOVR/Src/Service/Service_NetClient.h
	../external/LibOVR/Src/Service/Service_NetSessionCommon.cpp
	../external/LibOVR/Src/Service/Service_NetSessionCommon.h
)

set(libovr_tracking
	../external/LibOVR/Src/Tracking/Tracking_PoseState.h
	../external/LibOVR/Src/Tracking/Tracking_SensorState.h
	../external/LibOVR/Src/Tracking/Tracking_SensorStateReader.cpp
	../external/LibOVR/Src/Tracking/Tracking_SensorStateReader.h
)

set(libovr_util
	../external/LibOVR/Src/Util/Util_ImageWindow.cpp
	../external/LibOVR/Src/Util/Util_ImageWindow.h
	../external/LibOVR/Src/Util/Util_Interface.cpp
	../external/LibOVR/Src/Util/Util_Interface.h
	../external/LibOVR/Src/Util/Util_LatencyTest2Reader.cpp
	../external/LibOVR/Src/Util/Util_LatencyTest2Reader.h
	../external/LibOVR/Src/Util/Util_LatencyTest2State.h
	../external/LibOVR/Src/Util/Util_LongPollThread.cpp
	../external/LibOVR/Src/Util/Util_LongPollThread.h
	../external/LibOVR/Src/Util/Util_Render_Stereo.cpp
	../external/LibOVR/Src/Util/Util_Render_Stereo.h
	../external/LibOVR/Src/Util/Util_Settings.cpp
	../external/LibOVR/Src/Util/Util_Settings.h
)

source_group(LibOVR\\Include FILES ${libovr_include})
source_group(LibOVR\\Src FILES ${libovr_src})
source_group(LibOVR\\Src\\CAPI FILES ${libovr_capi})
source_group(LibOVR\\Src\\CAPI\\D3D1X FILES ${libovr_d3d})
source_group(LibOVR\\Src\\CAPI\\GL FILES ${libovr_gl})
source_group(LibOVR\\Src\\CAPI\\Shaders FILES ${libovr_shaders})
source_group(LibOVR\\Src\\CAPI\\Textures FILES ${libovr_textures})
source_group(LibOVR\\Src\\Displays FILES ${libovr_displays})
source_group(LibOVR\\Src\\Kernel FILES ${libovr_kernel})
source_group(LibOVR\\Src\\Net FILES ${libovr_net})
source_group(LibOVR\\Src\\Sensors FILES ${libovr_sensors})
source_group(LibOVR\\Src\\Service FILES ${libovr_service})
source_group(LibOVR\\Src\\Tracking FILES ${libovr_tracking})

set(src_oculus 
	${libovr_include}
	${libovr_src}
	${libovr_capi}
	${libovr_d3d}
	${libovr_gl}
	${libovr_shaders}
	${libovr_textures}
	${libovr_displays}
	${libovr_kernel}
	${libovr_net}
	${libovr_sensors}
	${libovr_service}
	${libovr_tracking}
)

