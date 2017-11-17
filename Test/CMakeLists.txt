cmake_minimum_required( VERSION 2.6 )

project( FalconTest )

#######################################
# Include HDAL (Novint Falcon library)
#######################################
 
find_path( HDAL_ROOT_DIR include/hdl/hdl.h $ENV{NOVINT_DEVICE_SUPPORT} )

include_directories( ${HDAL_ROOT_DIR}/include )
link_directories( ${HDAL_ROOT_DIR}/lib )

set( HDAL_LIB hdl.lib )

#######################################
# Include Falcon and FalconTest code
#######################################

include_directories( ${FalconUnityPlugin_SOURCE_DIR} )

set( SRC FalconTest.cpp
         ${FalconUnityPlugin_SOURCE_DIR}/Falcon.cpp )

add_executable( FalconTest ${SRC} )
target_link_libraries( FalconTest ${HDAL_LIB} )