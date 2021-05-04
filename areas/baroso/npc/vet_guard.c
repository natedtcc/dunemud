/*
** Veteran Atreidies Marine Guard - Entrance Guard
** original by Kiza circa 1996
** upgraded by Bebop July 2018
*/

inherit "/d/Atreid/benedict/lib/atreides.c";

void reset(int arg)
{
  object wep, arm;
  ::reset(arg);
  if (arg)
    return;

  set_name("guard");
  set_race("Atreides");
  set_alias(({"atreid", "atreides"}));
  set_level(310 + random(7));
  set_hei(20);
  add_money(6000 + random(1500));
  ;

  set_short("Veteran Atreides $CYANMarine$OFF Guard");
  set_long("\
      This guard helps protect the security of Baroso.\n");
  set_gender(1);

  wep = clone_object("/d/Atreid/kiza/ubase/obj/lasgun.c");
  move_object(wep, this_object());
  command("wield " + wep->query_name());
  arm = clone_object("/d/Atreid/kiza/ubase/obj/muniform.c");
  move_object(arm, this_object());
  command("wear " + arm->query_name());
}
