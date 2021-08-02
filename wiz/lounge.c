// Bebop's Lounge - Bebop 2018

#include <mudlib.h>
inherit ROOM;

void reset(int arg)
{

  if (!present("ashtray"))
    place_objects("/players/bebop/obj/ashtray.c", 1);
  if (!present("zong"))
    place_objects("/players/bebop/obj/zong2.c", 1);
  if (!present("bouncer"))
    move_object(clone_object("/players/bebop/npc/bouncer.c"),
                this_object());

  ::reset(arg);

  if (arg)
    return;

  set_light(1);
  set_short(" The Lounge ");
  set_long(
      "\nAs you meander into this room, you are instantly met with\n" + "a thick cloud of smoke and the sound of jazz music bellowing out\n" + "of a nearby jukebox. You are surrounded by comfy looking leather\n" + "sofas, and the walls are covered with paintings and photographs.\n" + "Everyone here looks really....stoned.\n\n" + "Theres a bouncer by the west exit. He looks huge.\n" + "There's a gate to the north, and a portal to Caladan to the East.\n\n");

  add_exit("west", "/players/bebop/workroom.c");
  add_exit("north", "/players/bebop/area/sapho/room/dhall1.c");
  add_exit("portal", "/d/Space/stryder/planets/cal_station");
  add_item("jukebox", "An old style jukebox, filled with records.");
  add_item("records",
           "Coltrane, Mingus, Ellington, Davis... just to name a few.");
  add_item("sofa", "They look unbelievably comfortable.");
  add_item("walls",
           "The walls here are covered with portraits of famous jazz musicians.");
}

void init()
{
  ::init();

  add_action("check", "west");
}

check()
{
  if (present("bouncer"))
  {
    tell_object(this_player(),
                "The Bouncer says: Sorry, bub. Members only.\n");
    tell_room(this_object(),
              "The Bouncer says to " +
                  this_player()->query_name() +
                  ": Sorry, bub. Members only.",
              ({this_player()}));
    return 1;
  }
  return 0;
}
