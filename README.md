---
title: "analog-clock"
name: "analog-clock"
tagline: "A lightweight borderless desktop analog clock widget for Windows and macOS."
date: "2023-12"

type: tool                  # tool | experiment | library | site | game
platform:
  - desktop
status: archived            # active | archived | wip
visibility: public          # public | archived | private-demo
featured: false

tech:
  - C++
  - Qt 5

ai:
  level: none               # none | light | moderate | heavy
  tools: []
  notes: "Built without AI assistance."

screenshots:
  - screenshots/screenshot-1.png
  - screenshots/screenshot-2.png

live_url: "https://github.com/peteristhegreat/analog-clock-svg-qt/releases"
repo_url: "https://github.com/peteristhegreat/analog-clock-svg-qt"

blog_posts: []
# blog_posts:
#   - label: "Post title here"
#     url: https://example.com/blog/post

popularity:
  label: personal-use       # personal-use | open-source | shipped

motivation: "Built as a simple weekend desktop utility to provide a clean, always-visible analog clock experience without window chrome or unnecessary distractions."
---

## Overview

Analog Clock is a small cross-platform desktop widget built with Qt 5. It provides a clean analog clock display in a borderless window designed to sit naturally on the desktop without feeling like a full application.

The project was a focused utility build rather than a broad product: lightweight, visual, and practical. It emphasized desktop packaging, simple presentation, and compatibility across Windows and macOS.

## Features

- Borderless desktop analog clock widget
- Lightweight utility-focused design
- Cross-platform support for Windows and macOS
- Always-visible clock-style presentation
- Minimal UI with no unnecessary controls or chrome

## Installation

```bash
# clone the repository
git clone https://github.com/peteristhegreat/analog-clock-svg-qt.git

# enter the project directory
cd analog-clock-svg-qt

# open in Qt Creator or build with qmake / cmake depending on project setup
# example:
qmake
make
```
