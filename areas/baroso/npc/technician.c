/* 
** Atreides Technician Guard
** by Kiza 10-Jul-1996
** upgraded by Bebop July 2018 
*/

inherit "/obj/player/monster";

void reset(int arg){
    object arm;
    ::reset(arg);
    if(arg) return;
    set_name("tech");
    set_race("Atreides");
    set_alias(({"atreid", "atreides"}));
    set_level(290 + random (8));
    add_money(4000 + random (1000));
  /* Gave this guy some H2H skill, like his Scientist friend -Bebop */
    set_skill("Hand to Hand",1000);
    improve_skill("Hand to Hand",56);
    set_short("$GREENAtreides$OFF Technician");
    set_long("\
      This is a lab technician who works on one of the many projects in \
      the base.\n");
    set_gender(1);
    arm = clone_object("/d/Atreid/kiza/ubase/obj/labcoat.c");
    move_object(arm,this_object());
    command("wear "+arm->query_name());
}

