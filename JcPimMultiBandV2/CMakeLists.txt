set(SHARE_LIB_SOURCE_FILES
    3rdParty/CppSQLite3.cpp
    3rdParty/CppSQLite3.h
    3rdParty/NrpControl2.h
    3rdParty/rsnrpz.h
    3rdParty/sqlite3.c
    3rdParty/sqlite3.h
    3rdParty/visa.h
    3rdParty/visatype.h
    MyUtil/JcCommonAPI.cpp
    MyUtil/JcCommonAPI.h
    MyUtil/JcException.cpp
    MyUtil/JcException.h
    RohdeSchwarz/ClsAnaRsFspSerial.cpp
    RohdeSchwarz/ClsAnaRsFspSerial.h
    RohdeSchwarz/ClsSigRsSMxSerial.cpp
    RohdeSchwarz/ClsSigRsSMxSerial.h
    Switch/com_io_ctl.cpp
    Switch/com_io_ctl.h
    Switch/implementsetting.cpp
    Switch/implementsetting.h
    Switch/switch_info_poi.h
    Switch/switch_info_huawei.h
    Switch/JcMatrixSwitch.cpp
    Switch/JcMatrixSwitch.h
    ClsAnaAgN9020A.hpp
    ClsInstrAgilent.hpp
    ClsInstrRohdeSchwarz.h
    ClsJcSwitch.cpp
    ClsJcSwitch.h
    ClsSenAgU2000A.hpp
    ClsSenRsNrpz.hpp
    ClsSenRsNrt.hpp
    ClsSigAgN5181A.hpp
    ClsVnaAgE5062A.hpp
    IfAnalyzer.hpp
    IfInstrument.hpp
    IfSensor.hpp
    IfSignalSource.hpp
    IfSwitch.hpp
    IfVna.hpp
    Interface.h
    JcApi.cpp
    JcApi.h
    JcOffsetDB.cpp
    JcOffsetDB.h
    JcPimObject.h
    dllmain.cpp
    stdafx.cpp
    stdafx.h
    targetver.h)

add_library(JcPimMultiBandV2 SHARED ${SHARE_LIB_SOURCE_FILES})
target_link_libraries(JcPimMultiBandV2 "ws2_32.lib")
target_link_libraries(JcPimMultiBandV2 "mswsock.lib")
target_link_libraries(JcPimMultiBandV2 "Version.lib")
target_link_libraries(JcPimMultiBandV2 "${PROJECT_SOURCE_DIR}/JcPimMultiBandV2/3rdParty/visa32.lib")
target_link_libraries(JcPimMultiBandV2 "${PROJECT_SOURCE_DIR}/JcPimMultiBandV2/3rdParty/NrpControl2.lib")
target_link_libraries(JcPimMultiBandV2 "${PROJECT_SOURCE_DIR}/JcPimMultiBandV2/3rdParty/rsnrpz_32.lib")