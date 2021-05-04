
// A Massive Cane
// by Bebop 2018

inherit "/obj/player/weapon";

void reset(int arg)
{
    ::reset(arg);
    if(arg) return;


    set_name("cane");
    set_alias("cane");
    set_short("A massive cane");
    set_long("\
You are looking at one huge cane. You feel like you could easily crush skulls\
with this thing. There is a large jewel on the top of the handle, sparkling\
rather wildly.\n");

    set_weight(7);
    set_type("Staffs and Oaks");
    set_class(50);
    set_value(10000);

    set_str_bonus(6);
    set_wis_bonus(6);

    set_wield_mesg("\
      The cane feels heavy, but you think you can swing it effectively.\n");
    set_unwield_mesg("\
   Your arms feel much better as you unwield the massive cane.");

        set_min_wield_level(200);
    set_unique(1);
}
