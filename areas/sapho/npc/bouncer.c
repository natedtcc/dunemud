// bouncer.c - Bouncer in the VIP area - Bebop 2018

inherit "/obj/player/monster";
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

object weapon, armor;

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {

    set_name("bouncer");
    set_alias( ({"guard", "bouncer"}) );

    set_short("A massive bouncer");
    set_long(
      "This dude is buff. Don't even think about messing with him.");

    set_gender(1);
    set_race("Imperial");

    // Let's make the bouncers a bit bigger, eh?
    set_level(BASE_MOB_LEVEL + (BASE_MOB_LEVEL / 4));
    improve_skill("Dodge", 500);
    improve_skill("Roll", 500);
    improve_skill("Block", 500);
    improve_skill("Hand to Hand", 600);

    // armor = clone_object( OBJ_DIR + ".c");
    weapon = clone_object ( OBJ_DIR + EQUIP + "knuckles.c");
  }
  if (armor)
    move_object(armor, this_object());

  if (weapon)
    move_object(weapon, this_object());

  if (armor || weapon)
    command("powerup");
  
}
