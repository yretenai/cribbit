project('cribbit', 'c',
    license : 'BSD0',
    default_options : [
        'prefix=/usr',
        'c_std=c17',
        'warning_level=3'
    ])

threaddep = dependency('threads', required: true)

incdir = include_directories('include/')

cflags = []

if meson.get_compiler('c').get_id() == 'gcc'
    if meson.get_compiler('c').compiles('void test(main) { }', name: 'can use -fanalyzer', args: '-fanalyzer')
      cflags += ['-fanalyzer']
    endif
endif

subdir('src')
subdir('cli')

install_subdir('include/',
    install_dir: 'include/',
    strip_directory: true)