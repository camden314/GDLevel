CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GDLevel)

# Include directories
INCLUDE_DIRECTORIES(.)

#Source files
set (GDCRYPTO_SRC
    ../Source/level.cpp
)

#Define libraries
ADD_LIBRARY(gdlevel SHARED ${GDCRYPTO_SRC})
ADD_LIBRARY(gdlevel-static STATIC ${GDCRYPTO_SRC})

TARGET_LINK_LIBRARIES(GDLevel GDCrypto)