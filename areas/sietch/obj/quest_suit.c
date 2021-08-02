inherit "/obj/player/armour";
void reset (int arg) {
  ::reset(arg);
  if(!arg){
    set_type("armour");
    set_ac(38);
    set_name("still-suit");
    set_alias(({ "suit", "still-suit", "ancient" }));
    set_short("Ancient Still-Suit");
    set_long("\
This still-suit has been made by the best\n\
Fremen masters. It saves your water greatly\n\
during the desert walks. It seems extremly\n\
precious.\n");
    set_weight(4);
    set_value(2000+random(8000));
    set_str_bonus(4);
    set_qui_bonus(4);
    set_dex_bonus(4);
    set_resistance("slash", 10);
    set_resistance("blunt", 2);
    set_resistance("pierce", 2);
    set_min_wear_level(100);
  }
}

