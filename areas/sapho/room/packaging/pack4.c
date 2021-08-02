// pack4.c - Sapho Packaging Area - Bebop 2020

#include <mudlib.h>
inherit ROOM;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(int arg)
{
  ::reset(arg);
  if (!arg){
    set_light(1);
    set_short(parse_ansi("$BOLD$REDSapho $OFF$WHITEPackaging Area$OFF"));
    set_long(
"You are walking along the outer wall of the sapho packaging area, away\n"
+"from the workers and their \"work\". Some guards are here, seemingly\n"
+"not caring you're here. The sterile white walls of the packaging area\n"
+"sharply reflect the light from the overhead lamps, which makes you squint\n"
+"uncomfortably. This area smells noticably less of sapho than the previous\n"
+"two areas, which you find quite relieving. Other than the occastional\n"
+"loose baggie sitting around, this place is kept quite tidy. Clearly, the\n"
+"workers don't mix work and play - rather, it's strictly prohibited.\n");

    add_exit("north", ROOM_DIR + PACK + "pack1.c");
    add_exit("east", ROOM_DIR + PACK + "pack5.c");
    add_exit("south", ROOM_DIR + PACK + "pack7.c");

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

  place_objects(NPC_DIR + "guard.c", 3);

}
