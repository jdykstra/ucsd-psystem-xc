foto_sources = [
  'main.cc',
]

foto_deps = [
  libexplain_dep,
  boost_dep,
  libpng_dep,
]

foto_exe = executable(
  'ucsdpsys_foto',
  sources : foto_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : foto_deps,
  link_with : lib_lib,
  install : true,
)
