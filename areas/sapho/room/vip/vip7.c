// vip7.c - VIP Lounge Hallway - Bebop 2021

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{
  ::reset(arg);
  if (!arg){

    set_light(1);
    set_short(parse_ansi("$WHITEThe $YELLOWVIP$OFF Lounge - Hallway"));
    set_long(
      "You are walking along the circular hallway inside the VIP area. There are\n"
      +"a couple large bouncers here, apparently standing watch over this room.\n"
      +"You begin to wonder why. There's a room leading to the south, but you\n"
      +"can't quite see past the bouncers.");

    add_exit("west", ROOM_DIR + VIP + "vip6.c");
    add_exit("east", ROOM_DIR + VIP +"vip8.c");
    add_exit("south", ROOM_DIR + VIP +"saferoom.c");

    add_item("bouncers",
      "Seems like they're guarding something.");

  }

    place_objects(NPC_DIR + "bouncer.c", 3);

}

void init(){
  ::init();

  add_action("kill", "kill");
  add_action("move", "south");

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
  if (str != "bouncer") return notify_fail("Kill what ?");
  return bounce_player("kill");
}

