# set(SRC_CPP_FILES

# )

file(GLOB ABSOLUTE_SRC_CPP_FILES 
  src/*.c*
)

foreach(FILE ${ABSOLUTE_SRC_CPP_FILES})
  file(RELATIVE_PATH FILE ${CMAKE_CURRENT_SOURCE_DIR}/src ${FILE}) 
  list(APPEND SRC_CPP_FILES ${FILE})
endforeach()

# set(INTERNAL_CPP_FILES
#   org_mitk_dental_Activator.cpp
#   MitkView.cpp
# )

file(GLOB ABSOLUTE_INTERNAL_CPP_FILES 
  src/internal/*.c*
)

foreach(FILE ${ABSOLUTE_INTERNAL_CPP_FILES})
  file(RELATIVE_PATH FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/internal ${FILE}) 
  list(APPEND INTERNAL_CPP_FILES ${FILE})
endforeach()

# set(UI_FILES
#   src/internal/MitkViewControls.ui
# )

file(GLOB ABSOLUTE_UI_FILES 
  src/internal/*.ui
)

foreach(FILE ${ABSOLUTE_UI_FILES})
  file(RELATIVE_PATH FILE ${CMAKE_CURRENT_SOURCE_DIR} ${FILE}) 
  list(APPEND UI_FILES ${FILE})
endforeach()

# set(MOC_H_FILES
#   src/internal/org_mitk_dental_Activator.h
#   src/internal/MitkView.h
# )

file(GLOB ABSOLUTE_MOC_H_FILES
  src/internal/*.h*
)

foreach(FILE ${ABSOLUTE_MOC_H_FILES})
  file(RELATIVE_PATH FILE ${CMAKE_CURRENT_SOURCE_DIR} ${FILE})
  list(APPEND MOC_H_FILES ${FILE})
endforeach()

# list of resource files which can be used by the plug-in
# system without loading the plug-ins shared library,
# for example the icon used in the menu and tabs for the
# plug-in views in the workbench
set(CACHED_RESOURCE_FILES
  resources/icon.xpm
  plugin.xml
)

# list of Qt .qrc files which contain additional resources
# specific to this plugin
set(QRC_FILES

)

set(CPP_FILES )

foreach(file ${SRC_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})
