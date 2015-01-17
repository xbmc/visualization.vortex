project(visualization.vortex)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

find_package(kodi REQUIRED)

find_package(DirectX REQUIRED)
set(DEPLIBS winmm.lib ${DIRECTX_D3DX9_LIBRARY} ${kodiplatform_LIBRARIES})

include_directories(src/angelscript/include
                    src/core src/effects
                    ${DIRECTX_INCLUDE_DIR}
                    ${KODI_INCLUDE_DIR}
                    ${kodiplatform_INCLUDE_DIRS})

set(VORTEX_SOURCES src/VortexXBMC.cpp
                   src/angelscript/source/as_arrayobject.cpp
                   src/angelscript/source/as_atomic.cpp
                   src/angelscript/source/as_builder.cpp
                   src/angelscript/source/as_bytecode.cpp
                   src/angelscript/source/as_callfunc.cpp
                   src/angelscript/source/as_callfunc_arm.cpp
                   src/angelscript/source/as_callfunc_mips.cpp
                   src/angelscript/source/as_callfunc_ppc.cpp
                   src/angelscript/source/as_callfunc_ppc_64.cpp
                   src/angelscript/source/as_callfunc_sh4.cpp
                   src/angelscript/source/as_callfunc_x64_gcc.cpp
                   src/angelscript/source/as_callfunc_x86.cpp
                   src/angelscript/source/as_callfunc_xenon.cpp
                   src/angelscript/source/as_compiler.cpp
                   src/angelscript/source/as_configgroup.cpp
                   src/angelscript/source/as_context.cpp
                   src/angelscript/source/as_datatype.cpp
                   src/angelscript/source/as_gc.cpp
                   src/angelscript/source/as_generic.cpp
                   src/angelscript/source/as_globalproperty.cpp
                   src/angelscript/source/as_memory.cpp
                   src/angelscript/source/as_module.cpp
                   src/angelscript/source/as_objecttype.cpp
                   src/angelscript/source/as_outputbuffer.cpp
                   src/angelscript/source/as_parser.cpp
                   src/angelscript/source/as_restore.cpp
                   src/angelscript/source/as_scriptcode.cpp
                   src/angelscript/source/as_scriptengine.cpp
                   src/angelscript/source/as_scriptfunction.cpp
                   src/angelscript/source/as_scriptnode.cpp
                   src/angelscript/source/as_scriptobject.cpp
                   src/angelscript/source/as_string.cpp
                   src/angelscript/source/as_string_util.cpp
                   src/angelscript/source/as_thread.cpp
                   src/angelscript/source/as_tokenizer.cpp
                   src/angelscript/source/as_typeinfo.cpp
                   src/angelscript/source/as_variablescope.cpp
                   src/angelscript/add_on/scriptstring.cpp
                   src/angelscript/add_on/scriptstring_utils.cpp
                   src/core/DebugConsole.cpp
                   src/core/EffectBase.cpp
                   src/core/fft.cpp
                   src/core/Mesh.cpp
                   src/core/Preset.cpp
                   src/core/Renderer.cpp
                   src/core/Shader.cpp
                   src/core/Texture.cpp
                   src/core/Vortex.cpp
                   src/core/XmlDocument.cpp
                   src/effects/Map.cpp
                   src/effects/Tunnel.cpp
                   src/effects/VoicePrint.cpp)

add_definitions(-D_USRDLL -DVORTEX_EXPORTS -D_CRT_NONSTDC_NO_DEPRECATE)

build_addon(visualization.vortex VORTEX DEPLIBS)

include(CPack)
