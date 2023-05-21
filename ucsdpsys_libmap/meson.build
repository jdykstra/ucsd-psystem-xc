libmap_sources = [
  'main.cc',
]

libmap_deps = [
  libexplain_dep,
  boost_dep,
]

libmap_exe = executable(
  'ucsdpsys_libmap',
  sources : libmap_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : libmap_deps,
  link_with : lib_lib,
  install : true,
)
