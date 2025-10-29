# embedlibs

[![Formatting](https://github.com/sivakov512/embedlibs/actions/workflows/formatting.yml/badge.svg)](https://github.com/sivakov512/embedlibs/actions/workflows/formatting.yml)
[![Host Checks](https://github.com/sivakov512/embedlibs/actions/workflows/host-checks.yml/badge.svg)](https://github.com/sivakov512/embedlibs/actions/workflows/host-checks.yml)

Small C libraries I use everywhere in embedded projects.

## Installation

Add to your ESP-IDF project `idf_component.yml`:

<!--x-release-please-start-version-->
```yaml
dependencies:
  embedlibs:
    git: https://github.com/sivakov512/embedlibs.git
    version: "v1.2.0"
```
<!--x-release-please-end-->

## Development

```bash
# Format code
make check-format

# Run host tests
make host-test

# Run static analysis
make host-lint
```
