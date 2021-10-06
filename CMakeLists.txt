# CMAKE version of Ubuntu 16.04 is 3.5.
cmake_minimum_required(VERSION 3.5)
project(test)

set(LINKLIBS)
set(COMPILE_OPTION)
set(SRC)
set(INCL)
set(SRC_Qt)
set(SRC_Qt_MOC)

macro(display_elements
	DISPLAY_MESSAGE
	LIST_TO_DISPLAY)
    message("[ ${DISPLAY_MESSAGE} ]")
    foreach(_DISPLAY_ELEMENT ${LIST_TO_DISPLAY})
	message(STATUS ${_DISPLAY_ELEMENT})
    endforeach()
endmacro()


## MOC(Meta Object Compiler) of QtFramework
	file(MAKE_DIRECTORY tmp)
#display_elements(MOC_TARGET ${SRC_Qt_MOC})
	## sources to be processed by MOC
	list(APPEND SRC_Qt_MOC
		## TODO
		${CMAKE_CURRENT_LIST_DIR}/KWidgetASR.h

	)	
	
	## sources without MetaObject 
	list(APPEND SRC_Qt
		${CMAKE_CURRENT_LIST_DIR}/KWidgetASR.cpp
		)
	
	foreach(i  ${SRC_Qt_MOC})
		string(REPLACE "/" ";" parse_i_1 ${i})
		LIST(GET parse_i_1 -1 parse_i_2)
		string(REPLACE "." ";" parse_i_3 ${parse_i_2})
		LIST(GET parse_i_3 0 fisrt_element)
		
		if(MSVC)
			execute_process(
			COMMAND moc ${i} -o tmp/moc_${fisrt_element}.cpp
			WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin  
			 )
		endif()
		
		list(APPEND SRC_Qt
			tmp/moc_${fisrt_element}.cpp
		)
	endforeach()
	list(APPEND SRC ${SRC_Qt})
	list(APPEND SRC ${SRC_Qt_MOC})

################

list(APPEND SRC
	${CMAKE_CURRENT_LIST_DIR}/test.cpp
	${CMAKE_CURRENT_LIST_DIR}/ASR_ETRI.h
  )
list(APPEND INCL
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/lib/WAV
  ${CMAKE_CURRENT_LIST_DIR}/lib/curl/include/curl
  ${CMAKE_CURRENT_LIST_DIR}/lib/curl/include/openssl
  ${CMAKE_CURRENT_LIST_DIR}/lib/json/single_include/nlohmann
  
  	# QtFramework
	bin/include
	bin/include/QtCore
	bin/include/QtCore5Compat
	bin/include/QtGui
	bin/include/QtWidgets
	bin/include/QtOpenGL
	bin/include/QtOpenGLWidgets
  )
list(APPEND LINKLIBS
	#${CMAKE_CURRENT_LIST_DIR}/lib/curl/libcrypto.lib
	#${CMAKE_CURRENT_LIST_DIR}/lib/curl/libssl.lib
	${CMAKE_CURRENT_LIST_DIR}/bin/LIBCURL.LIB
	debug ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Cored.lib optimized ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Core.lib
	debug ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Guid.lib optimized ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Gui.lib
	debug ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Widgetsd.lib optimized ${PROJECT_SOURCE_DIR}/bin/lib/Qt6Widgets.lib	
	debug ${PROJECT_SOURCE_DIR}/bin/lib/Qt6OpenGLWidgetsd.lib optimized ${PROJECT_SOURCE_DIR}/bin/lib/Qt6OpenGLWidgets.lib	
  )
if(MSVC)
  list(APPEND COMPILE_OPTION
	-DCURL_STATICLIB
    )
else()
  list(APPEND COMPILE_OPTION
    )
endif()

display_elements(SRC "${SRC}")

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME}    PRIVATE    ${INCL}  )
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )
set_target_properties(${PROJECT_NAME} PROPERTIES  CXX_STANDARD 17 )