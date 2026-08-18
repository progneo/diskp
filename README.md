# DiskP

## Yet another disk scanning tool but for projects scanning

### Base features

1. Finding **regenerable** and **irreplaceable** directories in projects:
    1. **regenerable** directories - can be removed and restored later. Can be split into:
        1. Needs internet traffic to download some data *(download required libraries)*
        2. Needs CPU usage to create this files *(build some artifacts)*
        3. Both of them *(Docker layers for example)*
    2. **irreplaceable** directories - shouldn't be deleted. Can not be restored without any backups *(.git directory as
       example)*.
2. Dotfile as config:
    1. File is placed in `~/.config/diskp/config.toml`
    2. Exclude any directories from scanning
    3. Select searchable frameworks/tools *(Docker, Helm etc. as tools, and venv, node.js etc. as frameworks)*
    4. Add own directory with directory type. Can override preexist config
    5. Config precedence - CLI flags > user config > built-in rules
3. Report after scanning by groups ("safe to delete now", "delete if you're offline-tolerant", "don't touch") with
   percentage (between allocated reclaimable) and size (apparent and allocated) sorted by decreasing
4. Linked files count only once
5. Only CLI with pretty UI (sometimes later GUI)
6. Flags:
    1. **--auto-clean** - auto-removing regenerable files
    2. **--max-cost=max restore cost** - works only with auto-clean and required to id, allows to remove only with
       specified restore cost and lower
    3. **--min-confidence=min confidence level** - works only with auto-clean and required to id, allows to remove only
       with specified confidence and higher
    4. **--force-remove** - removes all files instantly without moving to trash
    5. **--sort=asc/desc** - sorting in report by reclaimable allocated size
    6. **--exclude=directory** (multiple) - exclude directory from scanning
    7. **--searchable=Docker/Helm/node.js/etc.** - what to search, comma is delimiter
    8. **--dir=directory** - where start to search (if not provided - use current directory)
    9. **--json** - returns report in JSON format
7. Only macOS supported for now

### Confidence levels of **regenerable**

There are 4 confidence levels:

| Level            | Meaning                                                                                               | Example                                                                        |
|------------------|-------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------|
| **verified**     | a lockfile or equivalent authoritative manifest exists that pins what regeneration produces           | `package-lock.json`, `Cargo.lock`, `poetry.lock`, `go.sum`                     |
| **likely**       | project manifest identifies the tool, and the directory is that tool's known output path, but no lock | 'CMakeLists.txt' + 'build/', 'package.json' without lockfile + 'node_modules/' |
| **guess**        | directory name matched a known pattern, no supporting marker nearby                                   | `node_modules/` with no `package.json`, lockfile, or other Node markers        |
| **unclassified** | cannot determine whether it is safely regenerable                                                     | Arbitrary `cache/`, `tmp/`, `generated/`, or custom directory                  |

And there are 4 restore costs:

1. free - free to recover
2. cpu - required some CPU usage to rebuild
3. network - required network connection
4. both - required network connection and CPU usage

### *Non*-goals

1. Not yet another yet treemap tool
2. Not an all-in-one tool
3. Not **100% will find all your projects** tips and hints for now

### Safety invariants

- Dry-run is the default; a normal run never modifies anything
- Must not operate on `/`, `$HOME`, system paths
- Never remove files without a flag
- Deletion moves to `~/.Trash` unless `--force-remove`
- Classification is re-verified immediately before deleting (the state on disk changed since the scan)
- Never delete through a symlink

### Notes

***Project*** - a project root is the nearest ancestor directory containing any marker from a known set; a directory's
classification only looks at markers within its own project root; nested roots are separate projects and the inner one
wins. For now is: `.git`, `package.json`, `Cargo.toml`, `go.mod`, `CMakeLists.txt`, `pyproject.toml`, `pom.xml`,
`build.gradle`