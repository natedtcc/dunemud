// cane.c - Gabriel's cane - Bebop 2018

inherit "/obj/player/weapon";

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {

    set_name("cane");
    set_alias("cane");
    set_short("A massive cane");
    set_long(
      "You are looking at one huge cane. You feel like you could easily crush skulls\n"
      +"with this thing. There is a large jewel on the top of the handle, sparkling\n"
      +"rather wildly.\n");

    set_weight(2);
    set_type("Staffs and Oaks");
    set_class(38);
    set_value(5000);

    set_str_bonus(6);
    set_wis_bonus(6);

    set_wield_mesg(
      "The cane feels heavy, but you think you can swing it effectively.");
    set_unwield_mesg(
      "Your arms feel much better as you unwield the massive cane.");

    set_min_wield_level(120);
  }
}

