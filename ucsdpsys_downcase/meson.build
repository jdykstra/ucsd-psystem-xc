downcase_sources = [
  'main.cc',
]

downcase_deps = [
  libexplain_dep,
  boost_dep,
]

downcase_exe = executable(
  'ucsdpsys_downcase',
  sources : downcase_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : downcase_deps,
  link_with : lib_lib,
  install : true,
)
