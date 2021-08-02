inherit "/obj/object";
void reset (int arg) {
  ::reset(arg);
  if(!arg){
    set_name("pellet");
    set_short("Glass pellet");
    set_long("Small, glass pellet.\n");
    set_weight(1);
    set_value(100);
  }
}
