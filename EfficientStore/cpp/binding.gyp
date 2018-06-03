{
  "targets": [
    {
      "target_name": "engine_addon",
      "sources": [ "engine.cpp" ],
      "cflags": ["-Wall", "-std=c++11"],
      "build_v8_with_gn": "true"
    }
  ]
}