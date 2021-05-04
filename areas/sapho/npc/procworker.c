/// procworker.c - F in the sapho facility - Bebop 2020

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
  add_money(3000 + random(500));
  set_short("Packaging Plant Worker");

  set_long(
      "Covered in a red substance and sticky all over, this worker\n"
    +"is responsible for handling and packaging the red tar that\n"
    +"is created. To maximize product cleanliness and to make sure\n"
    +"no employees pocket any of the product, the worker is wearing\n"
    +"only a dust mask and his underwear. He doesn't look too happy.");

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
