#include <cstring>
#include <iostream>

class Map {
public:
  Map(const char *map[]) {
    // Calculate the width and height of the map
    width_ = strlen(map[0]);
    height_ = 0;
    for (int i = 0; map[i] != nullptr; ++i) {
      ++height_;
    }
  }

  int getWidth() const { return width_; }

  int getHeight() const { return height_; }

private:
  int width_;
  int height_;
};

int main() {
  const char *map[] = {
      "#####################",
      "#         #         #",
      "# ### ### # ### ### #",
      "#                   #",
      "# ## #   ###   # ## #",
      "#    ###  #  ###    #",
      "#    #    #    #    #",
      "#      #     #      #",
      "#    # ####### #    #",
      "#    #         #    #",
      "#    #         #    #",
      "#    #         #    #",
      "#    #         #    #",
      "#    #         #    #",
      "#    #         #    #",
      "#####################",
      nullptr // End marker for the array
  };

  Map mapObject(map);

  std::cout << "Width: " << mapObject.getWidth() << std::endl;
  std::cout << "Height: " << mapObject.getHeight() << std::endl;

  return 0;
}
