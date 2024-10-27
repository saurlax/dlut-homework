#pragma once
#include <map>
#include <string>
using namespace std;

extern map<string, int> ID;

class Chunk {
  char data[16][128] = {};

 public:
  char& operator()(int x, int y);
};

class World {
  map<int, Chunk> chunks;
  map<int, int> noise;
  int seed;

 public:
  World();
  char& operator()(int x, int y);
  char safeRead(int x, int y);
};