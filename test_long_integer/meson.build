test_long_integer_grammar_ch = bison_gen.process(
  'grammar.y', preserve_path_from : meson.source_root(),
)

test_long_integer_sources = [
  test_long_integer_grammar_ch,
  'lex.cc',
  'main.cc',
]

test_long_integer_deps = [
  libexplain_dep,
  boost_dep,
]

test_long_integer_exe = executable(
  'test_long_integer',
  sources : test_long_integer_sources,
  include_directories : root_inc,
  implicit_include_directories : false,
  dependencies : test_long_integer_deps,
  link_with : lib_lib,
  install : false,
)
