ucsdpsys_sources = [
  'main.cc',
  'volume.cc',
  'volume/directory.cc',
  'volume/file.cc',
  'volume/list.cc',
]

ucsdpsys_deps = [
  libexplain_dep,
  boost_dep,
]

ucsdpsys_exe = executable(
  'ucsdpsys',
  sources : ucsdpsys_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : ucsdpsys_deps,
  link_with : lib_lib,
  install : true,
)
