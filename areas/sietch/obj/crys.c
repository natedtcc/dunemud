// This weapon was upgraded! It's now much stronger
// and able to be stored. - Bebop 2021

inherit "/obj/player/weapon";
#include <ansi.h>
#define E environment()
#define ENV(x) environment(x)
int home;
void reset (int arg) {
  ::reset(arg);
  if(!arg){
    set_type("Crysknife");
    set_class(25);
    set_weight(2);
    set_str_bonus(random(2));
    set_name("crysknife");
    set_alias("knife");
    set_short("A sharp crysknife");
    set_long("\
Huge tooth of Shai-Hulud with metal\n\
handle. This knife must be made by\n\
one of the master craftsmans of Arrakis.\n");
    set_wield_mesg(
      "You feel yourself stronger as you wield this crysknife.\n");
    set_unwield_mesg(
      "You feel no longer the power of Shai-Hulud.\n");
    set_hit_func(this_object());
    set_min_wield_level(20);
    auto_value();
  }
}

weapon_hit(attacker)
{
  // This weapon apparently would get stronger / weaker depending on
  // the wielder's guild. Removed! Crysknives for everyone! \o/ 
  //
  // string file,tmp;
  // file = object_name(ENV(E));
  // if((sscanf(file, "d/Fremen/%s", tmp) == 1) || (sscanf(file, "d/Speakers/%s", tmp) == 1)) 
  //   set_class(20); 
  // else 
  //   set_class(1);

  if(random(10)==1)
  {
    write(CYAN+"Your cut "+attacker->query_name()+" with power of Shai-Hulud.\n"+OFF);
    tell_object(attacker,this_player()->query_name()+" cuts you with blurring speed.\n");
    attacker->add_hp(-5);
  }
}
