/* Atreidies Marine Guard - Entrance Guard */
/* Scaled up by Bebop June 2018 */
inherit "/d/Atreid/benedict/lib/atreides.c";

void reset(int arg)
{
  object wep, arm;
  ::reset(arg);
  if (arg)
    return;

  set_name("guard");
  set_race("Atreides");
  set_guild_level(1);
  set_alias(({"atreid", "atreides"}));
  set_level(295);
  add_money(5000 + random(2000));

  set_short("Atreides $CYANMarine$OFF Guard");
  set_long(
      "This guard help protects the security of Baroso.\n");
  set_gender(1);

  wep = clone_object("/d/Atreid/kiza/ubase/obj/lasgun.c");
  move_object(wep, this_object());
  command("wield " + wep->query_name());
  arm = clone_object("/d/Atreid/kiza/ubase/obj/muniform.c");
  move_object(arm, this_object());
  command("wear " + arm->query_name());
}
