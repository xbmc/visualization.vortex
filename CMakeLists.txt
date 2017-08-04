project(visualization.vortex)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

find_package(Kodi REQUIRED)
find_package(p8-platform REQUIRED)
find_package(kodiplatform REQUIRED)

set(DEPLIBS winmm.lib ${p8-platform_LIBRARIES} ${kodiplatform_LIBRARIES})

include_directories(src/angelscript/add_on
                    src/angelscript/include
                    src/core src/effects src/shaders
                    ${KODI_INCLUDE_DIR}
                    ${p8-latform_INCLUDE_DIRS}
                    ${kodiplatform_INCLUDE_DIRS})

set(SHADER_FILES src/shaders/ColorPixelShader.hlsl
                 src/shaders/ColourRemapPixelShader.hlsl
                 src/shaders/DiffuseNormalEnvCubeVertexShader.hlsl
                 src/shaders/DiffuseUVCubeVertexShader.hlsl
                 src/shaders/DiffuseUVEnvCubeVertexShader.hlsl
                 src/shaders/DiffuseUVEnvVertexShader.hlsl
                 src/shaders/DiffuseUVVertexShader.hlsl
                 src/shaders/TexturePixelShader.hlsl
                 src/shaders/UVNormalEnvVertexShader.hlsl)
set(SHADER_INCLUDES )

foreach(SHADER_FILE ${SHADER_FILES})
  get_filename_component(FileName ${SHADER_FILE} NAME_WE)
  if(${FileName} MATCHES "(.+Vertex.+)")
    set(ShaderType vs)
  elseif(${FileName} MATCHES "(.+Pixel.+)")
    set(ShaderType ps)
  else()
    message(FATAL_ERROR "Unknown shader type.")
  endif()
  set(IncFile src/shaders/${FileName}.inc)
  list(APPEND SHADER_INCLUDES ${IncFile})

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${IncFile}
    COMMAND fxc /E Main /nologo /T${ShaderType}_4_0_level_9_1 /Vn ${FileName}Code /Fh ${IncFile} ${SHADER_FILE} /I ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
    DEPENDS ${SHADER_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/constants_table.fx
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM)
endforeach(SHADER_FILE)

add_custom_target(generate ALL DEPENDS ${SHADER_INCLUDES})

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
                   src/core/CommonStates.cpp
                   src/core/DDSTextureLoader.cpp
                   src/core/DebugConsole.cpp
                   src/core/EffectBase.cpp
                   src/core/fft.cpp
                   src/core/Mesh.cpp
                   src/core/Preset.cpp
                   src/core/Renderer.cpp
                   src/core/Shader.cpp
                   src/core/Texture.cpp
                   src/core/TextureDX.cpp
                   src/core/Vortex.cpp
                   src/core/WICTextureLoader.cpp
                   src/core/XmlDocument.cpp
                   src/core/XMMatrixStack.cpp
                   src/effects/Map.cpp
                   src/effects/Tunnel.cpp
                   src/effects/VoicePrint.cpp)

set(VORTEX_HEADERS src/angelscript/add_on/scriptstring.h
                   src/angelscript/include/angelscript.h
                   src/angelscript/source/as_array.h
                   src/angelscript/source/as_arrayobject.h
                   src/angelscript/source/as_atomic.h
                   src/angelscript/source/as_builder.h
                   src/angelscript/source/as_bytecode.h
                   src/angelscript/source/as_callfunc.h
                   src/angelscript/source/as_compiler.h
                   src/angelscript/source/as_configgroup.h
                   src/angelscript/source/as_config.h
                   src/angelscript/source/as_context.h
                   src/angelscript/source/as_criticalsection.h
                   src/angelscript/source/as_datatype.h
                   src/angelscript/source/as_debug.h
                   src/angelscript/source/as_gc.h
                   src/angelscript/source/as_generic.h
                   src/angelscript/source/as_map.h
                   src/angelscript/source/as_memory.h
                   src/angelscript/source/as_module.h
                   src/angelscript/source/as_objecttype.h
                   src/angelscript/source/as_outputbuffer.h
                   src/angelscript/source/as_parser.h
                   src/angelscript/source/as_property.h
                   src/angelscript/source/as_restore.h
                   src/angelscript/source/as_scriptcode.h
                   src/angelscript/source/as_scriptengine.h
                   src/angelscript/source/as_scriptfunction.h
                   src/angelscript/source/as_scriptnode.h
                   src/angelscript/source/as_scriptobject.h
                   src/angelscript/source/as_string.h
                   src/angelscript/source/as_string_util.h
                   src/angelscript/source/as_texts.h
                   src/angelscript/source/as_thread.h
                   src/angelscript/source/as_tokendef.h
                   src/angelscript/source/as_tokenizer.h
                   src/angelscript/source/as_typeinfo.h
                   src/angelscript/source/as_variablescope.h
                   src/core/CommonStates.h
                   src/core/dds.h
                   src/core/DDSTextureLoader.h
                   src/core/DebugConsole.h
                   src/core/DemandCreate.h
                   src/core/DirectXHelpers.h
                   src/core/EffectBase.h
                   src/core/fft.h
                   src/core/Mesh.h
                   src/core/Preset.h
                   src/core/Renderer.h
                   src/core/Shader.h
                   src/core/SharedResourcePool.h
                   src/core/TextureDX.h
                   src/core/Texture.h
                   src/core/Vortex.h
                   src/core/WICTextureLoader.h
                   src/core/XBMCTypes.h
                   src/core/XmlDocument.h
                   src/core/XMMatrixStack.h
                   src/effects/Map.h
                   src/effects/Tunnel.h
                   src/effects/VoicePrint.h)

if(CORE_SYSTEM_NAME STREQUAL windowsstore)
  add_definitions(-DVORTEX_EXPORTS -D_WIN32_WINNT=0x0A00 -D_WINDOWS -D_CRT_NONSTDC_NO_DEPRECATE)
else()
  add_definitions(-D_USRDLL -DVORTEX_EXPORTS -D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(-D_WIN32_WINNT=0x0600 -D_WINDOWS)
endif()
remove_definitions(-DNOMINMAX)

build_addon(visualization.vortex VORTEX DEPLIBS)

include(CPack)
