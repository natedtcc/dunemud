inherit "/obj/player/weapon";
string finder;
void reset (int arg) {
  ::reset(arg);
  if(!arg){
    set_type("Crysknife");

    // Made this baby a lil' stronker - Bebop 2021
    set_class(35);
    set_weight(2);
    set_value(10000);
    set_wis_bonus(1);
    set_name("crysknife");
    set_alias("knife");
    set_short("An ancient crysknife");
    set_long(
      "This appears to have been taken from a Fremen slaughtering in\n"
      +"some Harkonnen military campaign. Someone might be looking\n"
      +"for this..\n");
    set_hit_func(this_object());
  }

}

weapon_hit(attacker) {
  if(!random(100)) {
   write("Your crysknife burns into "+attacker->query_name()
    +"'s skin with a red hot flare.\n");
   
   say(this_player()->query_name()+"'s crysknife burns into "
    +attacker->query_name()+"'s skin with a hot flash!\n");
   
   this_player()->add_hp(5);
   add_condition(50);
   return 100;
  }
}
query_quest_obj(){ return 5; }
query_no_store() { return 1; }
set_finder(string str) { finder = str; }
query_finder() { return finder; }
