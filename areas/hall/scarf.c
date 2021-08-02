inherit "/obj/player/armour";
reset(arg) {
::reset(arg);
if(arg)return;
  set_name("scarf");
  set_alias("linen");
  set_short("Linen scarf");
  set_long("A thin piece of cloth used to cover one's head. Although\n\
it seems to be frail, thin shigawire strips traverse it,\n\
deflecting many of the most powerful blows.\n");
  set_type("head");
  set_ac(5+random(3));
  set_weight(3);
  set_value(2000);
  //set_unique(1);
  set_origin("Caladan");
}
string query_auto_load() { return real_file(this_object()) + ":"; }
