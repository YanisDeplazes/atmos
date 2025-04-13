import { defineConfig } from "vite";
import glsl from "vite-plugin-glsl";
import path from "path";

export default defineConfig({
  root: "./",
  plugins: [glsl()],
  server: {
    host: "0.0.0.0",
    port: 5173,
    strictPort: true,
    watch: { usePolling: true },
  },
  build: {
    outDir: "../../build/frontend",
    emptyOutDir: true,
    manifest: true,
    rollupOptions: {
      input: path.resolve(__dirname, "/assets/js/main.js"),
    },
  },
});
