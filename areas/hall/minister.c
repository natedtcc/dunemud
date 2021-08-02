inherit "/obj/player/monster";

// Tidy'd up and fixed a very weird bug that was causing only case 0
// to fail (and /hall/research.c to break) - Bebop 2021

object armour;

void reset(status arg) {
::reset(arg);
if (!arg){
  set_name("minister");
  set_race("Atreides");
  switch(random(5)) {
   case 0:
     set_short("Minister of Commerce");
     set_alias( ({"minister", "commerce"}) );
     armour = clone_object("/d/Atreid/alan/hall/obj/scarf");
   break;
   case 1:
     set_short("Minister of Services");
     set_alias( ({"minister", "services"}) );
     armour = clone_object("/d/Atreid/alan/hall/obj/metaglass");
   break;
   case 2:
     set_short("Minister of Central Planning");
     set_alias( ({"minister", "planning"}) );
     armour = clone_object("/d/Atreid/alan/hall/obj/fanmetal");
   break;
   case 3:
     set_short("Minister of Treasury");
     set_alias( ({"minister", "treasury"}) );
     armour = clone_object("/d/Atreid/alan/hall/obj/jubba");
   break;
   case 4:
     set_short("Minister of Public Health");
     set_alias( ({"minister", "health"}) );
     armour = clone_object("/d/Atreid/alan/hall/obj/belt");
   break;
  }
  set_long("A civil servant of the town of Caladonia. This person controls\n\
a certain aspect of the city, and has final say over all decisions\n\
pertaining to that sector.\n");
  set_level(25+random(15));
  set_hp(level*250);
  set_ac(level*3);
  set_wc(level*2);
  set_gender(random(3));
  set_al(random(100)-75);
}

  if (armour){
    move_object (armour, this_object ());
    command("powerup");
  }

}
