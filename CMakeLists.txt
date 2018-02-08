# If we don't need RTTI or EH, there's no reason to export anything
# from the hello plugin.
if( NOT LLVM_REQUIRES_RTTI )
  if( NOT LLVM_REQUIRES_EH )
    set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/MyCodeGen.exports)
  endif()
endif()

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS Core Support)
endif()

aux_source_directory(. SRC_LIST)

add_llvm_loadable_module( MyDeadCodeGen
	MyCodeGen.cpp
  CodeGen.cpp
  Common.h
  log.h
  SemanticGen.h
  SemanticGen0.h
  UtilTool.h
  IncludeFile.h
  Common.cpp
    
  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
