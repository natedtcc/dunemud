/// pack7.c - Sapho Packaging Area - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/players/bebop/area/sapho/include/include.c";

void reset(int arg)
{

  ::reset(arg);

  place_objects(NPC_DIR + "guard.c", 3);

  if (arg)
    return;
  set_light(1);
  set_short("$BOLD$REDSapho $OFF$WHITEPackaging Area$OFF");
  set_long(
      "You are walking along the outer wall of the sapho packaging area, away from the\n"
      +"workers and their \"work\". Some guards are here, seemingly not caring you're here.\n"
      +"The sterile white walls of the packaging area sharply reflect the light from the \n"
      +"overhead lamps, which makes you squint uncomfortably. This area smells noticably\n"
      +"less of sapho than the previous two areas, which you find quite relieving. Other\n"
      +"than the occastional loose baggie sitting around, this place is kept quite tidy.\n"
      +"Clearly, the workers don't mix work and play - rather, it's strictly prohibited.\n");

  add_exit("north", ROOM_DIR + "pack4.c");
  add_exit("east", ROOM_DIR + "pack8.c");
  add_exit("south", ROOM_DIR + "pack10.c");

  add_item(
    "baggie",
      "An empty baggie used for packaging red-tar. There's nothing in it, you junkie.");

  add_item(
      "walls",
      "Immaculately white, and highly reflective.");

  add_item(
      "workers",
        "You can see them off in the distance, hard at work.");

}