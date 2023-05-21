errors_sources = [
  'arch.cc',
  'arch/adaptable.cc',
  'arch/adaptable/6502.cc',
  'arch/adaptable/6800.cc',
  'arch/adaptable/8080.cc',
  'arch/adaptable/ga440.cc',
  'arch/adaptable/pdp11.cc',
  'arch/adaptable/ti9900.cc',
  'arch/adaptable/z80.cc',
  'arch/factory.cc',
  'main.cc',
]

errors_deps = [
  libexplain_dep,
  boost_dep,
]

errors_exe = executable(
  'ucsdpsys_errors',
  sources : errors_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : errors_deps,
  link_with : lib_lib,
  install : true,
)
