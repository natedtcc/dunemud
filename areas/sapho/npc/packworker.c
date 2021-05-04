/// procworker.c - F in the sapho facility - Bebop 2020

inherit "/obj/player/monster";
inherit "/players/bebop/area/sapho/include/include.c";

object weapon, armor;

void reset(status arg)
{
  ::reset(arg);
  if (arg)
    return;

  set_level(75);
  set_name("worker");
  set_race("none");
  set_gender(1);
  add_money(3000 + random(500));
  set_short("Packaging area worker");

  set_long(
      "Covered in a red substance and sticky all over, this worker\n"
    +"is responsible for handling and packaging the red tar that\n"
    +"is created. To maximize product cleanliness and to make sure\n"
    +"no employees pocket any of the product, the worker is wearing\n"
    +"only a dust mask and his underwear. He doesn't look too happy.");

  set_non_combat_chats(30, ({
    ({"The worker weighs a fat rock of sapho on a digital scale.", "echo"}),
    ({"You see a worker putting a rock of red tar into a small bag.", "echo"}),
    ({"Worker looks at you.", "echo"})
      }));

  armor = clone_object( OBJ_DIR + "mask.c");

  if (armor)
    move_object(armor, this_object());

  if (weapon)
    move_object(weapon, this_object());

  if (armor || weapon)
    command("powerup");
}
