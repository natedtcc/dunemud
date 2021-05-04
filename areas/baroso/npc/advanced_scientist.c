/*
** Atreides Food Tech Scientist
** by Kiza 21-Jul-1996
** upgraded by Bebop July 2018
*/

inherit "/obj/player/monster";

void reset(int arg) {
  object arm;
  ::reset(arg);
  if(arg) return;  // Varrack 120404
  set_name("scientist");
  set_alias(({"atreid", "atreides"}));
  set_short("Advanced $GREENAtreides$OFF Scientist");
  set_long(
    "A scientist for House Atreides, currently working on a food technology"
    "project.\n");
  set_level(400 + random (7));
  set_skill("Hand to Hand",1000);
  set_race("Atreides");
  add_money(5000 + random (2000));
  arm = clone_object("/d/Atreid/kiza/ubase/obj/labcoat.c");
  move_object(arm,this_object());
  init_command("wear "+arm->query_name());
}
