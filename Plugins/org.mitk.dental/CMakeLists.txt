project(org_mitk_dental)

mitk_create_plugin(
  EXPORT_DIRECTIVE DENTAL_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkQtWidgetsExt MitkSceneSerialization
)

target_link_libraries(${PROJECT_NAME} PRIVATE view)