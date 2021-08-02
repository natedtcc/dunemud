// vip6.c - VIP Lounge Hallway - Bebop 2021

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

int has_searched;
void reset(int arg)
{

  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short(parse_ansi("$WHITEThe $YELLOWVIP$OFF Lounge - Hallway"));
    set_long(
  "You are walking along the circular hallway inside the VIP area. There are\n"
  +"a couple large bouncers here, apparently standing watch over these giant\n"
  +"boxes here in the room. I wonder what could be inside?\n");

    add_exit("south", ROOM_DIR + VIP + "vip4.c");
    add_exit("east", ROOM_DIR + VIP +"vip7.c");

    add_item("bouncers",
      "Seems like they're guarding something.");

    add_item("boxes",
      "The bouncers seem to be guarding these. Perhaps try searching them?");

    add_item("hallway",
      "It's a pretty plain hallway.");

  }

    has_searched = 0;
    place_objects(NPC_DIR + "bouncer.c", 2);

}

void init(){
  ::init();

  add_action("move", "east");
  add_action("kill", "kill");
  add_action("search", "search");

}
// NOTE: bounce_player(str type) is defined
// in /include/area_utils.c !!!

// If the player tries to move past the bouncer, they 
// get blocked. If they don't have a keycard, they
// get bounced.

int move()
{
  return bounce_player("move");
}

// If the player has no keycard and tries to kill any bouncers,
// they get thrown out of the area.

int kill (string str){
  object keycard;
  if (str != "bouncer") return notify_fail("Kill what ?");
  return bounce_player();
}

// Search the boxes for some ph4t l00tz

int search(string str)
{
  // Make sure the player has a keycard and
  // the guards are dead!
  if (bounce_player("search")) {return 1;}

  if (!bounce_player("search")){
    if(str != "boxes")
      return notify_fail("Search what?\n");

    // Search cooldown (until next reset)
    if (str == "boxes" && has_searched)
      return notify_fail ("There's nothing in here at the moment..\n");

    // All systems go...
    else{

      // Winner winner, chicken dinner!  
      // Currently 1/3 chance to get loot from a box
      if (generate_loot_outcome() <= LOOT_CHANCE){
        write(
          "You open one of the boxes and find a "
          +"large mass of red tar sapho!\n");

        object sapho_loot = clone_object(OBJ_DIR + "sapho_loot.c");
        move_object(sapho_loot, this_player());
      }

      // Failure..
      else{
        write(
          "You open one of the boxes but find "
          +"nothing! Try again later!\n");
        }
      // Disable searching til' next reload
      has_searched = 1;
      return 1;
    }
  }
}