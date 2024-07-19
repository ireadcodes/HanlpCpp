project(HanlpLibrary)

set(CMAKE_CXX_STANDARD 20)

set(HANLP_SRC
    mining/word/TfIdfCounter.cpp
    mining/word/TfIdfCounter.h
    mining/word/TfIdf.cpp
    mining/word/TfIdf.h
)

add_library(${PROJECT_NAME} ${HANLP_SRC})
