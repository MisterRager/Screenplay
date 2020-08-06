load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")
load("@rules_foreign_cc//tools/build_defs:make.bzl", "make")
load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

cmake_external(
    name = "libz",
    lib_source = "@cc_zlib//:all",
)

cmake_external(
    name = "liblzo2",
    lib_source = "@cc_lzo//:all",
)

cmake_external(
    name = "libjpeg-turbo",
    cache_entries = {
        "WITH_JPEG8": "1",
    },
    lib_source = "@cc_libjpeg-turbo//:all",
    # only for some linuxes... ugh
    out_lib_dir = "lib64",
    static_libraries = [
        "libturbojpeg.a",
        "libjpeg.a",
    ],
    visibility = ["//visibility:public"],
)

cmake_external(
    name = "libpng",
    cache_entries = {
        "ZLIB_LIBRARY": "$EXT_BUILD_DEPS/libz/lib/libz.a",
    },
    lib_source = "@cc_libpng//:all",
    static_libraries = [
        "libpng.a",
    ],
    deps = [
        ":libz",
    ],
)

configure_make(
    name = "libjpeg",
    configure_env_vars = {
        "AR": "ar",
    },
    configure_options = [
        "--enable-static",
        "--disable-shared",
    ],
    lib_source = "@cc_libjpeg//:all",
)

configure_make(
    name = "libgcrypt",
    autogen = True,
    configure_env_vars = {
        "AR": "/usr/bin/ar",
    },
    configure_options = [
        "--enable-static",
        "--disable-shared",
    ],
    lib_source = "@cc_libgcrypt//:all",
)

cmake_external(
    name = "libvnc",
    cache_entries = {
        "ZLIB_LIBRARY": "$EXT_BUILD_DEPS/libz/lib/libz.a",
        "JPEG_LIBRARY": "$EXT_BUILD_DEPS/libjpeg/lib/libjpeg.a",
        "JPEG_INCLUDE_DIR": "$EXT_BUILD_DEPS/libjpeg/include",
        "PNG_LIBRARY": "$EXT_BUILD_DEPS/libpng/lib/libpng.a",
        "LZO_LIBRARIES": "$EXT_BUILD_DEPS/liblzo2/lib/liblzo2.a",
        "BUILD_SHARED_LIBS": "OFF",
        "WITH_GCRYPT": "OFF",
        "WITH_GNUTLS": "OFF",
        "WITH_OPENSSL": "OFF",
        "WITH_SASL": "OFF",
    },
    lib_source = "@cc_libvnc//:all",
    static_libraries = [
        "libvncserver.a",
        "libvncclient.a",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":libjpeg",
        ":liblzo2",
        ":libpng",
        ":libz",
    ],
)

cmake_external(
    name = "libopenjp2",
    lib_source = "@cc_openjpeg//:all",
)

cmake_external(
    name = "opencv",
    cache_entries = {
        "ZLIB_LIBRARY": "$EXT_BUILD_DEPS/libz/lib/libz.a",
        "JPEG_LIBRARY": "$EXT_BUILD_DEPS/libjpeg/lib/libjpeg.a",
        "JPEG_INCLUDE_DIR": "$EXT_BUILD_DEPS/libjpeg/include",
        "PNG_LIBRARY": "$EXT_BUILD_DEPS/libpng/lib/libpng.a",
        "BUILD_SHARED_LIBS": "OFF",
        "BUILD_LIST": "core,imgproc,imgcodecs",
        "WITH_OPENEXR": "OFF",
        "WITH_TIFF": "OFF",
        "WITH_WEBP": "OFF",
        "WITH_ITT": "OFF",
        "WITH_CUBLAS": "OFF",
        "WITH_OPENCL": "OFF",
        "WITH_LAPACK": "OFF",
        "WITH_JASPER": "OFF",
    },
    lib_source = "@cc_opencv//:all",
    linkopts = [
        "-lpthread",
    ],
    out_include_dir = "include/opencv4",
    static_libraries = [
        #"libopencv_calib3d.a",
        "libopencv_core.a",
        #"libopencv_dnn.a",
        #"libopencv_features2d.a",
        #"libopencv_flann.a",
        #"libopencv_highgui.a",
        "libopencv_imgcodecs.a",
        "libopencv_imgproc.a",
        #"libopencv_ml.a",
        #"libopencv_objdetect.a",
        #"libopencv_photo.a",
        #"libopencv_stitching.a",
        #"libopencv_video.a",
        #"libopencv_videoio.a",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":libjpeg",
        ":libopenjp2",
        ":libpng",
        ":libz",
    ],
)
