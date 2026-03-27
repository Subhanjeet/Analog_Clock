# 🕐 Analog Clock in C — BGI Graphics

> *No frameworks. No libraries. Just raw C, a loop that never quits, and enough trigonometry to make your math teacher cry tears of joy.*

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)]()
[![Compiler: Turbo C](https://img.shields.io/badge/Compiler-Turbo%20C-red.svg)]()
[![Graphics: BGI](https://img.shields.io/badge/Graphics-BGI-green.svg)]()

---

## 💀 What Is This?

A **real-time analog clock** rendered entirely in C using Turbo C and the BGI graphics library. Zero dependencies on modern frameworks — this project is built on raw trigonometry, a 640×480 graphics window, and a `while(1)` loop that simply does not stop. Ever.

The clock reads live system time via `struct tm`, converts each time unit into a degree angle, and uses `cos()` / `sin()` to calculate pixel coordinates for every hand. It's giving *1990s computer science lab* energy but the logic is absolutely immaculate.

---

## 👀 Preview

```
         ┌─────────────────────────────────┐
         │       BGI Graphics Window       │
         │                                 │
         │           ── 12 ──              │
         │        ╱    │      ╲            │
         │       9 ─────●───── 3           │
         │        ╲           ╱            │
         │           ── 6 ──               │
         │           HH:MM:SS              │
         └─────────────────────────────────┘
```

---

## 🧠 The Core Math (This Is Where It Gets Nerdy)

Every single hand — hour, minute, second — is drawn using one formula. One. That's it. The entire clock is just this repeated three times with different numbers:

```
angle_radians = (degrees - 90) × π / 180
x = CX + length × cos(angle)
y = CY + length × sin(angle)
```

### Why subtract 90°? (The Part Everyone Gets Wrong)

```
  Without −90°           With −90°
  
  cos(0°) → RIGHT        cos(0°−90°) → TOP
  
      ──►                     ▲
      0°                     12
  
  Clock starts at 3     Clock starts at 12  ✓
```

`cos(0)` naturally points to 3 o'clock. Clocks start at 12 o'clock. Subtracting 90° from every angle rotates the entire coordinate system so the origin aligns with the top. This is the single most important line in the whole program — fr fr.

---

## ⏱️ Angle Formulas for Each Hand

Converting time units to degrees on a 360° circle:

```
┌──────────────┬──────────────────────────┬────────────────────────────┐
│ Hand         │ Formula                  │ Logic                      │
├──────────────┼──────────────────────────┼────────────────────────────┤
│ Second hand  │ sec × 6                  │ 360° ÷ 60 sec = 6°/sec     │
│ Minute hand  │ min × 6 + sec / 10       │ 6°/min + smooth drift      │
│ Hour hand    │ hr × 30 + min / 2        │ 360° ÷ 12 hr = 30°/hr      │
└──────────────┴──────────────────────────┴────────────────────────────┘
```

> The drift terms (`sec/10` and `min/2`) are what separate a *good* clock from a *great* one. Without them the hands jump between positions like a PowerPoint transition. With them, they glide smoothly. Smooth hands = smooth brain moment unlocked.

---

## 🔄 Program Flow (The Infinite Loop Arc)

```
         ┌─────────────────────────┐
         │     initgraph()         │  ← Opens the BGI graphics window
         └────────────┬────────────┘
                      │
         ┌────────────▼────────────┐
    ┌───▶│      while (1)          │  ← Runs. Forever. No cap.
    │    └────────────┬────────────┘
    │                 │
    │    ┌────────────▼────────────┐
    │    │     cleardevice()       │  ← Wipes previous frame
    │    └────────────┬────────────┘
    │                 │
    │    ┌────────────▼────────────┐
    │    │  time() + localtime()   │  ← Grabs hr, min, sec from OS
    │    └────────────┬────────────┘
    │                 │
    │    ┌────────────▼────────────┐
    │    │   Calculate angles      │  ← Pure trig. This is the fun part.
    │    └────────────┬────────────┘
    │                 │
    │    ┌────────────▼────────────┐
    │    │     drawFace()          │  ← Rings + markers + dots
    │    └────────────┬────────────┘
    │                 │
    │    ┌────────────▼────────────┐
    │    │   drawHand() × 3        │  ← Hour → Minute → Second
    │    └────────────┬────────────┘
    │                 │
    │    ┌────────────▼────────────┐
    │    │   outtextxy()           │  ← Prints HH:MM:SS on screen
    │    └────────────┬────────────┘
    │                 │
    │    ┌────────────▼────────────┐
    └────│     delay(1000)         │  ← Chills for 1 second. Then repeats.
         └─────────────────────────┘
```

---

## 🎨 Clock Face Structure

```
                      12
                     │   ← Thick hour marker
                 ●   │   ●     ← Minute dots (putpixel, LIGHTGRAY)
            ●                  ●
          9 ──────── ● ──────── 3   ← Red center dot (fillellipse)
            ●                  ●
                 ●   │   ●  
                     6

  Outer ring : circle(CX, CY, R)       ← R = 180
  Inner ring : circle(CX, CY, R − 5)  ← Creates double-ring border
  Hour marks : 12 thick lines, i × 30°, from R−10 to R−25
  Min dots   : 48 pixels at R−12, angle i × 6° (skip every 5th)
  Center dot : fillellipse(320, 240, 6, 6), RED — covers all hand roots
```

---

## 📐 drawFace() — Step by Step

```
Step 1 ── Two white rings
          circle(320, 240, 180)    ← outer
          circle(320, 240, 175)    ← inner

Step 2 ── 12 hour markers  (loop i = 0 to 11)
          angle = i × 30°
          draw line from radius (R−10) to (R−25)
          thickness = 3, color = WHITE

Step 3 ── 48 minute dots  (loop i = 0 to 59, skip i % 5 == 0)
          angle = i × 6°
          putpixel at radius (R−12)
          2px wide, LIGHTGRAY

Step 4 ── Red center dot
          fillellipse(320, 240, 6, 6)
          Covers the ugly intersection of all three hands. Smart fix. W move.
```

---

## ✋ drawHand() — Annotated

```c
void drawHand(int angle_deg, int length, int color, int thickness)
{
    double angle;
    int x, y;

    // Convert degrees → radians, shift origin to 12 o'clock
    angle = (angle_deg - 90) * PI / 180.0;

    // Trig does the heavy lifting — finds tip pixel
    x = CX + (int)(length * cos(angle));
    y = CY + (int)(length * sin(angle));

    // BGI drawing calls — sets color and draws the line
    setcolor(color);
    setlinestyle(SOLID_LINE, 0, thickness);
    line(CX, CY, x, y);   // center → tip
}
```

---

## 📏 Hand Specifications

```
┌──────────────┬──────────┬─────────┬──────────┬───────────────────────┐
│ Hand         │ Length   │ Pixels  │ Color    │ Thickness             │
├──────────────┼──────────┼─────────┼──────────┼───────────────────────┤
│ Hour         │ R × 0.50 │ 90 px   │ WHITE    │ 3  (chonky)           │
│ Minute       │ R × 0.75 │ 135 px  │ WHITE    │ 2  (medium)           │
│ Second       │ R × 0.85 │ 153 px  │ RED      │ 1  (thin, fast, red)  │
└──────────────┴──────────┴─────────┴──────────┴───────────────────────┘
```

> Shorter + thicker = slow, powerful (hour hand built different).
> Longer + thinner = fast, dramatic (second hand said run).

---

## ⚙️ Requirements

| Item            | Detail                                        |
|-----------------|-----------------------------------------------|
| Compiler        | Turbo C 3.0 or Dev-C++ with WinBGIm           |
| OS              | Windows (DOS/NT), or DOSBox on modern systems |
| Screen Mode     | 640 × 480, 16-color BGI                       |
| Libraries       | `graphics.h`, `math.h`, `time.h`, `dos.h`     |
| Brain Required  | Basic C knowledge + respect for trigonometry  |

---

## 🚀 How to Run

**In Turbo C:**
1. Open `ANALOG_C.C`
2. Press `Ctrl + F9` to compile and run
3. Press `Alt + F5` to view the output window

---

## 🧪 Concepts Covered

- **BGI graphics** — `initgraph`, `circle`, `line`, `putpixel`, `fillellipse`, `outtextxy`
- **Trigonometry in graphics** — `cos()` and `sin()` converting angles to pixel coordinates
- **System time in C** — `time()`, `localtime()`, `struct tm`
- **Animation loop pattern** — `cleardevice()` + `delay()` for frame-by-frame rendering
- **C89 strict mode** — Turbo C rules: all variables declared at top, no `for(int i = 0; ...)`
- **Coordinate system rotation** — the −90° offset trick that every graphics programmer needs to know

---

## 📜 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.
You can use it, modify it, distribute it, learn from it.
Just keep the copyright notice. That's literally all that's asked.


---

*Built from scratch. No tutorials were harmed in the making of this project 😜*
