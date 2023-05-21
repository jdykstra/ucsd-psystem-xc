setup_sources = [
  'main.cc',
]

setup_deps = [
  libexplain_dep,
  boost_dep,
]

setup_exe = executable(
  'ucsdpsys_setup',
  sources : setup_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : setup_deps,
  link_with : lib_lib,
  install : true,
)
