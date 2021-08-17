project('sudoku', 'c',
        default_options: [ 'buildtype=optimized', 'c_std=c11', 'warning_level=3' ]
)

sources_list = []

inc = [include_directories('include')]

#Descend into src subdirectory
subdir('src')

sudoku_bin = executable('sudoku',
    sources_list,
    include_directories: inc,
)