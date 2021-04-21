project(
    'hashed-table-demo',
    'c'
)

# hashed-table library
subdir('src')

src_files = 'demo.c'
incl_dir = 'src'

executable(
    'hashed-table-demo',
    sources : src_files,
    include_directories : incl_dir,
    link_with : hashed_table_lib
)