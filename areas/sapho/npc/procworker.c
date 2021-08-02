// procworker.c - Mobs in the sapho processing area - Bebop 2020

inherit "/obj/player/monster";
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

object weapon, armor, armor2;

void reset(status arg)
{
  ::reset(arg);
  if (!arg) {

    set_level(BASE_MOB_LEVEL + random(10));
    set_name("worker");
    set_race("none");
    set_gender(1);
    add_money(2800 + random(500));
    set_short("Processing area worker");
    improve_skill("Dodge", 500);
    improve_skill("Roll", 500);
    improve_skill("Block", 500);
    improve_skill("Axes and Maces", 500);

    set_long(
        "This is a red-tar sapho processor, fully decked out in a DuPont Tyvek\n"
      +"chemical suit, working on getting the next batch of sapho ready for\n"
      +"processing. You can't see their face through the respirator they're\n"
      +"wearing, but you get the feeling they're looking you straight in the\n"
      +"eyes.");

    set_non_combat_chats(25, ({
      ({"You see a processor carrying a large beaker full of strange crystals.", "echo"}),
      ({"A worker climbs a small rolling staircase and dumps some liquid into a vat.", "echo"}),
      ({"Worker says: \"This batch is gonna be killer\".", "echo"}),
      ({"One of the workers pulls a lever, and a large vacuum pump kicks on.", "echo"})
      }));

    set_combat_chats (20, ({
      ({ "The worker shrieks and swings his beaker at you!", "echo" }),
      ({ "The worker is thirsty for blood.", "echo" })
      }));     

      armor = clone_object( OBJ_DIR + EQUIP + "tyvek.c");
      armor2 = clone_object( OBJ_DIR + EQUIP + "mask.c");
      weapon = clone_object ( OBJ_DIR + EQUIP + "beaker.c");

  }

  if (armor)
    move_object(armor, this_object());

  if (armor2)
    move_object(armor2, this_object());

  if (weapon)
    move_object(weapon, this_object());

  if (armor || armor2 || weapon)
    command("powerup");

}