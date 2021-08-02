// sign.c - Sapho Facility Warning Sign - Bebop 2021

#include <mudlib.h>
inherit OBJECT;
inherit "/d/Imperial/bebop/sapho/include/area_utils.c";

void reset(status arg)
{
  ::reset(arg);
  if (!arg) {
    set_name("sign");
    set_alias ( ({"sign"}) );
    set_short ("$BOLDA big $REDWARNING$WHITE sign$OFF");
    set_long(
      "\n$BOLD$RED                               WARNING!$OFF\n\n"
      +"$BOLDThe mobs in this area are much bigger than in the area above (level "
      + BASE_MOB_LEVEL +"+)\n\n"
      +"\n                       PROCEED AT YOUR OWN RISK!!\n\n"
      +"                               -Mgmt$OFF\n\n");
    
    set_no_get(1);
  }
}

void init ()
{
  ::init ();

  add_action ("do_read", "read");
}

int do_read (string str){
  if (str != "sign") {return notify_fail("Read what? The sign?\n");}

  tell_object(
      this_player(),
    "\n$BOLD$RED                               WARNING!$OFF\n\n"
    +"$BOLDThe mobs in this area are much bigger than in the area above (level "
    + BASE_MOB_LEVEL +"+)\n\n"
    +"\n                       PROCEED AT YOUR OWN RISK!!\n\n"
    +"                                -Mgmt$OFF\n\n");
    return 1;

}
