opcodes_sources = [
  'arch.cc',
  'arch/adaptable.cc',
  'arch/adaptable/z80.cc',
  'arch/factory.cc',
  'arch/pcode.cc',
  'bitflip.cc',
  'decode.cc',
  'encode.cc',
  'main.cc',
]

opcodes_deps = [
  libexplain_dep,
  boost_dep,
]

opcodes_exe = executable(
  'ucsdpsys_opcodes',
  sources : opcodes_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : opcodes_deps,
  link_with : lib_lib,
  install : true,
)
