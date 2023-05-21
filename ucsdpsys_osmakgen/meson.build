osmakgen_sources = [
  'collect.cc',
  'main.cc',
  'target.cc',
  'target/build.cc',
  'target/build/cook.cc',
  'target/build/makefile.cc',
  'target/debian.cc',
  'target/factory.cc',
]

osmakgen_deps = [
  libexplain_dep,
  boost_dep,
]

osmakgen_exe = executable(
  'ucsdpsys_osmakgen',
  sources : osmakgen_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : osmakgen_deps,
  link_with : lib_lib,
  install : true,
)
