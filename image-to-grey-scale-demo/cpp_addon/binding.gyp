{
  "includes": ["config.gypi"],

  "targets": [
    {
      "target_name": "image_greyscale",
      "sources": [ "greyscale.cc" ],
      "cflags": [ "-frtti", "-fexceptions", "-std=c++17" ],
      "cflags_cc": [ "-frtti", "-fexceptions", "-std=c++17" ],
      
      "conditions": [
        ["OS=='mac'", {
          "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "/usr/local/include/opencv4"
          ],
          "libraries": [
            "-L/usr/local/lib",
            "-lopencv_core",
            "-lopencv_imgproc",
            "-lopencv_highgui"
          ],
          "link_settings": {
            "ldflags": [
              "-Wl,-rpath,/usr/local/lib"
            ]
          },
          "xcode_settings": {
            "GCC_ENABLE_CPP_RTTI": "YES",
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "CLANG_CXX_LANGUAGE_STANDARD": "c++17",
            "CLANG_CXX_LIBRARY": "libc++"
          }
        }],
        ["OS=='win'", {
          "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "<(opencv_include_dir)"
          ],
          'conditions': [
            ['default_configuration=="Debug"', {
              "libraries": [
                "<(opencv_lib_dir_debug)/opencv_core490d.lib",
                "<(opencv_lib_dir_debug)/opencv_imgproc490d.lib",
                "<(opencv_lib_dir_debug)/opencv_highgui490d.lib",
                "<(opencv_lib_dir_debug)/opencv_imgcodecs490d.lib",
                "-L<(opencv_lib_dir_debug)"
              ]
            }],
            ['default_configuration=="Release"', {
              "libraries": [
                "<(opencv_lib_dir)/opencv_core490.lib",
                "<(opencv_lib_dir)/opencv_imgproc490.lib",
                "<(opencv_lib_dir)/opencv_highgui490.lib",
                "<(opencv_lib_dir)/opencv_imgcodecs490.lib",
                "-L<(opencv_lib_dir)"
              ]
            }]
          ]
        }]
      ]
    }
  ]
}
