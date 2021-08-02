inherit "obj/player/monster";

#include "/d/Ix/include/cells.h"

int j;  /* counter for movement */
object con;

reset(arg)
{
  ::reset(arg);
  if(!arg)
  {
    set_level(50);
    set_hp(5000);
    set_name("mech");
    set_short("Zevdal the ICTS hunter MECH (nasty)");
    set_long("This is Zevdal the `hunter' of the ICTS.\n"+
             "He travels the cell system insearch of runners to disable.\n"+
             "His eyes examine you deeply.\n");
    set_alias("hunter");
    set_alt_name("zevdal");
    set_al(-1000);
    add_money(random(5000)+211);
    add_money(10000);
    set_heart_beat(1);

     log_file("kbl.quest",CTPRN+" solved ICTS Puzzle at "+ctime(time())+".\n");
     this_player ()->add_explorer_flag (hunter);

     this_player ()->set_quest ("ICTS_Puz");

     write("You feel more experienced.\n");
     return 1;
    }
   }
   write("Zevdal tells you: You do not have a ICTS puzzle! Go to the\n"+
         "                  tech shop on Ventura and get one. Now go away!\n");
   return 1;
  }
}
