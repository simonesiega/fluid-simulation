# Build skill

Use this skill for CMake, dependencies, compiler configuration, project layout, runtime resources, local execution, and CUDA build plumbing.

Do not use it for solver mathematics, shader/kernel implementation details, benchmark methodology, or release writing unless the task also changes the build.

## Build contract

- Target C++20.
- Keep CMake as the source of truth for configure, build, test, install, and packaging behavior.
- Keep Windows + MSVC as the first-class development target while avoiding unnecessary platform lock-in.
- Prefer target-based CMake commands (`target_*`) over directory-wide/global flags.
- Keep generated files, compiler output, caches, and packaged artifacts under `build/` or another generated-output directory.
- Never commit generated build output, IDE state, local binaries, or release ZIPs.
- Add dependencies only when they clearly improve the project.

## raylib

- Keep raylib pinned to the project-approved version and fetched reproducibly with CMake.
- Set raylib cache/options before `FetchContent_MakeAvailable(raylib)`.
- Do not require a separate manual raylib installation for the normal build.
- Keep project-owned warnings/options on project targets; do not make third-party raylib warnings fatal.

## Targets and source layout

Create directories and targets only when the roadmap or current implementation needs them.

The project is expected to grow around:

```text
src/
├── app/
├── graphics/
├── simulation/
│   ├── cpu/
│   ├── gpu/
│   └── cuda/
├── platform/
└── util/
shaders/
assets/
tests/
docs/
```

- When adding a `.cpp`, `.hpp`, `.cu`, shader, test, or runtime resource, update the owning CMake target/rules in the same change when required.
- Prefer explicit source lists over broad recursive globbing.
- Keep executable/library ownership clear as the project grows.

## Runtime resources

- Treat shaders, icons, and other runtime assets as build/runtime resources rather than source-code assumptions.
- Copy or install required resources with CMake so the executable does not depend on being launched from the repository root.
- Use the CMake install layout as the basis for the final Windows package.

## Optional CUDA

CUDA is additive and must never become a requirement for the portable build.

- Keep the default/non-CUDA configure path working without the CUDA Toolkit.
- Detect/enable CUDA only inside the CUDA-enabled path.
- Keep `.cu` sources and CUDA-specific link settings isolated from the portable targets where practical.
- A machine without an NVIDIA GPU must still be able to configure and build the non-CUDA project.
- Do not silently change CPU/OpenGL behavior when CUDA support is disabled.

## Validation

For build-system changes, validate the smallest relevant set:

1. clean or fresh CMake configure;
2. build the affected target in the intended configuration;
3. run the executable or affected test when practical;
4. verify required runtime resources appear beside/in the expected output;
5. when touching CUDA build logic, also confirm the non-CUDA configure path still works.

Do not claim a build path was validated if it was not actually run.
