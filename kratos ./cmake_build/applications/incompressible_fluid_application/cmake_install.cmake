# Install script for directory: /home/daniel/software/kratos/applications/incompressible_fluid_application

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/KratosIncompressibleFluidApplication.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/KratosIncompressibleFluidApplication.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/KratosIncompressibleFluidApplication.so"
         RPATH "/home/youruser/kratos/libs:/home/daniel/software/boost_1_57_0/stage/lib:/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu/libpython3.4m.so:/usr/lib/libblas/libblas.so.3.0:/usr/lib/lapack/liblapack.so.3.0:/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu")
  ENDIF()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/libs" TYPE SHARED_LIBRARY FILES "/home/daniel/software/kratos/cmake_build/applications/incompressible_fluid_application/KratosIncompressibleFluidApplication.so")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/KratosIncompressibleFluidApplication.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/KratosIncompressibleFluidApplication.so")
    FILE(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/KratosIncompressibleFluidApplication.so"
         OLD_RPATH "/home/daniel/software/boost_1_57_0/stage/lib:/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu/libpython3.4m.so:/usr/lib/libblas/libblas.so.3.0:/usr/lib/lapack/liblapack.so.3.0:/home/daniel/software/kratos/cmake_build/kratos:/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu:/home/daniel/software/kratos/cmake_build/external_libraries/zlib:"
         NEW_RPATH "/home/youruser/kratos/libs:/home/daniel/software/boost_1_57_0/stage/lib:/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu/libpython3.4m.so:/usr/lib/libblas/libblas.so.3.0:/usr/lib/lapack/liblapack.so.3.0:/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/libs/KratosIncompressibleFluidApplication.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/KratosMultiphysics" TYPE FILE FILES "/home/daniel/software/kratos/applications/incompressible_fluid_application/IncompressibleFluidApplication.py")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

