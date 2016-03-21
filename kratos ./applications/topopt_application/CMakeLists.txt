set(CMAKE_INCLUDE_CURRENT_DIR ON)

message("**** configuring KratosTopOptApplication ****")

include_directories( ${CMAKE_SOURCE_DIR}/kratos )

## generate variables with the sources
set( KRATOS_TOPOPT_APPLICATION_SOURCES     
	${CMAKE_CURRENT_SOURCE_DIR}//topopt_application.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/custom_python/topopt_python_application.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/custom_python/add_custom_utilities_to_python.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/custom_python/add_custom_constitutive_laws_to_python.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/custom_python/add_custom_strategies_to_python.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/custom_elements/topopt_small_displacement_element.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/custom_elements/topopt_small_displacement_element.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/custom_constitutive/hyperelastic_3D_law.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/custom_constitutive/linear_elastic_3D_law.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/custom_conditions/force_load_condition.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/custom_conditions/point_load_3D_condition.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/custom_conditions/line_load_3D_condition.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/custom_conditions/surface_load_3D_condition.cpp
)

###############################################################
## define library Kratos which defines the basic python interface
add_library(KratosTopOptApplication SHARED ${KRATOS_TOPOPT_APPLICATION_SOURCES})
target_link_libraries(KratosTopOptApplication KratosCore )
set_target_properties(KratosTopOptApplication PROPERTIES PREFIX "")
install(TARGETS KratosTopOptApplication DESTINATION libs )


# get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
# message("TestApplication subdir inc_dirs = ${inc_dirs}")

# changing the .dll suffix to .pyd 
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set_target_properties(KratosTopOptApplication PROPERTIES SUFFIX .pyd)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if(${INSTALL_PYTHON_FILES} MATCHES ON)
  get_filename_component (CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python_scripts DESTINATION applications/${CURRENT_DIR_NAME}  FILES_MATCHING PATTERN "*.py"  PATTERN ".svn" EXCLUDE)
endif(${INSTALL_PYTHON_FILES} MATCHES ON)

# get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
# message("TestApplication subdir inc_dirs = ${inc_dirs}")

# Add to the KratosMultiphisics Python module
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/TopOptApplication.py" DESTINATION KratosMultiphysics )
