inherit "/obj/object";
void reset (int arg) {
  ::reset(arg);
  if(!arg){
    set_name("doll");
    set_short("A doll");
    set_long("Small, nice doll.\n");
    set_weight(1);
    set_value(100);
  }
}
