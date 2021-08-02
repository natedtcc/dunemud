inherit "/obj/object";
void reset (int arg) {
  ::reset(arg);
  if(!arg){
    set_name("pearl");
    set_short("A pearl");
    set_long("Milk white pearl.\n");
    set_weight(1);
    set_value(1000);
  }
}

