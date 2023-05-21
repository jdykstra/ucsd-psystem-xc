link_sources = [
  'link_info_entry.cc',
  'main.cc',
  'segrec.cc',
  'workrec.cc',
]

link_deps = [
  libexplain_dep,
  boost_dep,
]

link_exe = executable(
  'ucsdpsys_link',
  sources : link_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : link_deps,
  link_with : lib_lib,
  install : true,
)
