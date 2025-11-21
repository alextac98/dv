"""Bazel rules for building Python extensions with maturin."""

def _maturin_extension_impl(ctx):
    """Implementation of the maturin_extension rule."""
    
    # Get the manifest file (Cargo.toml)
    manifest = ctx.file.manifest
    
    # Determine the output file name based on platform
    if ctx.attr.platform == "darwin":
        extension = ".so"
    elif ctx.attr.platform == "windows":
        extension = ".pyd"
    else:
        extension = ".so"
    
    # Output file
    output = ctx.actions.declare_file(ctx.attr.module_name + extension)
    
    # Gather all source files
    srcs = ctx.files.srcs
    deps_manifests = [dep[DefaultInfo].files.to_list()[0] for dep in ctx.attr.deps if DefaultInfo in dep]
    
    # Create a command to run maturin build
    # We'll use maturin develop in release mode to build the extension
    manifest_dir = manifest.dirname
    
    # Create the build command
    build_script = ctx.actions.declare_file(ctx.label.name + "_build.sh")
    
    script_content = """#!/bin/bash
set -e

# Change to the directory containing Cargo.toml
cd {manifest_dir}

# Install maturin if not available (or ensure it's available)
if ! command -v maturin &> /dev/null; then
    echo "maturin not found. Please install it with: pip install maturin"
    exit 1
fi

# Build the extension with maturin
maturin build --release --strip --manylinux off

# Find the built wheel and extract the .so file
WHEEL=$(ls -t target/wheels/*.whl 2>/dev/null | head -1)

if [ -z "$WHEEL" ]; then
    echo "No wheel file found in target/wheels/"
    exit 1
fi

# Extract the .so/.pyd file from the wheel
unzip -o "$WHEEL" "*.so" "*.pyd" -d target/extracted/ 2>/dev/null || true

# Find the extension file
EXT_FILE=$(find target/extracted -name "*{module_name}*{extension}" | head -1)

if [ -z "$EXT_FILE" ]; then
    echo "Could not find extension file in wheel"
    exit 1
fi

# Copy to output location
cp "$EXT_FILE" {output_path}
""".format(
        manifest_dir = manifest_dir,
        module_name = ctx.attr.module_name,
        extension = extension,
        output_path = output.path,
    )
    
    ctx.actions.write(
        output = build_script,
        content = script_content,
        is_executable = True,
    )
    
    # Run the build script
    ctx.actions.run(
        executable = build_script,
        inputs = [manifest] + srcs + deps_manifests,
        outputs = [output],
        mnemonic = "MaturinBuild",
        progress_message = "Building Python extension with maturin: %s" % ctx.label.name,
        use_default_shell_env = True,
    )
    
    return [DefaultInfo(files = depset([output]))]

maturin_extension = rule(
    implementation = _maturin_extension_impl,
    attrs = {
        "manifest": attr.label(
            allow_single_file = ["Cargo.toml"],
            mandatory = True,
            doc = "The Cargo.toml manifest file",
        ),
        "srcs": attr.label_list(
            allow_files = [".rs"],
            doc = "Rust source files",
        ),
        "deps": attr.label_list(
            doc = "Dependencies (other Rust crates)",
        ),
        "module_name": attr.string(
            mandatory = True,
            doc = "Name of the Python module",
        ),
        "platform": attr.string(
            default = "linux",
            values = ["linux", "darwin", "windows"],
            doc = "Target platform",
        ),
    },
    doc = "Builds a Python extension module using maturin",
)
