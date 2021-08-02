// door_guard.c - Door guard in the sapho facility - Bebop 2020

inherit "/obj/player/monster";
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

object weapon, armor;

void reset(status arg)
{
  ::reset(arg);
  if (!arg) {

    set_level(BASE_MOB_LEVEL);
    set_name("guard");
    set_race("Imperial");
    set_alias("soldier", "guard");
    set_gender(1);
    add_money(3000 + random(1000));
    set_short("Sapho-crazed guard");
    improve_skill("Dodge", 500);
    improve_skill("Roll", 500);
    improve_skill("Block", 500);
    improve_skill("Axes and Maces", 500);

    set_long(
      "His eyes glowing blood red, this guard seems to be in an almost\n"
      +"berserker-like state. His fingers are stained red with what appears\n"
      +"to be either sapho or blood. Judging by how he's glaring at you, it\n"
      +"could honestly be both.\n");

    set_non_combat_chats(25, ({
      ({"The guard lets out a gutteral scream. You feel terrified.", "echo"}),
      ({"*PUFF PUFF* The guard rips his sapho pipe and exclaims : HELL YEAH!", "echo"}),
      ({"Guard peers at you and says: \"You don't look familiar...\"", "echo"}),
      ({"Guard gags and coughs.", "echo"})}));

    set_combat_chats (20, ({
      ({ "The guard rushes you!", "echo" }),
      ({ "The guard starts circling you.", "echo" })
      }));

    armor = clone_object( OBJ_DIR + EQUIP + "plate.c");
    weapon = clone_object ( OBJ_DIR + EQUIP + "club.c");
    
  }
  if (armor)
    move_object(armor, this_object());

  if (weapon)
    move_object(weapon, this_object());

  if (armor || weapon)
    command("powerup");
  
  

}
