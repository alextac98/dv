load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@rules_python//python:packaging.bzl", "py_wheel")

WHEEL_PLATFORMS = select(
    {
        "//python:linux_aarch64": "linux_aarch64",
        "//python:linux_x86_64": "linux_x86_64",
        "//python:macos_arm64": "macosx_11_0_arm64",
        "//python:macos_x86_64": "macosx_10_15_x86_64",
        "//python:windows_x86_64": "win_amd64",
    },
    no_match_error = "Wheel platform tags are only configured for Linux/macOS x86_64 and arm64, plus Windows x86_64.",
)

def _nanobind_copts(repo_path, limited_api = False):
    windows = [
        "/std:c++17",
        "/I" + repo_path + "/include",
        "/I" + repo_path + "/src",
        "/I" + repo_path + "/ext/robin_map/include",
    ]
    posix = [
        "-std=c++17",
        "-faligned-allocation",
        "-fvisibility=hidden",
        "-I" + repo_path + "/include",
        "-I" + repo_path + "/src",
        "-I" + repo_path + "/ext/robin_map/include",
    ]
    if limited_api:
        windows = windows + ["/DPy_LIMITED_API=0x030C0000"]
        posix = posix + ["-DPy_LIMITED_API=0x030C0000"]
    return select({
        "//python:windows_x86_64": windows,
        "//conditions:default": posix,
    })

def dv_extension(
        name,
        nanobind_repo_path,
        nanobind_whl_files,
        nanobind_combined_src,
        python_headers,
        python_libs = None,
        limited_api = False,
        extra_deps = [],
        linkopts = []):
    wheel_files_target = "nanobind_wheel_files_{}".format(name)
    native_target = "dv_ext_native_{}".format(name)

    cc_library(
        name = wheel_files_target,
        hdrs = [nanobind_whl_files],
    )

    deps = [
        ":generated_nanobind_header_lib",
        ":" + wheel_files_target,
        "//core/ffi/diplomat:dv_diplomat_capi_static",
        python_headers,
    ]
    if python_libs:
        deps.append(python_libs)

    resolved_linkopts = select({
        "//python:windows_x86_64": ["ntdll.lib"],
        "//conditions:default": [],
    }) + linkopts

    cc_binary(
        name = native_target,
        srcs = [
            "dv_ext_ext.cpp",
            "sub_modules/dv_ext/BaseUnits_binding.cpp",
            "sub_modules/dv_ext/DvError_binding.cpp",
            "sub_modules/dv_ext/DimensionalVariable_binding.cpp",
            nanobind_combined_src,
        ],
        copts = _nanobind_copts(nanobind_repo_path, limited_api = limited_api),
        deps = deps + extra_deps,
        linkopts = resolved_linkopts,
        linkshared = True,
    )

def dv_runtime_package(name, native_target):
    so_target = "{}_so".format(name)
    pyd_target = "{}_pyd".format(name)

    native.genrule(
        name = so_target,
        srcs = [native_target],
        outs = ["dv_py/dv_ext.so"],
        cmd_bash = "cp $< $(RULEDIR)/dv_py/dv_ext.so",
        cmd_bat = "copy /Y \"$(location {})\" \"$(RULEDIR)\\dv_py\\dv_ext.so\" >NUL".format(native_target),
    )

    native.genrule(
        name = pyd_target,
        srcs = [native_target],
        outs = ["dv_py/dv_ext.pyd"],
        cmd_bash = "cp $< $(RULEDIR)/dv_py/dv_ext.pyd",
        cmd_bat = "copy /Y \"$(location {})\" \"$(RULEDIR)\\dv_py\\dv_ext.pyd\" >NUL".format(native_target),
    )

    native.filegroup(
        name = name,
        srcs = select({
            "//python:windows_x86_64": [":" + pyd_target],
            "//conditions:default": [":" + so_target],
        }),
    )

def dv_wheel(
        name,
        native_target,
        abi,
        python_tag,
        dist_folder,
        author,
        author_email,
        classifiers,
        description_file,
        distribution,
        license,
        project_urls,
        python_requires,
        summary,
        version):
    init_pkg_target = "{}_pkg_init".format(name)
    so_pkg_target = "{}_pkg_so".format(name)
    pyd_pkg_target = "{}_pkg_pyd".format(name)
    contents_target = "{}_contents".format(name)
    wheel_dir = name

    native.genrule(
        name = init_pkg_target,
        srcs = ["dv_py/__init__.py"],
        outs = ["{}/dv_py/__init__.py".format(wheel_dir)],
        cmd_bash = "cp $(location dv_py/__init__.py) $(RULEDIR)/{}/dv_py/__init__.py".format(wheel_dir),
        cmd_bat = "copy /Y \"$(location dv_py/__init__.py)\" \"$(RULEDIR)\\{}\\dv_py\\__init__.py\" >NUL".format(wheel_dir),
    )

    native.genrule(
        name = so_pkg_target,
        srcs = [native_target],
        outs = ["{}/dv_py/dv_ext.so".format(wheel_dir)],
        cmd_bash = "cp $(location {1}) $(RULEDIR)/{0}/dv_py/dv_ext.so".format(wheel_dir, native_target),
        cmd_bat = "copy /Y \"$(location {1})\" \"$(RULEDIR)\\{0}\\dv_py\\dv_ext.so\" >NUL".format(wheel_dir, native_target),
    )

    native.genrule(
        name = pyd_pkg_target,
        srcs = [native_target],
        outs = ["{}/dv_py/dv_ext.pyd".format(wheel_dir)],
        cmd_bash = "cp $(location {1}) $(RULEDIR)/{0}/dv_py/dv_ext.pyd".format(wheel_dir, native_target),
        cmd_bat = "copy /Y \"$(location {1})\" \"$(RULEDIR)\\{0}\\dv_py\\dv_ext.pyd\" >NUL".format(wheel_dir, native_target),
    )

    native.filegroup(
        name = contents_target,
        srcs = select({
            "//python:windows_x86_64": [":" + init_pkg_target, ":" + pyd_pkg_target],
            "//conditions:default": [":" + init_pkg_target, ":" + so_pkg_target],
        }),
    )

    py_wheel(
        name = name,
        abi = abi,
        author = author,
        author_email = author_email,
        classifiers = classifiers,
        description_file = description_file,
        distribution = distribution,
        dist_folder = dist_folder,
        license = license,
        platform = WHEEL_PLATFORMS,
        project_urls = project_urls,
        python_requires = python_requires,
        python_tag = python_tag,
        strip_path_prefixes = ["python/{}".format(wheel_dir)],
        summary = summary,
        version = version,
        deps = [":" + contents_target],
    )
