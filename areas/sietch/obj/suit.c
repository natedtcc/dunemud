inherit "/obj/player/armour";
void reset (int arg) {
  ::reset(arg);
  if(!arg){
    set_type("armour");
    set_ac(20);
    set_name("still-suit");
    set_alias("suit");
    set_short("Fremen still-suit");
    set_long("\
This Fremen still-suit is a perfect handcraft of\n\
the fremen craftsmen and is made of stillcloth\n\
with interwoven tubes. It could me basically described\n\
as a micro-sandwich which acts as high efficiency\n\
filter and heat-exchange system. Wearing this\n\
you lose very little of your body moisture.\n");
    set_weight(4);
    set_value(1500+random(500));
  }
}


