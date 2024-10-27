#pragma once

#include <map>

#include "Shader.h"

class Block {
  static int count;

 public:
  static Block byId[];
  Texture* textures[6];
  static int Create(Texture* top, Texture* bottom, Texture* front,
                    Texture* back, Texture* left, Texture* right);
  static int Create(Texture* top, Texture* bottom, Texture* side);
  static int Create(Texture* all);
};

class Chunk {
 public:
  char blocks[16][16][256];
};

class World {
  std::map<std::pair<int, int>, Chunk> chunks;

 public:
  void SetBlock(int x, int y, int z, char id);
  char GetBlock(int x, int y, int z);
};
