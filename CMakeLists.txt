cmake_minimum_required(VERSION 3.15)
project(coin_lib)

set(CMAKE_CXX_STANDARD 11)

link_directories (${PROJECT_SOURCE_DIR}/osi/lib)

add_executable(${PROJECT_NAME} main.cpp ${SRC_LIST})

include_directories(${PROJECT_SOURCE_DIR}/osi/include)

target_link_libraries (${PROJECT_NAME}  -lcoinglpk -lClp -lOsiClp  -lCgl -lOsi -lSym -lOsiSym -lCoinUtils)