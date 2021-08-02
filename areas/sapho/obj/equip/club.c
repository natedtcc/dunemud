// club.c - Guard weapon - Bebop 2021

inherit "/obj/player/weapon";

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {

    set_name("club");
    set_alias("club");
    set_short("A big plasteel club");
    set_long(
      "This is a large blunt weapon, made entirely from plasteel, a legendary\n"
      +"material known for it's hardiness. The handle is made from leather and\n"
      +"is nicely worn in.\n");

    set_weight(1);
    set_type("Axes and Maces");
    set_class(18);
    set_value(850);

    set_str_bonus(1);

    set_wield_mesg(
      "You are ready to crush some skulls with minimal effort.\n");
    set_unwield_mesg(
      "You relenquish your ability to easily crush skulls.\n");

    set_min_wield_level(40);
  }
}
