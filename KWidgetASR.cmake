################################################################
#        CMake file for using KWidgetASR   as submodule        #
################################################################

file(COPY ${CMAKE_CURRENT_LIST_DIR}/move_contents_to_bin/libcurl.dll
     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/move_contents_to_bin/libcurl.lib
     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

list(APPEND SRC_Qt_MOC
	${CMAKE_CURRENT_LIST_DIR}/KWidgetASR.h
)

list(APPEND SRC_Qt
	${CMAKE_CURRENT_LIST_DIR}/KWidgetASR.cpp
)

list(APPEND SRC
	${CMAKE_CURRENT_LIST_DIR}/ASR_ETRI.h
)
	
list(APPEND INCL
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/lib/WAV
	${CMAKE_CURRENT_LIST_DIR}/lib/curl/include/curl
	${CMAKE_CURRENT_LIST_DIR}/lib/curl/include/openssl
	${CMAKE_CURRENT_SOURCE_DIR}/lib/json/single_include/nlohmann
)

if(UNIX AND NOT APPLE)
	list(APPEND LINKLIBS
	)
elseif(MSVC)
	list(APPEND LINKLIBS
	${CMAKE_CURRENT_SOURCE_DIR}/bin/libcurl.lib
	
	)
endif()