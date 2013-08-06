#设定项目名称
SET(THIS_PROJECT_NAME myOCR)

#设定QT4环境 AUTOMOC???
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})  # include UseQt4.cmake
MESSAGE(STATUS "QT_USE_FILE=" ${QT_USE_FILE})
   
# support for out-of-source build
INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR} #this is where ui_SimpleViewUI.h is generated
  ${CMAKE_CURRENT_SOURCE_DIR}
)
MESSAGE(STATUS "CMAKE_CURRENT_BINARY_DIR=" ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR=" ${CMAKE_CURRENT_SOURCE_DIR})

# Set your files and resources here  
  
# 1. Set path for each kind of files  
SET(SourceFilesPath ${PROJECT_SOURCE_DIR}/src)
MESSAGE(STATUS "SourceFilesPath=" ${SourceFilesPath})

SET(UIFilesPath ${PROJECT_SOURCE_DIR}/ui)
MESSAGE(STATUS "UIFilesPath=" ${UIFilesPath})

SET(HeadersFilesPath ${PROJECT_SOURCE_DIR}/inc)
MESSAGE(STATUS "HeadersFilesPath=" ${HeadersFilesPath})
  
# 2. Add items into current project  
SET(SourceFiles main.cpp) 
SET(UIFiles)
SET(HeadersFiles) 
  
# 3. Concatenate items and their paths  
FOREACH(SrcFile ${SourceFiles})  
    SET(SimpleViewSrcs ${SimpleViewSrcs} ${SourceFilesPath}/${SrcFile})  
ENDFOREACH(SrcFile)  
MESSAGE(STATUS "Files: " ${SimpleViewSrcs})  
  
FOREACH(UIFile ${UIFiles})  
    SET(SimpleViewUI ${SimpleViewUI} ${UIFilesPath}/${UIFile})  
ENDFOREACH(UIFile)  
MESSAGE(STATUS "UIFlie: " ${SimpleViewUI})  
  
FOREACH(HeaderFile ${HeadersFiles})  
    SET(SimpleViewHeaders ${SimpleViewHeaders} ${HeadersFilesPath}/${HeaderFile})  
ENDFOREACH(HeaderFile)  
MESSAGE(STATUS "Headers: " ${SimpleViewHeaders})  
  
# Wrap items to pass them to qmake  
QT4_WRAP_UI(UISrcs ${SimpleViewUI})  
QT4_WRAP_CPP(MOCSrcs ${SimpleViewHeaders} )  
   
SOURCE_GROUP("Resources" FILES  
  ${SimpleViewUI}  
)  
   
SOURCE_GROUP("Generated" FILES  
  ${UISrcs}  
  ${MOCSrcs}  
)  

ADD_EXECUTABLE(${THIS_PROJECT_NAME} ${SimpleViewSrcs} ${UISrcs} ${MOCSrcs})
TARGET_LINK_LIBRARIES(${THIS_PROJECT_NAME} ${QT_QTMAIN_LIBRARY} ${QT_LIBRARIES})

#避免启动时出现命令行窗口
IF(WIN32)
    SET_TARGET_PROPERTIES(${THIS_PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
ENDIF()
