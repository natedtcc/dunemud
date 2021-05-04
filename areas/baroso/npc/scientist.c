/*
** Atreides Scientist 1 - Marine Project Lab 3
** by Kiza 17-Jul-1996
** upgraded by Bebop July 2018
*/

inherit "/obj/player/monster";

void reset(int arg) {
  object arm;
  ::reset(arg);

  if(arg) return;

  set_name("scientist");
  set_alias(({"atreid", "atreides"}));
  set_short("$GREENAtreides$OFF Scientist");
  set_long(
    "A scientist for house Atreides, currently busily working on a marine\n"+
    "project.\n");
  set_level(380 + random (8));
  set_skill("Hand to Hand",1000);
  improve_skill("Hand to Hand",56);
  set_race("Atreides");
  add_money(4000 + random (1500));
  arm = clone_object("/d/Atreid/kiza/ubase/obj/labcoat.c");
  move_object(arm,this_object());
  init_command("wear "+arm->query_name());
}