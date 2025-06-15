# Snake.IO (WebAssembly)

A classic Snake game ported to the browser using C++ and Raylib, compiled to WebAssembly with Emscripten. Grow your snake by eating food, avoid walls and your own tail, and rack up the high score—all rendered in real-time on an HTML5 canvas with native-like performance.

---

## Demo

**[Live on Vercel](https://snake-io-in-wasm.vercel.app/)**  

Watch the demo (It takes some time to load or go to the demo_video folder ) ->
![Demo GIF](./demo_video/snake.gif)

---

## Features

- **WebAssembly Performance**  
  - Native-speed C++ game loop via Emscripten  
  - Real-time rendering with no frame rate cap

- **Raylib Rendering & Input**  
  - Clean, rounded-rectangle snake and food sprites  
  - Responsive arrow keys or WASD controls  

- **Procedural Food Spawning**  
  - Random, non-overlapping food positions on a 30×25 grid  
  - Tail-growth and scoring on each eat  

- **Collision & Game Over**  
  - Detect wall and self-collisions  
  - Automatic reset and score zeroing on game over  

- **Low-Latency Audio**  
  - Eat and crash effects handled via JavaScript AudioContext  
  - Preloaded and decoded AudioBuffers for instant playback  

- **Single-File Deployment**  
  - `snake.html`, `snake.js`, `snake.wasm`, and asset folders  
  - No backend required—just static hosting  

---

## Build & Run

### Prerequisites

- **Emscripten SDK** (latest)  
- **Raylib** (include headers & `libraylib.a`)  
- **Python 3** (for local HTTP server)

### Clone & Compile

```bash
git clone https://github.com/harsh-panchal-804
cd Snake.IO

# Build with Emscripten
emcc main.cpp -o snake.html \
  -O2 \
  -std=c++14 \
  -I/path/to/raylib/src \
  -L/path/to/raylib/lib -lraylib \
  -s USE_GLFW=3 \
  -s ASYNCIFY \
  -s FORCE_FILESYSTEM=1 \
  --preload-file Sounds \
  --preload-file Graphics \
  -DPLATFORM_WEB \
  -s EXPORTED_FUNCTIONS='["_main","_emscripten_run_script"]' \
  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'
```

---

## Serve Locally

After compiling, you can serve the project locally using a lightweight HTTP server like Python's built-in module:

```bash
python3 -m http.server 8000
```

Then open your browser at:

```
http://localhost:8000/snake.html
```

Ensure `snake.html`, `snake.js`, `snake.wasm`, and asset folders (`Sounds`, `Graphics`) are in the same directory.

---

## Contributing

Pull requests and suggestions are welcome!

1. **Fork the repository**

2. **Create a new branch**  
   ```bash
   git checkout -b feature/your-feature
   ```
3. **Commit your changes**  
   ```bash
   git commit -m "Add your feature"
   ```
4. **Push the branch**  
   ```bash
   git push origin feature/your-feature
   ```
5. **Open a Pull Request**
