project(org_mycompany_myplugin)

mitk_create_plugin(
  EXPORT_DIRECTIVE MYPLUGIN_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkQtWidgetsExt
)
