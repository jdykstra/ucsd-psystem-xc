project('ucsd-psystem-xc', 'cpp')

fs = import('fs')

cpp = meson.get_compiler('cpp')
sh = find_program('sh', '/bin/sh')
bison = find_program('bison')

bison_gen = generator(
  bison,
  output : ['@BASENAME@.yacc.cc', '@BASENAME@.yacc.h'],
  arguments : [
    '@INPUT@', '-Wno-yacc', '--defines=@OUTPUT1@', '--output=@OUTPUT0@'
  ],
)

libexplain_dep = dependency('libexplain')
boost_dep = dependency('boost')
libpng_dep = dependency('libpng')
libpng_headers_dep = libpng_dep.partial_dependency(includes : true)

root_inc = include_directories('.')

subdir('lib')

subdir('ucsdpsys')
subdir('ucsdpsys_assemble')
subdir('ucsdpsys_charset')
subdir('ucsdpsys_compile')
subdir('ucsdpsys_depends')
subdir('ucsdpsys_disassemble')
subdir('ucsdpsys_downcase')
subdir('ucsdpsys_errors')
subdir('ucsdpsys_foto')
subdir('ucsdpsys_libmap')
subdir('ucsdpsys_librarian')
subdir('ucsdpsys_link')
subdir('ucsdpsys_littoral')
subdir('ucsdpsys_opcodes')
subdir('ucsdpsys_osmakgen')
subdir('ucsdpsys_pretty')
subdir('ucsdpsys_setup')

subdir('man')

subdir('test_cardinal')
subdir('test_long_integer')

subdir('test')
