
inherit "/obj/player/weapon";

void reset(int arg)
{
  ::reset(arg);
  if(arg) return;

  set_name("knuckles");
  set_alias("brass_knuckles");
  set_short("A pair of studded brass knuckles");
  
  set_long(
  "These bad boys will do some serious damage to someone. The studs\n"
  +"are actually more like spikes, sharpened and honed perfectly.\n");

  set_weight(2);
  set_type("Hand to Hand");
  set_class(69);
  set_value(5000);
  set_min_wield_level(300);
}
