import { readFileSync } from "fs";
import { resolve, dirname } from "path";
import { defineConfig } from "vite";
import { viteStaticCopy } from "vite-plugin-static-copy";

function htmlInclude() {
  const includedFiles = [];
  return {
    name: "html-include",
    transformIndexHtml: {
      order: "pre",
      handler(html, ctx) {
        const root = ctx.filename ? dirname(ctx.filename) : process.cwd();
        includedFiles.length = 0;
        html = html.replace(/<!--include\s+([\w.\/\-]+)\s*-->/g, (_, file) => {
          const path = resolve(root, file);
          includedFiles.push(path);
          return readFileSync(path, "utf-8");
        });
        return html;
      },
    },
    configureServer(server) {
      server.watcher.add(resolve(process.cwd(), "sections"));
      server.watcher.on("change", (file) => {
        if (includedFiles.includes(file)) {
          server.ws.send({ type: "full-reload" });
        }
      });
    },
  };
}

export default defineConfig({
  base: "./",
  // Restrict dep pre-bundling to our own entry - otherwise Vite's scanner
  // walks into reveal.js/test/test.html and fails on its qunit imports.
  optimizeDeps: {
    entries: ["index.html"],
  },
  build: {
    // reveal.js's highlight.mjs is a pre-bundled ~1MB artifact with all
    // highlight.js languages baked in via side-effect registration - not
    // tree-shakeable. Lift the warning above that.
    chunkSizeWarningLimit: 1200,
  },
  plugins: [
    htmlInclude(),
    viteStaticCopy({
      targets: [
        { src: "reveal.js/dist", dest: "reveal.js" },
        // Vite's HTML scanner only rewrites known asset attributes (<img src>
        // etc.), so reveal.js's data-background-image/video/iframe URLs are
        // left untouched and never copied to dist. Copy the whole images dir
        // verbatim so any data-background-* reference Just Works.
        { src: "images", dest: "." },
      ],
    }),
    {
      name: "md-reload",
      configureServer(server) {
        server.watcher.add("*.md");
        server.watcher.on(
          "change",
          (file) => file.endsWith(".md") && server.ws.send({ type: "full-reload" }),
        );
      },
    },
  ],
});
