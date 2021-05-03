/// worker.c - Worker in the sapho facility - Bebop 2020

inherit "/obj/player/monster";
inherit "/players/bebop/area/sapho/include/include.c";

object weapon, armor;

void reset(status arg)
{
  ::reset(arg);
  if (arg)
    return;

  set_level(160);
  set_name("worker");
  set_race("Imperial");
  set_alias("worker", "sard");
  set_gender(1);
  add_money(3000 + random(6000));
  set_short("Sapho facility worker");

  set_long(
      "His eyes glowing red, this guard seems to be in an almost\n" + "berserker-like state. His fingers are stained red with what\n" + "appears to be either sapho or blood. Judging by how he's\n" + "glaring at you, it could honestly be both.");

  set_non_combat_chats(25, ({({"The guard lets out a gutteral scream. You feel terrified.", "echo"}),
                             ({"*PUFF PUFF* The guard rips his sapho pipe and exclaims : HELL YEAH!", "echo"}),
                             ({"Guard peers at you and says: \"You don't look familiar...\"", "echo"}),
                             ({"Guard gags and coughs.", "echo"})}));

  ///armor = clone_object( OBJ_DIR + "armor.c");
  ///weapon = clone_object ( OBJ_DIR + "weapon.c");

  if (armor)
    move_object(armor, this_object());

  if (weapon)
    move_object(weapon, this_object());

  if (armor || weapon)
    command("powerup");
}
