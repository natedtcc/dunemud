// Bebop's Lounge - Bebop 2018

#include <mudlib.h>
inherit ROOM;

void reset(int arg)
{

  ::reset(arg);
  if(!arg) {
    set_light(1);
    set_short(" The Lounge ");
    set_long(
  "\nAs you meander into this room, you are instantly met with\n"
  +"a thick cloud of smoke and the sound of jazz music bellowing out\n"
  +"of a nearby jukebox. You are surrounded by comfy looking leather\n"
  +"sofas, and the walls are covered with paintings and photographs.\n"
  +"Everyone here looks really....stoned.\n\n"
  +"Theres a bouncer. He looks huge.\n"
  +"There's a gate to the north, and a portal room to the East.\n\n");

    add_exit("north", "/d/Imperial/bebop/sapho/room/dhall1.c");
    add_exit("east", "/players/bebop/room/portals.c");
    add_exit("inner","/room/adv/inner");
    add_exit("northeast","/players/bebop/room/btest");
    add_exit("south","/d/Fremen/bebop/sietch/room/main/entr");
    add_item("jukebox", "An old style jukebox, filled with records.");
    add_item("records",
            "Coltrane, Mingus, Ellington, Davis... just to name a few.");
    add_item("sofa", "They look unbelievably comfortable.");
    add_item("walls",
            "The walls here are covered with portraits of famous jazz musicians.");
  }
  place_objects("/players/bebop/obj/zong.c", 1);
  place_objects("/players/bebop/npc/bouncer.c", 1);
  if (!present("ashtray", this_object())){
    place_objects("/players/bebop/obj/ashtray.c", 1);
  }
}

status no_clean_up() {
        return 1;
}

void init()
{
  ::init();

  add_action("testing", "testing");
}

int testing (string arg){
  if (arg == "check")
    if (this_player()->command("look"))
      return 1;
    else {
      write(this_player()->query_name());
    return 0;
    }
}
