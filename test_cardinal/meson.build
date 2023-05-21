test_cardinal_sources = [
  'main.cc',
]

test_cardinal_deps = [
  libexplain_dep,
  boost_dep,
]

test_cardinal_exe = executable(
  'test_cardinal',
  sources : test_cardinal_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : test_cardinal_deps,
  link_with : lib_lib,
  install : false,
)
