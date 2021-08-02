inherit "/obj/player/monster";
object suit;
void reset (int arg)
{
  ::reset(arg);
  if(!arg)
  {
    set_level(40+random(20));
    set_name("guard");
    set_alias("fremen");
    set_short("A Fremen Guard");
    set_race("fremen");
    set_long("\
This is a huge, well trained fremen. You are\n\
impressed by his huge muscles and decide that\n\
it will be better to leave this guy alone.\n");
    set_hp(level*200+random(level*100));
    set_ac(70);
    set_wc(70);
    set_gender(1);
    improve_skill("Roll", random(70));
  }
  
  suit = clone_object("/d/Fremen/bebop/sietch/obj/suit");
  
  if (suit)
  {
    suit->set_size(this_object()->query_hei()/5+1);
    move_object(suit, this_object());
    command("wear suit");
  }

}

monster_died() {
  write("You see small mist above the dead corpse.\n");
  if(this_player()->query_guild()=="Fremen") {
    say("Dying fremen whispers : My fremen brothers will kill you, traitor !");
  } else {
    say("You hear a quiet voice comming from somewhere :"
    +" The god will pay you back for what you did !");
  }
 
}    

