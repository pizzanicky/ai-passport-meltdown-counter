#!/usr/bin/env python3
"""Check meltdown UI strings against the generated fonts and the PCM clips."""

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCES = (
    ROOT / "main/meltdown_model.c",
    ROOT / "main/meltdown_ui.c",
    ROOT / "main/meltdown_app.c",
)
FONTS = (
    ROOT / "assets/fonts/meltdown_font_12.c",
    ROOT / "assets/fonts/meltdown_font_16.c",
)


def string_literals(text: str) -> list[str]:
    stripped = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    stripped = re.sub(r"//.*?$", "", stripped, flags=re.M)
    return re.findall(r'"((?:\\.|[^"\\])*)"', stripped)


def code_points(literal: str) -> set[int]:
    points: set[int] = set()
    index = 0
    while index < len(literal):
        if literal[index] == "\\" and index + 1 < len(literal):
            index += 2
            continue
        point = ord(literal[index])
        if point > 127:
            points.add(point)
        index += 1
    return points


def font_points(path: Path) -> set[int]:
    return {int(value, 16) for value in re.findall(r"/\* U\+([0-9A-Fa-f]+)", path.read_text(encoding="utf-8"))}


def main() -> None:
    required: set[int] = set()
    for path in SOURCES:
        for literal in string_literals(path.read_text(encoding="utf-8")):
            required |= code_points(literal)
    assert required, "expected Chinese UI strings"
    # Active UI uses generated A8 phrase images, not the legacy font files.
    dots = (ROOT / "assets/dots/meltdown_dots.c").read_text(encoding="utf-8")
    available = set()
    for literal in string_literals(dots):
        available |= code_points(literal)
    assert required <= available, sorted(required - available)

    c3 = (ROOT / "assets/music/meltdown_c3_pcm.c").read_text(encoding="utf-8")
    egg = (ROOT / "assets/music/meltdown_egg_pcm.c").read_text(encoding="utf-8")
    assert "meltdown_c3_pcm_len = 35200" in c3
    assert "meltdown_c3_pcm_rate = 16000" in c3
    assert "meltdown_egg_pcm_len = 28800" in egg
    assert "meltdown_egg_pcm_rate = 16000" in egg

    pins = (ROOT / "components/bsp/include/bsp_pins.h").read_text(encoding="utf-8")
    assert re.search(r"#define BSP_BTN_SHORT_PRESS_MS\s+5\b", pins)
    assert re.search(r"#define BSP_BTN_LONG_PRESS_MS\s+500\b", pins)
    main_c = (ROOT / "main/main.c").read_text(encoding="utf-8")
    assert "enter_menu" not in main_c
    assert "meltdown_app_start" in main_c
    cmake = (ROOT / "main/CMakeLists.txt").read_text(encoding="utf-8")
    assert "demo_display.c" not in cmake
    assert "meltdown_app.c" in cmake
    print("meltdown asset checks passed")


if __name__ == "__main__":
    main()
