cmake_minimum_required(VERSION 3.5.1)

project(Mission_Plaza_Rev10)

set(CMAKE_CXX_STANDARD 17)


find_package( OpenCV REQUIRED )

include_directories(
		include/
        ${OpenCV_INCLUDE_DIRS}
)




find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")

    message([main] " OpenMP_C_FLAGS=${OpenMP_C_FLAGS}") # -fopenmp
    message([main] " OpenMP_CXX_FLAGS}=${OpenMP_CXX_FLAGS}") # -fopenmp
    message([main] " OpenMP_EXE_LINKER_FLAGS=${OpenMP_EXE_LINKER_FLAGS}") # ***

    # no use for xxx_INCLUDE_DIRS and xxx_libraries for OpenMP
    message([main] " OpenMP_INCLUDE_DIRS=${OpenMP_INCLUDE_DIRS}") # ***
    message([main] " OpenMP_LIBRARIES=${OpenMP_LIBRARIES}") # ***

    

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

   set(CMAKE_CXX_FLAGS_RELEASE "-o3 ")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()




ADD_CUSTOM_TARGET(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
  )

ADD_CUSTOM_TARGET(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Release"
  )


set(SOURCE_FILES 
	main.cpp 
	measure2.cpp 
	process_other.cpp
  process_filters.cpp	
  player_class.cpp
  USB_FTDI.cpp
  sculpture_class.cpp
  file_io_2.cpp
)
	


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries( ${PROJECT_NAME}   ${OpenCV_LIBS} /usr/local/lib/libftd2xx.so ) 

