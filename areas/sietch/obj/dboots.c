inherit "/obj/player/armour";
void reset (int arg) {
  ::reset(arg);
  if(!arg){
    set_name("boots");
    set_alias("fremen boots");
    set_short("Fremen desert boots.");
    set_long("\
  Perfectly made leather fremen desert boots,\n\
  they probably save water during the desert walks\n");
    set_weight(3);
    set_ac(20);
    set_con_bonus(1);
    set_int_bonus(1);
    set_no_store(1);
    set_value(1500+random(1000));
    set_type("boots");
    set_resistance("slash",15);
  }
}