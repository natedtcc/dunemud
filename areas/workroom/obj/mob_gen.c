// mob_gen.c - Bebop 2021

/* Generates mobs based on a define level
   or to the size of a player. */

int       scaled_level,
          variation = 10;

string    mob_filename;

int query_scaled_level(){return scaled_level;}

void set_scaled_level(int level){scaled_level = level;}

string query_mob_filename(){return mob_filename;}

void set_mob_filename(string fn){mob_filename = fn;}

void scale_to_player(object player, int variation){
  if (playerp(player)){
    scaled_level = player->query_level();
  }
}

int build_mob(object mob){
  if (objectp(mob) && scaled_level){
    mob->set_level(scaled_level+random(10));
    return 1;
  }
  return 0;
}

int spawn_mobs(object room, int count){
  obj* mobs = ({});
  if (roomp(room) && count > 0){
    for (int i=0;i<count%;i++){
      mobs += clone()
    }
    place_objects(mob_filename, count, room);
    return 1;
  }
  return 0;
}

