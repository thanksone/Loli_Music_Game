#include <string>

#include "TapEnemy.hpp"

TapEnemy::TapEnemy(int x, int y, int t, int g, float s) : Enemy(t < 2? "play/sangoodtap.png" : "play/sanbadtap.png", t < 2? "play/sangoodtapbomb.png" : "play/sanbadtapbomb.png", x, y, t, g, s) {

}
