// plate.c - Guard armour - Bebop 2021

inherit "/obj/player/armour";

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {

    set_name("plate");
    set_short("A nano-weave plate");
    set_alias("plate");
    set_wear_mesg("Your stomach feels well protected.\n");
    set_remove_mesg("Your stomach seems less protected.\n");

    set_long(
      "This plate is a highly advanced textile with an ultra tight weave,\n"+
      "providing additional protection from enemy attacks as well as extreme\n"+
      "elements.\n");

    set_type("stomach");
    set_ac(18);

    set_weight(1);
    set_value(600);

    set_min_wear_level(40);
  }
}
