#include "World.h"

#include <cmath>
#include <cstdlib>
#include <ctime>

map<string, int> ID = {
    {"grass_block", 1},
    {"dirt", 2},
    {"stone", 3},
    {"oak_log", 4},
    {"oak_leaves", 5},
    {"coal_ore", 6},
    {"copper_ore", 7},
    {"iron_ore", 8},
    {"deepslate", 9},
    {"deepslate_gold_ore", 10},
    {"deepslate_emerald_ore", 11},
    {"deepslate_diamond_ore", 12},
    {"bedrock", 13},
    {"inventory", 14},
};

World::World() {
  srand(time(0));
  seed = rand();
}

char& Chunk::operator()(int x, int y) { return data[x][y]; }

char& World::operator()(int x, int y) {
  if (y < 0 || y >= 128) printf("Block out of bound: %d %d\n", x, y);
  int cid = floor(x / 16.0f);
  if (chunks.find(cid) == chunks.end()) {
    Chunk chunk;
    srand(seed * (cid - 1));
    noise[cid - 1] = rand() % 16 + 64;
    srand(seed * cid);
    noise[cid] = rand() % 16 + 64;
    srand(seed * (cid + 1));
    noise[cid + 1] = rand() % 16 + 64;

    int x1 = x - 16, x2 = x, x3 = x + 16;
    int y1 = noise[cid - 1], y2 = noise[cid], y3 = noise[cid + 1];
    float denom = (x1 - x2) * (x1 - x3) * (x2 - x3);
    float a = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;
    float b =
        (x3 * x3 * (y1 - y2) + x2 * x2 * (y3 - y1) + x1 * x1 * (y2 - y3)) /
        denom;
    float c = (x2 * x3 * (x2 - x3) * y1 + x3 * x1 * (x3 - x1) * y2 +
               x1 * x2 * (x1 - x2) * y3) /
              denom;

    int tree = rand() % 8 + 4;

    for (int cx = 0; cx < 16; cx++) {
      int h = a * (x + cx) * (x + cx) + b * (x + cx) + c;
      for (int cy = h; cy < 128; cy++) {
        if (cy < 0) continue;
        if (cy - h == 0)
          chunk(cx, cy) = ID["grass_block"];
        else if (cy - h < 4)
          chunk(cx, cy) = ID["dirt"];
        else if (cy - h < 32) {
          chunk(cx, cy) = ID["stone"];
          if (rand() % 1000 < 40) chunk(cx, cy) = ID["coal_ore"];
          if (rand() % 1000 < 30) chunk(cx, cy) = ID["copper_ore"];
          if (rand() % 1000 < 20) chunk(cx, cy) = ID["iron_ore"];
        } else if (cy < 125) {
          chunk(cx, cy) = ID["deepslate"];
          if (rand() % 1000 < 10) chunk(cx, cy) = ID["deepslate_gold_ore"];
          if (rand() % 1000 < 5) chunk(cx, cy) = ID["deepslate_emerald_ore"];
          if (rand() % 1000 < 1) chunk(cx, cy) = ID["deepslate_diamond_ore"];
        } else
          chunk(cx, cy) = ID["bedrock"];
      }
      if (cx == tree) {
        for (int i = 1; i < 4; i++) {
          chunk(cx, h - i) = ID["oak_log"];
        }
        for (int i = 0; i < 2; i++) {
          for (int j = -2; j <= 2; j++) {
            chunk(cx + j, h - 4 - i) = ID["oak_leaves"];
          }
        }
        chunk(cx, h) = ID["dirt"];
        chunk(cx - 1, h - 6) = ID["oak_leaves"];
        chunk(cx + 1, h - 6) = ID["oak_leaves"];
        chunk(cx, h - 6) = ID["oak_leaves"];
        chunk(cx, h - 7) = ID["oak_leaves"];
      }
    }

    chunks[cid] = chunk;
  }
  return chunks[cid]((x % 16 + 16) % 16, y);
}

char World::safeRead(int x, int y) {
  if (y < 0 || y >= 128) return 0;
  return this->operator()(x, y);
}