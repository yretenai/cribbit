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
ldflags = []

if meson.get_compiler('c').get_id() == 'gcc'
    if meson.get_compiler('c').compiles('void test(main) { }', name: 'can use -fanalyzer', args: '-fanalyzer')
      cflags += ['-fanalyzer']
    endif
endif

if host_machine.system() == 'windows'
    ldflags += ['-lWs2_32']
endif

subdir('src')
subdir('cli')

install_subdir('include/',
    install_dir: 'include/',
    strip_directory: true)