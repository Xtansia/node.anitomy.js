{
  'targets': [
    {
      'target_name': 'anitomyjs',
      'sources': [
        'src/anitomyjs.cpp',
        'src/util.cpp',
        'src/objects/anitomy_obj.cpp',
        'src/objects/elements_obj.cpp',
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