cmake_minimum_required(VERSION 3.2)
project(JcPimMultiBandV2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SHARE_LIB_SOURCE_FILES
    JcPimMultiBandV2/3rdParty/CppSQLite3.cpp
    JcPimMultiBandV2/3rdParty/CppSQLite3.h
    JcPimMultiBandV2/3rdParty/NrpControl2.h
    JcPimMultiBandV2/3rdParty/rsnrpz.h
    JcPimMultiBandV2/3rdParty/sqlite3.c
    JcPimMultiBandV2/3rdParty/sqlite3.h
    JcPimMultiBandV2/3rdParty/visa.h
    JcPimMultiBandV2/3rdParty/visatype.h
    JcPimMultiBandV2/MyUtil/JcCommonAPI.cpp
    JcPimMultiBandV2/MyUtil/JcCommonAPI.h
    JcPimMultiBandV2/MyUtil/JcException.cpp
    JcPimMultiBandV2/MyUtil/JcException.h
    JcPimMultiBandV2/RohdeSchwarz/ClsAnaRsFspSerial.cpp
    JcPimMultiBandV2/RohdeSchwarz/ClsAnaRsFspSerial.h
    JcPimMultiBandV2/RohdeSchwarz/ClsSigRsSMxSerial.cpp
    JcPimMultiBandV2/RohdeSchwarz/ClsSigRsSMxSerial.h
    JcPimMultiBandV2/Switch/com_io_ctl.cpp
    JcPimMultiBandV2/Switch/com_io_ctl.h
    JcPimMultiBandV2/Switch/implementsetting.cpp
    JcPimMultiBandV2/Switch/implementsetting.h
    JcPimMultiBandV2/Switch/switch_info.h
    JcPimMultiBandV2/ClsAnaAgN9020A.hpp
    JcPimMultiBandV2/ClsInstrAgilent.hpp
    JcPimMultiBandV2/ClsInstrRohdeSchwarz.h
    JcPimMultiBandV2/ClsJcSwitch.cpp
    JcPimMultiBandV2/ClsJcSwitch.h
    JcPimMultiBandV2/ClsSenAgU2000A.hpp
    JcPimMultiBandV2/ClsSenRsNrpz.hpp
    JcPimMultiBandV2/ClsSenRsNrt.hpp
    JcPimMultiBandV2/ClsSigAgN5181A.hpp
    JcPimMultiBandV2/ClsVnaAgE5062A.hpp
    JcPimMultiBandV2/IfAnalyzer.hpp
    JcPimMultiBandV2/IfInstrument.hpp
    JcPimMultiBandV2/IfSensor.hpp
    JcPimMultiBandV2/IfSignalSource.hpp
    JcPimMultiBandV2/IfSwitch.hpp
    JcPimMultiBandV2/IfVna.hpp
    JcPimMultiBandV2/Interface.h
    JcPimMultiBandV2/JcApi.cpp
    JcPimMultiBandV2/JcApi.h
    JcPimMultiBandV2/JcOffsetDB.hpp
    JcPimMultiBandV2/JcPimObject.h
    #JcPimMultiBandV2/resource.h
    JcPimMultiBandV2/stdafx.cpp
    JcPimMultiBandV2/stdafx.h
    JcPimMultiBandV2/targetver.h)

link_directories(${PROJECT_SOURCE_DIR}/JcPimMultiBandV2/3rdParty)

add_library(JcPimMultiBandV2 SHARED ${SHARE_LIB_SOURCE_FILES})
target_link_libraries(JcPimMultiBandV2 "ws2_32.lib")
target_link_libraries(JcPimMultiBandV2 "mswsock.lib")
target_link_libraries(JcPimMultiBandV2 "Version.lib")
target_link_libraries(JcPimMultiBandV2 "visa32.lib")
target_link_libraries(JcPimMultiBandV2 "NrpControl2.lib")
target_link_libraries(JcPimMultiBandV2 "rsnrpz_32.lib")

set(DEMO_SOURCE_FILES
    TestDemo/stdafx.cpp
    TestDemo/stdafx.h
    TestDemo/targetver.h
    TestDemo/TestDemo.cpp)

add_executable(TestDemo ${DEMO_SOURCE_FILES})