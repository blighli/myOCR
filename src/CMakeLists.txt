#设定项目名称
SET(THIS_PROJECT_NAME myOCR)

#设定QT4环境,参考FindQt4.cmake
FIND_PACKAGE(Qt4 REQUIRED)

#引入需要的Qt库，必须放在QT_USE_FILE之前
SET(QT_USE_QTMAIN TRUE)
#SET(QT_USE_QTXML TRUE)
#SET(QT_USE_QTOPENGL TRUE)

#引入Qt必要的设定
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

#设定源文件
SET(sourceFiles main.cpp)
SET(headerFiles) #包含Q_OBJECT的头文件
SET(uiFiles)
SET(resourceFiles)

#对Qt源文件进行处理
QT4_WRAP_CPP(mocCpps ${headerFiles} )
QT4_WRAP_UI(uicHeaders ${uiFiles})
QT4_ADD_RESOURCES(qrcCpps ${resourceFiles})

#对生成文件分组
SOURCE_GROUP("Resources" FILES  
  ${uiFiles}  
)
SOURCE_GROUP("Generated" FILES  
  ${uicHeaders}
  ${mocCpps}
  ${qrcCpps}
)

#目标文件和链接文件
ADD_EXECUTABLE(${THIS_PROJECT_NAME} ${sourceFiles} ${uicHeaders} ${mocCpps} ${qrcCpps})
TARGET_LINK_LIBRARIES(${THIS_PROJECT_NAME} ${QT_LIBRARIES})

#避免启动时出现命令行窗口
IF(WIN32)
    SET_TARGET_PROPERTIES(${THIS_PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
ENDIF()
