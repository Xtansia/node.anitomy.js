{
  "targets": [
    {
      "target_name": "anitomyjs",
      "sources": [
        "src/anitomyjs.cpp",
        "src/util.cpp",
        "deps/anitomy/anitomy/anitomy.cpp",
        "deps/anitomy/anitomy/element.cpp",
        "deps/anitomy/anitomy/keyword.cpp",
        "deps/anitomy/anitomy/parser.cpp",
        "deps/anitomy/anitomy/parser_helper.cpp",
        "deps/anitomy/anitomy/parser_number.cpp",
        "deps/anitomy/anitomy/string.cpp",
        "deps/anitomy/anitomy/token.cpp",
        "deps/anitomy/anitomy/tokenizer.cpp"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "deps/include"
      ],
      "ccflags": [
        "-Wall",
        "-Wextra",
        "-Wno-unused-parameter",
        "-fno-exceptions",
        "-fno-rtti"
      ]
    }
  ]
}