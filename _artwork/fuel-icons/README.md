# Fuel GPU application icons

Four icons for the draw2d GPU UI test applications. The shared fuel flame and GPU
chip identify the family; circle, rectangle and stroke primitives suggest draw2d.

| Application | Rendering motif | Backend accent |
| --- | --- | --- |
| fuel-memory-opengl | Stacked offscreen render buffers | Cyan / blue |
| fuel-memory-vulkan | Stacked offscreen render buffers | Purple / violet |
| fuel-window-opengl | Onscreen application window and display | Cyan / blue |
| fuel-window-vulkan | Onscreen application window and display | Purple / violet |

Assets live at `_matter/<application>/_std/_std/main/` relative to app-core.
Each set contains RGBA PNGs at 16, 24, 32, 48, 49, 64, 96, 128, 256, 512 and
1024 pixels, an `icon.png` alias of the 1024px master, and an `icon.ico` with
16, 24, 32, 48, 64, 128 and 256px frames. The exterior is transparent.

The artwork was generated with the built-in image generation tool. The Vulkan
versions were subsequently recolored to purple using the same tool. Pillow is
used only to export the requested dimensions, package ICOs and make the review
sheet. Original generation prompts are recorded in `prompts.json`; the purple
edit prompt used for both Vulkan icons is in `recolor-prompt.txt`.

To rebuild the smaller assets and `preview.png` from the four 1024px masters:

```sh
python -m pip install pillow
python _artwork/fuel-icons/export_icons.py
```

The four application `matter.txt` files already include the corresponding
`app-core/<application>` matter root, so the assets use the existing resource
lookup convention.
