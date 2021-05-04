// Keycard Generator - Bebop 2020

/* Spawn a keycard on one random enemy located in the 
 Sapho Production areas - included/called in: <ROOM_DIR/sapho9.c> */

inherit "/players/bebop/area/sapho/include/include.c";

void keycard_gen()
{

  int rand_int;
  object keycard;

  /* Get array of all mobs in the area (minus the doorguards)
   and generate  random int based off of the total mob count*/

  object *mob_array =           clones(NPC_DIR + "guard.c", 2)+
                                clones(NPC_DIR + "packworker.c", 2)+
                                clones(NPC_DIR + "procworker.c", 2);
  
  
  rand_int = random(sizeof(mob_array));

  // If no keycard clones are found & mobs are available..

  if (sizeof(clones(OBJ_DIR + "keycard.c", 2)) == 0 
    && sizeof(mob_array) > 0 && (keycard = clone_object(OBJ_DIR + "keycard.c")))
  {
    keycard &&  move_object(keycard, mob_array[rand_int]);
  }
}