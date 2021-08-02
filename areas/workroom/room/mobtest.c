// Battle testing - Bebop 2021

#include <mudlib.h>
inherit ROOM;
// inherit "/players/bebop/obj/mob_gen";

void reset(int arg)
{

  ::reset(arg);
  if(!arg){
    set_light(1);
    set_short(" Combat Testing ");
    set_long(
        "\nTest room.\n"
        +"Type 'healme' if you need a quick heal.\n");

    add_exit("north", "/players/bebop/room/wimpy");
    }

    place_objects("/players/bebop/npc/testmob", 2);
}

void init()
{
  ::init();

  add_action("healme", "healme");
}

int healme(){
  if (this_player()->query_hp() >= this_player()->query_max_hp())
    return notify_fail("You're already at full health, dingus.\n");
  this_player()->add_hp(
    this_player()->query_max_hp() - this_player()->query_hp()
  );
  tell_object(this_player(),
    "You are magically healed.\n"
  );
  return 1;
}