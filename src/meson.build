cribbitlib = shared_library('cribbit', [
        'ribbit/ribbit_client.c',
        'ribbit/ribbit_v1.c',
        'ribbit/ribbit_v2.c',
        'tact/tact_pipe_file.c'
    ],
    include_directories: incdir,
    dependencies : [threaddep],
    install: true,
    c_args: cflags + ['-DCRIBBIT_EXPORTING'])
