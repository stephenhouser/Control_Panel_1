# Control Panel

The old-school hard-switch control panel for Home Automation.

This project has a few parts, each documented in their own section below.

- Project Box from Amazon; link and description
- Base and Top panels; SVG files for a laser cutter
- PCBs for Base and Top panels; KiCad PCB designs for manufacture

# Project Box

The project box is a [BUD Industries PC-11495 Plastic Style F Box 9" W x 5" H x 8.5" D, Natural](
https://www.amazon.com/gp/product/B005T98PQS/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) which I found on Amazon for ~$32 USD. The [Dimensions](https://www.budind.com/wp-content/uploads/2019/01/hb11495.pdf) are available from BUD Industries (thank you!)

Another option, without the top display is the [BUD Industries PC-11493 Plastic Style F Box 9" L x 8.5" W x 1.98" H, Natural](https://www.amazon.com/gp/product/B005T592P0/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) for ~$30 USD. The [Dimensions](https://www.budind.com/wp-content/uploads/2019/01/hb11493.pdf) are available from BUD Industries (thank you!)

You can find details on these boxes in general from [BUD Industries](https://www.budind.com/series/general-use-boxes/plasticase-style-f-plastic-cases-2/#group=series-products&external_dimensions_group=0&internal_dimensions=0)

# Base and Top Panels

Look in the Box_Panels directory. These are SVG files for a laser cutter

# PCBs for Base and Top panels

Uses KiCad v7.x for two PCB's; Control Board and Display Board.

Control Board
---

- Version 1.0 sent to fab on 7/17/2023

Components:

-  1 x ESP32 DevKit C ([Amazon](https://www.amazon.com/AITRIP-ESP32-DevKitC-Development-ESP32-WROOM-32D-Compatible/dp/B08HMJ1X6W/ref=sr_1_1_sspa?crid=38RBRSP8DY486&keywords=esp32+devkitc&qid=1689705861&sprefix=esp32+devkit+%2Caps%2C108&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1))
- 16 x Addressable RGB LEDs ([sparkfun](https://www.sparkfun.com/products/12986))
- 10 x 2-position SPST Toggle Switches ([Amazon](https://www.amazon.com/MTS-101-Position-Miniature-Toggle-Switch/dp/B0799LBFNY/ref=sr_1_1_sspa?crid=Q083CMZJXX9C&keywords=mini+toggle+switch&qid=1689705831&sprefix=mini+toggle+switch%2Caps%2C114&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1))
-  1 x 3-position SPST Toggle Switch ([Amazon](https://www.amazon.com/Twidec-Momentary-Miniature-Waterproof-MTS-123-MZ/dp/B07VHCB1Q4/ref=sr_1_4?crid=1ODFBUY13M4Y&keywords=3+position+mini+toggle+switch&qid=1689705803&sprefix=3+position+mini+toggle+switch%2Caps%2C120&sr=8-4))
- 10 x 1N4148 Diodes
-  1 x Rotary Encoder + Knob ([Amazon](https://www.amazon.com/gp/product/B07DM2YMT4/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1))
-  1 x Mini pushbutton switch ([Amazon](https://www.amazon.com/Momentary-Spring-Return-Self-Return-Pushbutton-Switches/dp/B09DJY5Y5L/ref=sr_1_3?keywords=mini+pushbutton+switch&qid=1689705893&sprefix=mini+pushbutton+s%2Caps%2C102&sr=8-3))


Display Board
---
Display Board not yet created.

Components:

-  2 x 0.56" LED Display Digital Tube Clock Module 4 Digit ([Amazon](https://www.amazon.com/gp/product/B0BB72R6RW/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1))