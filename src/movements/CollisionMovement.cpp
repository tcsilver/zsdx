/*
 * Copyright (C) 2009 Christopho, Zelda Solarus - http://www.zelda-solarus.com
 * 
 * Zelda: Mystery of Solarus DX is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Zelda: Mystery of Solarus DX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "movements/CollisionMovement.h"
#include "ZSDX.h"
#include "Game.h"
#include "Map.h"
#include "entities/MapEntity.h"

/**
 * Constructor.
 */
CollisionMovement::CollisionMovement(void) {
  last_collision_box_on_obstacle.x = 0;
  last_collision_box_on_obstacle.y = 0;
  last_collision_box_on_obstacle.w = 0;
  last_collision_box_on_obstacle.h = 0;
}

/**
 * Destructor.
 */
CollisionMovement::~CollisionMovement(void) {

}

/**
 * Returns whether the entity would collide with the map
 * if it was moved a few pixels from its position.
 * @param dx x distance between the current position and the position to check
 * @param dy y distance between the current position and the position to check
 * @return true if the entity would overlap the map obstacles in this position
 */
bool CollisionMovement::collision_with_map(int dx, int dy) {

  Map *map = entity->get_map();

  // place the collision box where we want to check the collisions
  SDL_Rect collision_box = entity->get_position_in_map();

  collision_box.x += dx;
  collision_box.y += dy;

  bool collision = map->collision_with_obstacles(entity->get_layer(), collision_box, entity);

  if (collision) {
    last_collision_box_on_obstacle = collision_box;
  }

  return collision;
}

/**
 * Updates the x position of the entity if it wants to move
 * (i.e. if x_move != 0).
 * This is a redefinition of Movement::update_x to make the move
 * only if there is no collision with the map.
 */
void CollisionMovement::update_x(void) {

  Uint32 now = SDL_GetTicks();
  if (x_move != 0 && now >= next_move_date_x) { // while it's time to try a move

    // make the move only if there is no collision
    if (!collision_with_map(x_move, 0)) {
      translate_x(x_move);
    }
    else {
      stop(); // also stop on y
    }
    next_move_date_x += x_delay;
  }
}

/**
 * Updates the y position of the entity if it wants to move
 * (i.e. if y_move != 0).
 * This is a redefinition of Movement::update_y to make the move
 * only if there is no collision with the map.
 */
void CollisionMovement::update_y(void) {

  Uint32 now = SDL_GetTicks();
  if (y_move != 0 && now >= next_move_date_y) { // while it's time to try a move

    // make the move only if there is no collision
    if (!collision_with_map(0, y_move)) {
      translate_y(y_move);
    }
    else {
      stop(); // also stop on x
    }
    next_move_date_y += y_delay;
  }
}

/**
 * Returns the collision box of the last call
 * to collision_with_map() returning true.
 * @return the collision box of the last collision detected
 */
const SDL_Rect& CollisionMovement::get_last_collision_box_on_obstacle(void) {
  return last_collision_box_on_obstacle;
}
