#ifndef WEBVIEW_BIND_EXAMPLE_HTML
#define WEBVIEW_BIND_EXAMPLE_HTML                                              \
  "<div>\n"                                                                    \
  "  <button id=\"increment\">+</button>\n"                                    \
  "  <button id=\"decrement\">−</button>\n"                                  \
  "  <span>Counter: <span id=\"counterResult\">0</span></span>\n"              \
  "</div>\n"                                                                   \
  "<hr />\n"                                                                   \
  "<div>\n"                                                                    \
  "  <button id=\"compute\">Compute</button>\n"                                \
  "  <span>Result: <span id=\"computeResult\">(not started)</span></span>\n"   \
  "</div>\n"                                                                   \
  "<script type=\"module\">\n"                                                 \
  "  const getElements = ids => Object.assign({}, ...ids.map(\n"               \
  "    id => ({ [id]: document.getElementById(id) })));\n"                     \
  "    const ui = getElements([\n"                                             \
  "    \"increment\", \"decrement\", \"counterResult\", \"compute\",\n"        \
  "    \"computeResult\"\n"                                                    \
  "  ]);\n"                                                                    \
  "  ui.increment.addEventListener(\"click\", async () => {\n"                 \
  "    ui.counterResult.textContent = await window.count(1);\n"                \
  "  });\n"                                                                    \
  "  ui.decrement.addEventListener(\"click\", async () => {\n"                 \
  "    ui.counterResult.textContent = await window.count(-1);\n"               \
  "  });\n"                                                                    \
  "  ui.compute.addEventListener(\"click\", async () => {\n"                   \
  "    ui.compute.disabled = true;\n"                                          \
  "    ui.computeResult.textContent = \"(pending)\";\n"                        \
  "    ui.computeResult.textContent = await window.compute(6, 7);\n"           \
  "    ui.compute.disabled = false;\n"                                         \
  "  });\n"                                                                    \
  "</script>"
#endif

#if defined(__cplusplus)
#include <random>

int random_number() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, 10000);
  return dist(gen);
}

#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#endif

// Make the current thread sleep for the given number of seconds.
void thread_sleep(int seconds) {
#ifdef _WIN32
  Sleep(seconds * 1000);
#else
  sleep(seconds);
#endif
}

int int_length(int n) {
  int count = 0;
  if (n == 0) {
    return 2;
  }
  if (n < 0) {
    count++;
    n = abs(n);
  }
  while (n != 0) {
    n /= 10;
    count++;
  }
  return count + 1;
}

const char *to_string(int value) {
  char result_str[int_length(value)];
  (void)sprintf(result_str, "%d", value);
  const char *result_char = result_str;
  return result_char;
}

const char *random_number(void) {
  srand((unsigned int)time(NULL));
  int random_num = rand() % 10001;
  return to_string(random_num);
}

#endif
