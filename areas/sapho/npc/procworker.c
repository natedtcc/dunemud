/// procworker.c - Mobs in the sapho processing area - Bebop 2020

inherit "/obj/player/monster";
inherit "/players/bebop/area/sapho/include/include.c";

object weapon, armor;

void reset(status arg)
{
  ::reset(arg);
  if (arg)
    return;

  set_level(150);
  set_name("worker");
  set_race("none");
  set_gender(1);
  add_money(2800 + random(500));
  set_short("Processing area Worker");

  set_long(
      "This is a red-tar sapho processor, fully decked out in a DuPont Tyvek\n"
    +"chemical suit, working on getting the next batch of sapho ready for\n"
    +"processing. You can't see their face through the respirator they're\n"
    +"wearing, but you get the feeling they're looking you straight in the\n"
    +"eyes.");

  set_non_combat_chats(25, ({({"You see a processor carrying a large beaker full of strange crystals.", "echo"}),
                             ({"A worker climbs a small rolling staircase and dumps some liquid into a vat.", "echo"}),
                             ({"Worker says: \"This batch is gonna killer\".", "echo"}),
                             ({"One of the workers pulls a lever, and a large vacuum pump kicks on.", "echo"})}));

  ///armor = clone_object( OBJ_DIR + "armor.c");
  ///weapon = clone_object ( OBJ_DIR + "weapon.c");

  if (armor)
    move_object(armor, this_object());

  if (weapon)
    move_object(weapon, this_object());

  if (armor || weapon)
    command("powerup");
}
