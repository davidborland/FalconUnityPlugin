cmake_minimum_required( VERSION 2.6 )


project( FalconUnityPlugin )


#######################################
# Include HDAL (Novint Falcon library)
#######################################
 

find_path( HDAL_ROOT_DIR include/hdl/hdl.h $ENV{NOVINT_DEVICE_SUPPORT} )

include_directories( ${HDAL_ROOT_DIR}/include )
link_directories( ${HDAL_ROOT_DIR}/lib )

set( HDAL_LIB hdl.lib )


#######################################
# Include FalconUnityPlugin code
#######################################

set( SRC FalconUnityPlugin.cpp
		 Falcon.h Falcon.cpp
		 ForceContainer.h )

add_library( FalconUnityPlugin SHARED ${SRC} )
target_link_libraries( FalconUnityPlugin ${HDAL_LIB} )


#######################################
# Include Test code
#######################################

ADD_SUBDIRECTORY( Test )