disassemble_sources = [
  'main.cc',
]

disassemble_deps = [
  libexplain_dep,
  boost_dep,
]

disassemble_exe = executable(
  'ucsdpsys_disassemble',
  sources : disassemble_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : disassemble_deps,
  link_with : lib_lib,
  install : true,
)
