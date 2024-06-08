#include <string>

#include "ouobEnemy.hpp"

ouobEnemy::ouobEnemy(int x, int y) : Enemy("play/enemy-4.png", x, y, 16, 50, 15, 15) {
	// Use bounding circle to detect collision is for simplicity, pixel-perfect collision can be implemented quite easily,
	// and efficiently if we use AABB collision detection first, and then pixel-perfect collision.
}
