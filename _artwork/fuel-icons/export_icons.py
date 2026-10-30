"""Export the Fuel application PNG/ICO sizes and comparison sheet from their 1024px masters."""
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[2]
APPS = (
    "fuel-memory-opengl", "fuel-memory-vulkan",
    "fuel-window-opengl", "fuel-window-vulkan",
)
SIZES = (16, 24, 32, 48, 49, 64, 96, 128, 256, 512, 1024)


def export():
    preview = Image.new("RGB", (1120, 440), "#18212e")
    draw = ImageDraw.Draw(preview)
    font = ImageFont.load_default(size=17)
    for column, app in enumerate(APPS):
        folder = ROOT / "_matter" / app / "_std" / "_std" / "main"
        with Image.open(folder / "icon-1024.png") as original:
            master = original.convert("RGBA")
        assert master.size == (1024, 1024), app
        assert master.getchannel("A").getextrema() == (0, 255), app
        master.save(folder / "icon.png")
        for size in SIZES:
            master.resize((size, size), Image.Resampling.LANCZOS).save(folder / f"icon-{size}.png")
        master.save(folder / "icon.ico", bitmap_format="bmp",
                    sizes=[(n, n) for n in (16, 24, 32, 48, 64, 128, 256)])
        x = column * 280
        preview.paste(master.resize((256, 256), Image.Resampling.LANCZOS), (x + 12, 16),
                      master.resize((256, 256), Image.Resampling.LANCZOS))
        draw.text((x + 16, 286), app, fill="#e5e9f0", font=font)
        for offset, size in zip((16, 104, 168, 216), (64, 48, 32, 16)):
            small = master.resize((size, size), Image.Resampling.LANCZOS)
            preview.paste(small, (x + offset, 326), small)
            draw.text((x + offset, 402), str(size), fill="#b8c5d6", font=font)
        for size in SIZES:
            with Image.open(folder / f"icon-{size}.png") as check:
                assert check.size == (size, size) and check.mode == "RGBA"
        with Image.open(folder / "icon.ico") as check:
            assert len(check.ico.sizes()) == 7
        print(f"Validated {app}: {len(SIZES) + 1} PNGs and a 7-resolution ICO")
    preview.save(Path(__file__).with_name("preview.png"))


if __name__ == "__main__":
    export()
