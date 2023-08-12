cribbitlib = library('cribbit', [
        'cribbit.c',
        'ribbit/ribbit_client.c',
        'ribbit/ribbit_shared.c',
        'ribbit/ribbit_v1.c',
        'ribbit/ribbit_v2.c',
        'tact/tact_pipe_file.c'
    ],
    include_directories: incdir,
    dependencies : [threaddep],
    install: true,
    link_args: ldflags,
    gnu_symbol_visibility: 'hidden',
    c_args: cflags + ['-DCRIBBIT_EXPORTING'])

cribbitdep = declare_dependency(link_with: cribbitlib, include_directories: incdir)
