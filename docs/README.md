# Website

This website is built using [Docusaurus](https://docusaurus.io/), a modern static website generator.

## Development

### Updating Package Lock

The bazel rules use pnpm as a package manager, so everything has to go through it. To update the package lock file `pnpm-lock.yaml`, run the following command:

```bash
# If you're not already in the right location, do so now!
cd docs/site

bazel run -- @pnpm --dir $PWD install --lockfile-only
```
