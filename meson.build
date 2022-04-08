project ('com.github.rafaelrpq.gamelog', 'c', version: '0.1')

resources = import ('gnome').compile_resources (
    'assets',
    'resources.xml',
    source_dir: 'assets'
)

executable (
    meson.project_name (),
    resources,
    'src/main.c',
    'src/data.c',
    'src/interface.c',
    dependencies: [
        dependency ('gtk+-3.0'),
    ],
    install : false
)
