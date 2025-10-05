import argparse
import toml
import requests


def parse_version_toml(version_file_path="VERSION.toml") -> tuple:
    """Parse the VERSION.toml file and return major, minor, and patch versions.
    Args:
        version_file_path (str): Path to the VERSION.toml file.
    Returns:
        tuple: A tuple containing major (int), minor (int), and patch (dict).
            The patch dict contains package-specific patch versions.
    """
    with open(version_file_path, "r") as f:
        version_toml = toml.load(f)

    major = version_toml["major"]
    minor = version_toml["minor"]
    patch = version_toml["patch"]

    return major, minor, patch


def update_rust(versions: tuple, dryrun: bool = False) -> bool:
    """Update the Rust Cargo.toml version based on the provided versions.
    Args:
        versions (tuple): A tuple containing major, minor, and patch versions.
        dryrun (bool): If True, only check for consistency without making changes.
    Returns:
        bool: True if an update was successfully made.
            In the case of dryrun, returns False if versions are inconsistent, True if consistent.
    """
    major, minor, patch = versions
    version_str = f"{major}.{minor}.{patch}"

    # Get all used versions from Crates.io
    crate_details = requests.get("https://crates.io/api/v1/crates/dv").json()
    crate_versions = [v["num"] for v in crate_details["versions"]]

    with open("core/Cargo.toml", "r") as f:
        cargo_toml = toml.load(f)

    current_version = cargo_toml["package"]["version"]

    versions_match = current_version == version_str
    version_not_used = version_str not in crate_versions

    has_passed = True

    print("\n--- Rust Version Check ---")

    if version_not_used:
        print("\t✅ Rust Cargo.toml version is not yet published on Crates.io.")
        has_passed &= True
    else:
        print("\t❌ Rust Cargo.toml version is already published on Crates.io.")
        has_passed &= False

    if versions_match:
        # Versions already match
        print("\t✅ Rust Cargo.toml version is consistent.")
        has_passed &= True
    else:
        if dryrun:
            print("\t❌ Rust Cargo.toml version is inconsistent.")
            print(f"\t\tCargo.toml:   {current_version}")
            print(f"\t\tVersion.toml: {version_str}")
            has_passed &= False  # In dryrun mode, indicate inconsistency
        elif not version_not_used:
            print(
                "\t❌ Rust Cargo.toml version cannot be automatically updated because the target version is already published on Crates.io."
            )
            has_passed &= False
        else:
            # Update the version in the Cargo.toml content
            cargo_toml["package"]["version"] = version_str
            with open("core/Cargo.toml", "w") as f:
                toml.dump(cargo_toml, f)
            print(
                f"\t⚠️  Updated Rust Cargo.toml version from {current_version} to {version_str}."
            )
            has_passed &= True

    return has_passed


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--dryrun", action="store_true", help="Check if versions are consistent."
    )
    parser.add_argument(
        "--all", action="store_true", help="Update all package versions. Defaults to true if no specific package is specified. and this is not specified."
    )
    parser.add_argument(
        "--rust", action="store_true", help="Only update Rust version."
    )
    parser.add_argument(
        "--python", action="store_true", help="Only update Python version."
    )
    parser.add_argument(
        "--cpp", action="store_true", help="Only update C++ version."
    )
    parser.add_argument(
        "--version_file", type=str, default="VERSION.toml", help="Path to version file"
    )
    args = parser.parse_args()

    if not (args.all or args.rust or args.python or args.cpp):
        args.all = True  # Default to updating all if no specific package is specified

    all_passed = True

    # Welcome Message
    print("\n================================")
    print("DV Update Versions Tool")
    print("================================")

    if args.dryrun:
        print("\n⚠️  Running in dryrun mode. No files will be modified. ⚠️\n")

    # Get versions
    versions = parse_version_toml(args.version_file)

    # Update Rust version
    if args.all or args.rust:
        all_passed &= update_rust(versions, dryrun=args.dryrun)

    if args.python:
        print("\n Python version update not yet implemented.")
        all_passed &= False

    if args.cpp:
        print("\n C++ version update not yet implemented.")
        all_passed &= False

    if not all_passed:
        print("\n❌ Version inconsistencies detected. Please resolve the issues above.")
        print("   Run `bazel run //:update_versions` to automatically fix issues where possible.")
        exit(1)

    print("\n✅ All version checks passed!")
    exit(0)


if __name__ == "__main__":
    main()
