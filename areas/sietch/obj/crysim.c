inherit "/obj/object";
void reset (int arg) {
  ::reset(arg);
  if(!arg){
    set_name("knife");
    set_alias("crysknife");
    set_short("Crysknife imitation");
    set_long("Crysknife imitation made of steel.\n");
    set_weight(1);
    set_value(1000);
  }
}
