cc_binary(
    name = "bin",
    srcs = [
        "DirectionVisitable.h",
        "DirectionVisitor.h",
        "Scene.cpp",
        "Scene.h",
        "ScreenModel.cpp",
        "ScreenModel.h",
        "ScriptParser.cpp",
        "ScriptParser.h",
        "directions/ClickRegion.cpp",
        "directions/ClickRegion.h",
        "directions/PressKey.cpp",
        "directions/PressKey.h",
        "operations/PerformDirections.cpp",
        "operations/PerformDirections.h",
        "operations/PrintDirections.cpp",
        "operations/PrintDirections.h",
        "screenplay.cc",
    ],
    linkopts = [
        "-lpthread",
        "-lopencv_core",
        "-lopencv_imgproc",
        "-lopencv_imgcodecs",
        "-lSegFault",  # stacktraces
        #"-lyaml-cpp", bazel rules seem to expose this one correctly?
    ],
    deps = [
        "//lib:libjpeg-turbo",
        "//lib:libvnc",
        "//lib:opencv",
        "@cc_yaml//:yaml-cpp",
    ],
)

cc_binary(
    name = "test-turbojpeg",
    srcs = [
        "test_turbojpeg.cc",
    ],
    linkopts = [
        "-lpthread",
    ],
    deps = [
        "//lib:libjpeg-turbo",
    ],
)
