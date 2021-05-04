// suit.c - Gabriel's armour - Bebop 2018

inherit "/obj/player/armour";

void reset(int arg)
{
    ::reset(arg);
    if(arg) return;

    set_name("suit");
    set_short("An expensive looking suit");
    set_alias("suit");
    set_wear_mesg("You feel fresher than a maufk\'.\n");
    set_remove_mesg("Your freshness has decreased noticably.\n");

    set_long(
      "This suit is extremely exquisite. It\'s previous owner must\n"+
      "have spent a ton getting this thing custom fitted. The label\n"+
      "on the collar reads \'Gucci\'.\n");

    set_type("total");
    set_ac(19);

    set_con_bonus(4);
    set_con_bonus(4);
    set_weight(2);
    set_value(10000);

    set_min_wear_level(50);
}
