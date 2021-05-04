// mask.c - Sapho Packaging Worker armor - Bebop 2021

inherit "/obj/player/armour";

void reset(int arg)
{
    ::reset(arg);
    if(arg) return;

    set_name("respirator");
    set_short("An N95 respirator");
    set_alias("mask");
    set_wear_mesg("The mask fits snugly over your face.\n");
    set_remove_mesg("The mask leaves an oddly shaped imprint on your face.\n");

    set_long(
      "This is a surgical grade mask, made for high-quality particle filtration.\n"
      +"It is capable of filtering 95% of all particles, hence the name \'N95'.\n");

    set_type("head");
    set_ac(8);

    set_con_bonus(2);
    set_weight(1);
    set_value(1000);

    set_min_wear_level(35);
}
