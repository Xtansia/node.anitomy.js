{
  'targets': [
    {
      'target_name': 'anitomyjs',
      'sources': [
        'src/anitomyjs.cpp',
        'src/util.cpp',
        'src/anitomy_object.cpp',
        'src/element_pair_object.cpp',
        'src/elements_object.cpp',
        'src/options_object.cpp',
        'deps/anitomy/anitomy/anitomy.cpp',
        'deps/anitomy/anitomy/element.cpp',
        'deps/anitomy/anitomy/keyword.cpp',
        'deps/anitomy/anitomy/parser.cpp',
        'deps/anitomy/anitomy/parser_helper.cpp',
        'deps/anitomy/anitomy/parser_number.cpp',
        'deps/anitomy/anitomy/string.cpp',
        'deps/anitomy/anitomy/token.cpp',
        'deps/anitomy/anitomy/tokenizer.cpp'
      ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")',
        'deps/anitomy'
      ],
      'conditions': [
        ['OS=="win"', {
          'ccflags': [
            '/Wall',
          ]
        }, {
          'ccflags': [
            '-Wall',
            '-Wextra',
            '-Wno-unused-parameter'
          ]
        }]
      ]
    }
  ]
}