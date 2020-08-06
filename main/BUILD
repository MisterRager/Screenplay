cc_binary(
    name = "bin",
    srcs = [
        "screenplay.cc",
    ],
    linkopts = [
        "-lpthread",
        "-lSegFault",  # stacktraces
        #"-lyaml-cpp", bazel rules seem to expose this one correctly?
    ],
    deps = [
        "//lib:libvnc",
        "//lib:screenplay",
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
