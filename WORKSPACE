workspace(name = "software_eastshore_screenplay")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# Group the sources of the library so that CMake rule have access to it
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

# Rule repository
http_archive(
    name = "rules_foreign_cc",
    strip_prefix = "rules_foreign_cc-master",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

# Call this function from the WORKSPACE file to initialize rules_foreign_cc
#  dependencies and let neccesary code generation happen
#  (Code generation is needed to support different variants of the C++ Starlark API.).
#
#  Args:
#    native_tools_toolchains: pass the toolchains for toolchain types
#      '@rules_foreign_cc//tools/build_defs:cmake_toolchain' and
#      '@rules_foreign_cc//tools/build_defs:ninja_toolchain' with the needed platform constraints.
#      If you do not pass anything, registered default toolchains will be selected (see below).
#
#    register_default_tools: if True, the cmake and ninja toolchains, calling corresponding
#      preinstalled binaries by name (cmake, ninja) will be registered after
#      'native_tools_toolchains' without any platform constraints.
#      The default is True.
rules_foreign_cc_dependencies()

http_archive(
    name = "cc_libvnc",
    build_file_content = all_content,
    sha256 = "0ae5bb9175dc0a602fe85c1cf591ac47ee5247b87f2bf164c16b05f87cbfa81a",
    strip_prefix = "libvncserver-LibVNCServer-0.9.13",
    urls = [
        "https://github.com/LibVNC/libvncserver/archive/LibVNCServer-0.9.13.tar.gz",
    ],
)

http_archive(
    name = "cc_libjpeg-turbo",
    build_file_content = all_content,
    sha256 = "b3090cd37b5a8b3e4dbd30a1311b3989a894e5d3c668f14cbc6739d77c9402b7",
    strip_prefix = "libjpeg-turbo-2.0.5",
    urls = [
        "https://github.com/libjpeg-turbo/libjpeg-turbo/archive/2.0.5.tar.gz",
    ],
)

http_archive(
    name = "cc_libjpeg",
    build_file_content = all_content,
    sha256 = "99cb50e48a4556bc571dadd27931955ff458aae32f68c4d9c39d624693f69c32",
    strip_prefix = "jpeg-9d",
    urls = [
        "https://www.ijg.org/files/jpegsrc.v9d.tar.gz",
    ],
)

http_archive(
    name = "cc_libpng",
    build_file_content = all_content,
    sha256 = "6d59d6a154ccbb772ec11772cb8f8beb0d382b61e7ccc62435bf7311c9f4b210",
    strip_prefix = "libpng-1.6.35",
    urls = [
        "https://github.com/glennrp/libpng/archive/v1.6.35.tar.gz",
    ],
)

http_archive(
    name = "cc_zlib",
    build_file_content = all_content,
    sha256 = "629380c90a77b964d896ed37163f5c3a34f6e6d897311f1df2a7016355c45eff",
    strip_prefix = "zlib-1.2.11",
    urls = [
        "https://github.com/bazelregistry/zlib/archive/v1.2.11.tar.gz",
    ],
)

http_archive(
    name = "cc_lzo",
    build_file_content = all_content,
    sha256 = "c0f892943208266f9b6543b3ae308fab6284c5c90e627931446fb49b4221a072",
    strip_prefix = "lzo-2.10",
    urls = [
        "https://www.oberhumer.com/opensource/lzo/download/lzo-2.10.tar.gz",
    ],
)

http_archive(
    name = "cc_openssl",
    build_file_content = all_content,
    sha256 = "ddb04774f1e32f0c49751e21b67216ac87852ceb056b75209af2443400636d46",
    strip_prefix = "openssl-1.1.1g",
    urls = [
        "https://www.openssl.org/source/openssl-1.1.1g.tar.gz",
    ],
)

http_archive(
    name = "cc_libgcrypt",
    build_file_content = all_content,
    sha256 = "0cba2700617b99fc33864a0c16b1fa7fdf9781d9ed3509f5d767178e5fd7b975",
    strip_prefix = "libgcrypt-1.8.6",
    urls = [
        "https://gnupg.org/ftp/gcrypt/libgcrypt/libgcrypt-1.8.6.tar.bz2",
    ],
)

http_archive(
    name = "cc_opencv",
    build_file_content = all_content,
    sha256 = "36799186756c1e12adde97f0a8d1afc395d5b0f86d8ad9ef951bc33aa732f9b9",
    strip_prefix = "opencv-4.3.0",
    urls = [
        "https://github.com/opencv/opencv/archive/4.3.0.zip",
    ],
)

git_repository(
    name = "cc_yaml",
    commit = "1c9abc8fa4702c06d53b866dd2e7ca3b9c865f7e",
    remote = "https://github.com/jbeder/yaml-cpp.git",
    shallow_since = "1595593720 -0500",
)

http_archive(
    name = "cc_openjpeg",
    build_file_content = all_content,
    sha256 = "63f5a4713ecafc86de51bfad89cc07bb788e9bba24ebbf0c4ca637621aadb6a9",
    strip_prefix = "openjpeg-2.3.1",
    urls = [
        "https://github.com/uclouvain/openjpeg/archive/v2.3.1.tar.gz",
    ],
)
