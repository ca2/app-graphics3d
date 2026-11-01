# Fuel graphics configuration

The base application resolves the saved settings and defaults. A derived
application can set these members in its constructor:

```cpp
m_strGraphics3DImplementationOverride = "vulkan";
m_strGraphics3DOutputOverride = "off_screen";
```

The virtual `graphics3d_get_implementation_override()` and
`graphics3d_get_output_override()` hooks can also be overridden. Empty overrides
preserve the ordinary `graphics3d.txt` and `graphics3d_output.txt` settings.
Application overrides take priority and do not rewrite either file.

Each `fuel-memory-*` variant sets `off_screen`; each `fuel-window-*` variant sets
`on_screen`. The GPU API is fixed to the suffix of the application name.

Fuel's Options tab uses the shared graphics settings form. Fixed GPU/output
controls are disabled. The draw2d choice is saved in the existing per-application,
per-operating-system `draw2d.txt` setting and takes effect after restarting.

| GPU | Default draw2d | GPU-compatible alternative |
| --- | --- | --- |
| OpenGL | opengl | nanovg |
| Vulkan | vulkan | vkvg |
| DirectX 11 | directx11 | direct2d_for_directx11 |
| DirectX 12 | directx12 | direct2d_for_directx12 |

Offscreen output also permits the platform CPU draw2d implementations (GDI+ and
Direct2D on Windows, Quartz 2D on Apple platforms, cairo elsewhere). Missing or
incompatible draw2d settings fall back to the selected GPU API. GPU-compatible
alternatives remain available for both offscreen and on-screen output.

The configuration policy is tested directly by
`source/app/aura/platform/tests/graphics_configuration_tests.cpp`. Compile and run
it with a C++17 compiler; it has no framework or GPU runtime dependencies.
