depends_sources = [
  'context.cc',
  'language.cc',
  'language/assembler.cc',
  'language/pascal.cc',
  'main.cc',
]

depends_deps = [
  libexplain_dep,
  boost_dep,
]

depends_exe = executable(
  'ucsdpsys_depends',
  sources : depends_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : depends_deps,
  link_with : lib_lib,
  install : true,
)
