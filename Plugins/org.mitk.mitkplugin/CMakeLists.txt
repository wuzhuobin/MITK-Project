project(org_mitk_mitkplugin)

mitk_create_plugin(
  EXPORT_DIRECTIVE MITKPLUGIN_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkQtWidgetsExt
)
