#include <string>

#include "HoldEnemy.hpp"

HoldEnemy::HoldEnemy(int x, int y, int t, int g, float s) : Enemy(t < 2? "play/sangoodhold.png" : "play/sanbadhold.png", t < 2? "play/sangoodholdbomb.png" : "play/sanbadholdbomb.png", x, y, t, g, s) {

}
