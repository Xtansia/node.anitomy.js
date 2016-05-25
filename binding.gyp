{
  "targets": [
    {
      "target_name": "anitomy",
      "sources": [
        "anitomy_wrap.cc"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}