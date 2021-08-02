// beaker.c - Processing worker weapon - Bebop 2021

inherit "/obj/player/weapon";

void reset(int arg)
{
  ::reset(arg);
  if (!arg) {

    set_name("flask");
    set_alias ( ({ "beaker", "flask" }) );
    set_short("A tall plaz flask");
    set_long(
      "Tall, thick, and construced from highly durable plaz, this volumetric\n"
      +"flask is traditionally used for preparing dilutions of various chemicals,\n"
      +"but it can be used as a highly effective blunt weapon.\n");

    set_weight(1);
    set_type("Axes and Maces");
    set_class(15);
    set_value(850);
    
    set_min_wield_level(40);
  }
}
