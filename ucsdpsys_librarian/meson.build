librarian_sources = [
  'main.cc',
]

librarian_deps = [
  libexplain_dep,
  boost_dep,
]

librarian_exe = executable(
  'ucsdpsys_librarian',
  sources : librarian_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : librarian_deps,
  link_with : lib_lib,
  install : true,
)
