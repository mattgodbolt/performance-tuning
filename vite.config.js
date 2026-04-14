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
  optimizeDeps: {
    entries: ["index.html"],
  },
  plugins: [
    htmlInclude(),
    viteStaticCopy({
      targets: [{ src: "reveal.js/dist", dest: "reveal.js" }],
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
