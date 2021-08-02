// tyvek.c - Tyvek suit armour - Bebop 2021

inherit "/obj/player/armour";

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {
    set_name("coveralls");
    set_short("White Tyvek coveralls");
    set_alias("coveralls");
    set_wear_mesg("*ZIP* You step into the suit and zip up the zipper.\n");
    set_remove_mesg("You unzipper and remove your Tyvek suit.\n");

    set_long(
      "Made for working around hazardous materials, this Tyvek suit\n"+
      "covers your body (and clothes, obviously) to protect you from\n"+
      "chemical spills. It even has a little hood!. Aren't coveralls great?\n");

    set_type("total");
    set_ac(17);

    set_con_bonus(1);
    set_wis_bonus(2);
    set_weight(1);
    set_value(500);

    set_min_wear_level(50);
  }
}
