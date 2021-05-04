// include.c - Global variables for Sapho Den / Refinery - Bebop 2020

// Define folder location strings

string ROOM_DIR = "/players/bebop/area/sapho/room/";
string NPC_DIR = "/players/bebop/area/sapho/npc/";
string OBJ_DIR = "/players/bebop/area/sapho/obj/";

// Define chances of loot being found in the packaging area
// Currently ~1/3

int LOOT_CHANCE = 35;


// Return a random int between 0 and 100 that will decide
// the outcome of receiving loot from a box

int generate_loot_outcome();

int generate_loot_outcome()
{
  int rand = random(100);
  return rand;
}

